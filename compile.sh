for file in *.c; do cc -I/usr/local/include -L/usr/local/lib $file -lczmq  -o $file.com;  chmod +x $file.com; done
