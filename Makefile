all:
	g++ main.cpp -o hello_OpenMP -fopenmp && ./hello_OpenMP --help > hello_OpenMP.help.output && ./hello_OpenMP --version > VERSION
	./hello_OpenMP --help && ./hello_OpenMP && ./hello_OpenMP -s 'Hello_OpenMP_thread#' -n 3
