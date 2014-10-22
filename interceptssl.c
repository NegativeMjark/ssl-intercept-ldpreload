#include <openssl/ssl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <nss/pk11pub.h>

const int MAX_NAME = 1024;



static SECStatus (*next_encrypt)(PK11SymKey *,
                       CK_MECHANISM_TYPE mechanism, SECItem *param,
                       unsigned char *out, unsigned int *outLen,
                       unsigned int maxLen,
                       const unsigned char *data, unsigned int dataLen) = NULL;

SECStatus PK11_Encrypt(PK11SymKey *symKey,                                             
                       CK_MECHANISM_TYPE mechanism, SECItem *param,                    
                       unsigned char *out, unsigned int *outLen,                       
                       unsigned int maxLen,                                            
                       const unsigned char *data, unsigned int dataLen){
    if (next_encrypt == NULL) {
        next_encrypt = dlsym(RTLD_NEXT, "PK11_Encrypt");
    }

    SECStatus ret = next_encrypt(symKey, mechanism, param,out,outLen,maxLen,data,dataLen);
    
    if(ret == 0){
        char filename[MAX_NAME];
        FILE *file;
        snprintf(filename, sizeof(filename), "SSL_write.%p", symKey);
        file = fopen(filename, "a");
        if (file) {
            fwrite(data, 1, dataLen, file);
            fclose(file);
        }
    }

    return ret;
}

SECStatus (*next_decrypt)(PK11SymKey *symkey,
                       CK_MECHANISM_TYPE mechanism, SECItem *param,
                       unsigned char *out, unsigned int *outLen,
                       unsigned int maxLen,
                       const unsigned char *enc, unsigned int encLen) = NULL;

SECStatus PK11_Decrypt(PK11SymKey *symkey,
		       CK_MECHANISM_TYPE mechanism, SECItem *param,
		       unsigned char *out, unsigned int *outLen,
		       unsigned int maxLen,
		       const unsigned char *enc, unsigned int encLen){

    if (next_decrypt == NULL) {
        next_decrypt = dlsym(RTLD_NEXT, "PK11_Decrypt");
    }
	
    SECStatus ret = next_decrypt(symkey,mechanism,param,out,outLen,maxLen,enc,encLen);
    if(ret == 0){
        char filename[MAX_NAME];
        FILE *file;
        snprintf(filename, sizeof(filename), "SSL_read.%p", symkey);
        file = fopen(filename, "a");
        if (file) {
            fwrite(out, 1, outLen[0], file);
            fclose(file);
        }
    }

    return ret;
}

static int (*next_SSL_read)(SSL *ssl, void *buf, int num) = NULL;

int SSL_read(SSL *ssl, void *buf, int num) {
    if (next_SSL_read == NULL) {
        next_SSL_read = dlsym(RTLD_NEXT, "SSL_read");
    }

    int ret = next_SSL_read(ssl, buf, num);

    if (ret > 0) {
        char filename[MAX_NAME];
        FILE *file;
        snprintf(filename, sizeof(filename), "SSL_read.%p", ssl);
        file = fopen(filename, "a");
        if (file) {
            fwrite(buf, 1, ret, file);
            fclose(file);
        }
    }
    return ret;
}

int (*next_SSL_write)(SSL *ssl, const void *buf, int num) = NULL;

int SSL_write(SSL *ssl, const void *buf, int num) {
    if (next_SSL_write == NULL) {
        next_SSL_write = dlsym(RTLD_NEXT, "SSL_write");
    }

    int ret = next_SSL_write(ssl, buf, num);

    if (ret > 0) {
        char filename[MAX_NAME];
        FILE *file;
        snprintf(filename, sizeof(filename), "SSL_write.%p", ssl);
        file = fopen(filename, "a");
        if (file) {
            fwrite(buf, 1, ret, file);
            fclose(file);
        }
    }
    return ret;
}
