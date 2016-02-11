BIN=hello_cimg
version=v0.0.4

VERSION=-DVERSION=\"$(version)\"
CIMG_LIB=-I../CImg -Wall -W -lm -ansi -pedantic -O3 -fno-tree-pre -Dcimg_use_vt100 -lpthread
CIMG_LIBX=-I/usr/X11R6/include -Dcimg_use_xshm -Dcimg_use_xrandr -L/usr/X11R6/lib -lX11 -lXext -lXrandr

all: bin doc run

version:
	echo ${version} > VERSION

#no display
bin: version
	$(CXX) -Dcimg_display=0 $(VERSION) $(CIMG_LIB) main.cpp -o $(BIN) && ./$(BIN) --help
	./$(BIN) --help > $(BIN)_help.output && ./$(BIN) --version > VERSION

#display
binX: version
	$(CXX) main.cpp $(VERSION) $(CIMG_LIB) $(CIMG_LIBX) -o $(BIN)X && ./$(BIN)X --help
	./$(BIN)X --help > $(BIN)X_help.output && ./$(BIN)X --version > VERSION

doc:
	doxygen && ls -lah doc/html

run:
	echo;echo "Hello:"
	./$(BIN) -o text.pnm
	identify text.pnm
