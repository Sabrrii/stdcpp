/**
 * \mainpage Welcome on the graph function file 
 * \brief minimal program for print graph 
 * \author Sebastien COUDERT and Sabri RABAHIA
 * 
**/

#include <string>         // std::string
#include <iostream>       // std::cout
#include "../CImg/CImg.h"  // CImg
using namespace cimg_library;


//! Create a baseline on the graph 
/**
 * \param img : [out] 
 * \param baseline : [in]
 * **/
void setBaseline(CImg<unsigned int> &img,float baseline){
	img.fill(baseline);
}//setBaseline

//!Create a graph with  an amplitude 
/**
 * \param img : [out] 
 * \param baseline : [in]
 * \param amplitude : [in]
 * \param nb_tB : [in] time before add the amplitude 
 * \param nb_tA : [in] duration of the amplitude 
 * 
 *  The amplitude is add to the baseline during all the time of nb_tA.
 * **/
void funcRect(CImg<unsigned int> &image,float baseline,float amplitude,int nb_tB,int nb_tA){
	//! 1. Draw the baseline
	setBaseline(image,baseline);
	if(amplitude !=0){
		//! 2. Duration of the amplitude 
		for( int i=0; i<nb_tA; i++){//duration of the amplitude 
			image(nb_tB+i)=baseline+amplitude;
		}//for (duration of amplitude)
	}//ifamplitude
}//funcRect

//! Create a graph with increasing amplitude 
/**
 * \param img : [out] 
 * \param baseline : [in]
 * \param amplitude : [in]
 * \param nb_tB : [in] time before add the amplitude 
 * \param nb_tA : [in] duration of the amplitude 
 * \param downRate : [in] step to decrease the slope
 * 
 *  The amplitude is increase progressivily during the increase time(nb_tA) and decrease with the downRate choose by the user 
 * **/  
void funcTri(CImg<unsigned int> &image,float baseline,float amplitude,int nb_tB,int nb_tA,float downRate){
	const float climbRate = amplitude/nb_tA;//give the size of the increase step
	const float deltaX = amplitude/downRate; //give the size of the slope
	float hill= baseline+climbRate;//first step of the rise
	//! 1. Draw the baseline
	setBaseline(image,baseline);
	
	if(amplitude !=0){
		//! 2. Duration of the amplitude 
		for( int i=0; i<nb_tA; i++){//duration of the amplitude 
			image(nb_tB+i)=hill;
			//! 3. Increase the rise of the amplitude
			hill += climbRate;//incrementing the rise
		}//for (duration of amplitude)
		//! 4. Start the descent of the amplitude 
		for(int i=nb_tB+nb_tA; i <nb_tB+nb_tA+deltaX; i++){//duration of the descent
			image(i)= hill;
			//! 5. decrease the amplitude with the step 
			hill -= downRate;//decrement with the descent step 
		}//for(duration of the descent)
    }//if (amplitude)
}//funcTri 
  
  
  
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
 
  const int graph = cimg_option("-g",1," Choose you're graph : 1:constant, 2:rect, 3:tri");
  const int width = cimg_option("-n", 1000,"Width of image");
  const float baseline = cimg_option("-b", 10.0,"Baseline of the graph");
  const float amplitude = cimg_option("-a", 100.0,"Amplitude to add at the baseline");
  const int nb_tB = cimg_option("-tb", 100,"Time before adding the amplitude to the baseline");
  const int nb_tA = cimg_option("-ta", 100,"Duration of the baseline increase");
  const float downRate = cimg_option("-rate", 0.5,"Step for the slope");

  
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
  
  //!Choose the graph to draw
  switch (graph)  { //choose the right graph 
    case 1:
        setBaseline(image,baseline);
        image.print("image");
        #if cimg_display!=0
		if(show) image.display_graph("Constant");
		#endif
        break;

    case 2:
        funcRect(image,baseline,amplitude,nb_tB,nb_tA);
        image.print("image");
		#if cimg_display!=0
		if(show) image.display_graph("Signal Rect");
		#endif
        break;
    case 3:
		funcTri(image,baseline,amplitude,nb_tB,nb_tA,downRate);
		image.print("image");
		#if cimg_display!=0
		if(show) image.display_graph("Signal Tri");
		#endif
        break;
    
	}//switch

  if(file_o) image.save(file_o);

  return 0;
}//main

