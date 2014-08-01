#include <openssl/ssl.h>
#include <dlfcn.h>
#include <stdio.h>

const int MAX_NAME = 1024;

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
