build:
	gcc *.c -o tema1 -g

test: build
	valgrind --leak-check=full ./tema1

run: build
	valgrind --leak-check=full ./tema1

clean:
	rm -rf tema1 output*.out
