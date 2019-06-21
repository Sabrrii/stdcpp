all: bin test

bin:
	g++ main.cpp -o factory && ./factory --help && ./factory --list

test:
	echo;echo "factory:"
	./factory -d laptop; echo
	./factory -d desktop; echo
	./factory -d NumExo2_ExoGam; echo
	echo;echo "errors:"
	./factory -d UnKnown; echo
	./factory -d NumExo2_error; echo

