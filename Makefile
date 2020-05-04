all: bin doc run

bin:
	$(CXX) main.cpp -o ArgParse && ./ArgParse --help
	./ArgParse --help > ArgParse_help.output

doc:
	doxygen && ls -lah doc/html

run:
	echo;echo "ArgParse:"
	./ArgParse -v -i 12 -s XYZ


