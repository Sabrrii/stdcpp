all: bin doc run

bin:
	$(CXX) main.cpp -o factory && ./factory --help && ./factory --list
	./factory --help > factory_help.output

doc:
	doxygen && ls -lah doc/html

run:
	echo;echo "factory:"
	./factory --list
	./factory -d laptop; echo
	./factory -d desktop; echo

test:
	echo;echo "factory:"
	./factory -d laptop; echo
	./factory -d desktop; echo
	./factory -d NumExo2_ExoGam; echo
	echo;echo "errors:"
	./factory -d UnKnown; echo
	./factory -d NumExo2_error; echo

