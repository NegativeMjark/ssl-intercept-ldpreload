libinterceptssl.so: interceptssl.c
	gcc -shared interceptssl.c -o libinterceptssl.so \
		-Wl,--no-as-needed -ldl -lssl -D_GNU_SOURCE -fpic

default: libinterceptssl.so
