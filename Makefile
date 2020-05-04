BIN=hello

all: bin doc run

bin:
	$(CXX) main.cpp -o $(BIN) && ./$(BIN) --help
	./$(BIN) --help > $(BIN)_help.output && ./$(BIN) --version > VERSION

doc:
	doxygen && ls -lah doc/html

run:
	echo;echo "Hello:"
	./$(BIN)

