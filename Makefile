all: bin doc
	

bin: main.cpp
	g++ main.cpp -o factory

doc: main.cpp
	doxygen
	echo "firefox doc/html/index.html &"
