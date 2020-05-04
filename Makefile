#main binary name should be consistent with CI, e.g. name of the branch (see .gitlab-ci.yml)
BIN=factory

all: bin doc run

bin:
	$(CXX) main.cpp -o $(BIN) && ./$(BIN) --help && ./factory --list
	./$(BIN) --help > $(BIN)_help.output && ./$(BIN) --version > VERSION

doc:
	doxygen && ls -lah doc/html

run:
	echo;echo "factory:"
	./$(BIN) --list
	./$(BIN) -d laptop; echo
	./$(BIN) -d desktop; echo

test:
	echo;echo "factory:"
	./$(BIN) -d laptop; echo
	./$(BIN) -d desktop; echo
	./$(BIN) -d NumExo2_ExoGam; echo
	echo;echo "errors:"
	./$(BIN) -d UnKnown; echo
	./$(BIN) -d NumExo2_error; echo

