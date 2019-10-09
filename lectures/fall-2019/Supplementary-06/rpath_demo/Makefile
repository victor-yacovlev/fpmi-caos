first:
	echo "Pass: library, program-no-rpath or program-with-rpath"

program-no-rpath: library ./src/program.c
	mkdir bin || true
	gcc -o ./bin/program ./src/program.c \
			-L./lib -lmygreatlib

program-with-rpath: library ./src/program.c
	mkdir bin || true
	gcc -o ./bin/program ./src/program.c \
			-L./lib -lmygreatlib \
			-Wl,-rpath,'$$ORIGIN/../lib/'

library: ./src/mygreatlib.c ./src/mygreatlib.h
	mkdir lib || true
	gcc -shared -fPIC -o ./lib/libmygreatlib.so ./src/mygreatlib.c

clean:
	rm -f ./bin/program
	rm -f ./lib/libmygreatlib.so
