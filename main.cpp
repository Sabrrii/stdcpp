/**
 * \brief minimal Hello CImg program (doxygen syntax)
 * \author Sebastien COUDERT
**/

#include <string>         // std::string
#include <iostream>       // std::cout
#include "../CImg/CImg.h"  // CImg
using namespace cimg_library;

//! hello starts here
/**
 * \warning \c argc and \c argv not used yet, see argp branch (e.g. git checkout argp)
**/
int main(int argc, char **argv)
{
  ///command arguments, i.e. CLI option
  cimg_usage(std::string("Hello to CImg.\n" \
  " It uses different GNU libraries (see --info option)\n\n" \
  " usage: ./hello_cimg  -h -I\n" \
  "        ./hello_cimg  -o hello_cimg.png\n" \
  "        ./hello_cimgX -o hello_cimg.png --show\n" \
  "\n version: "+std::string(VERSION)+"\n compilation date:" \
  ).c_str());//cimg_usage

  ///program options
  const char *file_o    = cimg_option("-o",(char*)0,"output image");

  ///standard options
  #if cimg_display!=0
  const bool show_X=cimg_option("-X",true,NULL);//-X hidden option
  bool show=cimg_option("--show",show_X,"show GUI (or -X option)");show=show_X|show;//same --show or -X option
  #endif
  const bool show_h   =cimg_option("-h",    false,NULL);//-h hidden option
        bool show_help=cimg_option("--help",show_h,"help (or -h option)");show_help=show_h|show_help; //same --help or -h option
  bool show_info=cimg_option("-I",false,NULL);//-I hidden option
  if( cimg_option("--info",show_info,"show compilation options (or -I option)") ) {show_info=true;cimg_library::cimg::info();}//same --info or -I option
  bool show_version=cimg_option("-v",false,NULL);//-v hidden option
  if( cimg_option("--version",show_version,"show version (or -v option)") ) {show_version=true;std::cout<<VERSION<<std::endl;return 0;}//same --version or -v option
 
  
  const int width = cimg_option("-n", 1000,"Width of image");
  const int baseline = cimg_option("-b", 10,"Baseline of the graph");
  const int amplitude = cimg_option("-a", 0,"Amplitude to add at the baseline");
  const int nb_tB = cimg_option("-tb", 100,"Duration before adding the amplitude to the baseline");
  const int nb_tA = cimg_option("-ta", 100,"Duration of the increase of the baseline");
  
   if(show_help) {/*print_help(std::cerr);*/return 0;}
  //}CLI option

  //! a few colors
  const unsigned char
            // R   G   B
    red[]   = {255,  0,  0},
    green[] = {  0,255,  0},
    blue [] = {  0,  0,255},
    black[] = {  0,  0,  0},
    white[] = {255,255,255};

  CImg<unsigned int> image(width);
  image.fill(baseline);
  if(amplitude !=0){
	for( int i=0; i<nb_tA; i++){//duration of the amplitude 
		image.fillY(nb_tB+i,0,0,baseline+amplitude);//fill the graph with the amplitude 
	}//for (duration of amplitude)
  }//if (amplitude)
  
  image.print("image");
  #if cimg_display!=0
  if(show) image.display_graph("Amplitude");
  #endif

  if(file_o) image.save(file_o);

  return 0;
}//main

