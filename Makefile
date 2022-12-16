all: bin doc
	

bin: main.cpp
	g++ main.cpp -o factory
	./factory --help

doc: main.cpp
	doxygen
	echo "firefox doc/html/index.html &"

run: bin
	./factory -d laptop
