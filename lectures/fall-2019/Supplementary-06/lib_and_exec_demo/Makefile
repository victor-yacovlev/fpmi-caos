first:
	echo "Pass: library, program or lib-and-program"

lib-and-program: file.c
	gcc -o lib-and-program \
		-fPIC \
		-pie \
		-Wl,-E \
		file.c

program: file.c
	gcc -o program file.c

library: file.c
	gcc -shared -fPIC -o library.so file.c

clean:
	rm -f library.so
	rm -f program
	rm -f lib-and-program
