gcc -c libuart.c
ar -cr libuart.a libuart.o
gcc test.c -o test ./libuart.o
