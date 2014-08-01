default: libintercept.so

libintercept.so:
	gcc -shared interceptssl.c -o libintercept.so -ldl -D_GNU_SOURCE -fpic
