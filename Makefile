default: libinterceptssl.so

libinterceptssl.so:
	gcc -I/usr/include/nss -I/usr/include/nspr/ -shared interceptssl.c -o libinterceptssl.so \
		-Wl,--no-as-needed -ldl -lssl -D_GNU_SOURCE -fpic
