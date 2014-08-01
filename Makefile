default: libinterceptssl.so

libinterceptssl.so:
	gcc -shared interceptssl.c -o libinterceptssl.so -ldl -D_GNU_SOURCE -fpic
