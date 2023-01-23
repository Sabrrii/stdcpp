BIN=hello_cimg
version=v0.0.6

VERSION=-DVERSION=\"$(version)\"
CIMG_LIB=-I../CImg -Wall -W -lm -ansi -pedantic -O3 -fno-tree-pre -Dcimg_use_vt100 -lpthread
CIMG_LIBX=-I/usr/X11R6/include -Dcimg_use_xshm -Dcimg_use_xrandr -L/usr/X11R6/lib -lX11 -lXext -lXrandr
LIB_NETCDF= -I../NetCDF.Tool  -I../CImg.Tool -I/usr/include/ -lnetcdf_c++ -L/usr/lib/aarch64-linux-gnu/ -lnetcdf


##do compile
DO_NETCDF=-DDO_NETCDF $(LIB_NETCDF) -DARCH=$(ARCH)
#DO_NETCDF=
DO_BLOCK=-DDO_BLOCK
#DO_BLOCK=

all: bin doc run

version:
	echo ${version} > VERSION

#no display
bin: version
	$(CXX) -Dcimg_display=0 $(VERSION) -fopenmp $(CIMG_LIB)  main.cpp  $(DO_NETCDF) -o $(BIN) && ./$(BIN) --help
	./$(BIN) --help > $(BIN)_help.output && ./$(BIN) --version > VERSION

#display
binX: version
	$(CXX) main.cpp $(VERSION) -fopenmp $(CIMG_LIB) $(CIMG_LIBX) $(DO_NETCDF) $(DO_BLOCK) -o $(BIN)X && ./$(BIN)X --help
	./$(BIN)X --help > $(BIN)X_help.output && ./$(BIN)X --version > VERSION

doc: main.cpp
	doxygen && ls -lah doc/html

run:
	echo;echo "Hello:"
	./$(BIN) -o text.pnm
	identify text.pnm #imagemagick

runX:
	echo;echo "HelloX:"
	./$(BIN)X -o text.pnm
