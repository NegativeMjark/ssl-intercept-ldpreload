default: libinterceptssl.so

libinterceptssl.so:
	gcc -shared interceptssl.c -o libinterceptssl.so \
		-Wl,--no-as-needed -ldl -lssl -D_GNU_SOURCE -fpic
