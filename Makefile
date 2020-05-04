#main binary name should be consistent with CI, e.g. name of the branch (see .gitlab-ci.yml)
BIN=hello_openmp

all: bin doc run

bin:
	$(CXX) main.cpp -o $(BIN) -fopenmp && ./$(BIN) --help
	./$(BIN) --help > $(BIN)_help.output && ./$(BIN) --version > VERSION

doc:
	doxygen && ls -lah doc/html

run:
	echo;echo "hello_OpenMP:"
	./$(BIN) -s 'Hello_OpenMP_thread#' -n 3

