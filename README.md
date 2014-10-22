ssl-intercept-ldpreload
=======================

LD_PRELOAD library for intercepting the plain text of SSL connections made 
with openssl & NSS

Usage:
    sudo apt-get install libnss3-dev

    make

    LD_PRELOAD=./libinterceptssl.so curl https://www.google.com    

    LD_PRELOAD=./libinterceptssl.so firefox
	
