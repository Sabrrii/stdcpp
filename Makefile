BIN=hello_cimg

all: bin doc run

#no display
bin:
	$(CXX) -Dcimg_display=0 main.cpp -o $(BIN) && ./$(BIN) --help
	./$(BIN) --help > $(BIN)_help.output && ./$(BIN) --version > VERSION

#display
binX:
	$(CXX) main.cpp -o $(BIN)X && ./$(BIN)X --help
	./$(BIN)X --help > $(BIN)X_help.output && ./$(BIN)X --version > VERSION

doc:
	doxygen && ls -lah doc/html

run:
	echo;echo "Hello:"
	./$(BIN)

