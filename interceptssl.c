#include <openssl/ssl.h>
#include <dlfcn.h>
#include <stdio.h>

const int MAX_NAME = 1024;

static char *OUTPUT_DIR = "";

static void set_output_dir() {
    char *output_dir = getenv("INTERCEPT_SSL_OUTPUT_DIR");
    if (output_dir) {
        OUTPUT_DIR = output_dir;
    }
}

static int (*next_SSL_read)(SSL *ssl, void *buf, int num) = NULL;

int SSL_read(SSL *ssl, void *buf, int num) {
    if (next_SSL_read == NULL) {
        next_SSL_read = dlsym(RTLD_NEXT, "SSL_read");
        set_output_dir();
    }

    int ret = next_SSL_read(ssl, buf, num);

    if (ret > 0) {
        char filename[MAX_NAME];
        FILE *file;
        snprintf(filename, sizeof(filename),
                 "%s/SSL_read.%p", OUTPUT_DIR, ssl);
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
        set_output_dir();
    }

    int ret = next_SSL_write(ssl, buf, num);

    if (ret > 0) {
        char filename[MAX_NAME];
        FILE *file;
        snprintf(filename, sizeof(filename),
                 "%s/SSL_write.%p", OUTPUT_DIR, ssl);
        file = fopen(filename, "a");
        if (file) {
            fwrite(buf, 1, ret, file);
            fclose(file);
        }
    }
    return ret;
}
