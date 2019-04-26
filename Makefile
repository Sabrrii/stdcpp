all:
	g++ main.cpp -o hello_OpenMP -fopenmp
	./hello_OpenMP --help && ./hello_OpenMP && ./hello_OpenMP -s 'Hello_OpenMP#'
