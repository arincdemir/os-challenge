Build:
	gcc -o server server.c ssltest.c -L/usr/lib -lcrypto

clean:
	rm -f server