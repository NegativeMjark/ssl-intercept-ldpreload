ssl-intercept-ldpreload
=======================

LD_PRELOAD library for intercepting the plain text of SSL connections made 
with openssl

Usage:

    LD_PRELOAD=./libinterceptssl.so curl https://www.google.com    
