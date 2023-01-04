/**
 * \mainpage Welcome on the graph function file 
 * \brief class programm with factory to print graph
 * \author Sebastien COUDERT and Sabri RABAHIA
 * 
**/

#include <string>         // std::string
#include <iostream>       // std::cout
#include "../CImg/CImg.h"  // CImg
using namespace cimg_library;


//! Virtual class for all graph
/**
 * \param image : [out]
 * \param width : [in] 
 * \param baseline : [in]
 * **/
/*template<typename T>*/
class Signal{
	protected:
		CImg</*T*/unsigned int> image;
		/*T*/float baseline;
		virtual void setImage(int width){ image.assign(width);}
	public:
		Signal(){}
		virtual void fillBaseline(float baseline) {image.fill(baseline);}
		virtual void display(){
			image.print("image");
			image.display_graph("Signal");
		}
		virtual void setSignal()=0;
	
};//class Signal

//!Create a Constant signal
/**
 * Create a graph with a constant signal
 * **/
class Constant : public Signal{
	public:
		//!Create a constant graph
	/**
	 * \param width : [in] 
	 * \param baseline : [in]
	 * 
	 * Basic graph with a flat baseline
	 * **/
		Constant(float width,float baseline){
			setImage(width);
			this->baseline=baseline;
		}
		void setSignal(){
			fillBaseline(baseline);//call the function from Signal class
		}
};//class Constant
//!Create Rectangle signal
/**
 *  Create a graph with a rectangle signal 
 * **/
class Rect : public Constant{
	public:
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
		Rect(float width,float baseline,float amplitude,int nb_tB,int nb_tA): Constant(width,baseline){
			this->amplitude=amplitude;
			this->nb_tB=nb_tB;
			this->nb_tA=nb_tA;
		}
		void setSignal(){
			fillBaseline(baseline);//call the function from Signal class
			for( int i=0; i<nb_tA; i++){//duration of the amplitude 
				image(nb_tB+i)=baseline+amplitude;
			}//for (duration of amplitude)
		}
	protected:
		float amplitude;
		int nb_tB;
		int nb_tA;
};//class Rect

//!Create triangle signal
/**
 *  Create a graph with a triangle signal 
 * **/
class Tri : public Rect{
	public :
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
		Tri(float width,float baseline,float amplitude,int nb_tB,int nb_tA,float downRate) : Rect(width,baseline,amplitude,nb_tB,nb_tA){
			this->downRate=downRate;
		}
		void setSignal(){
			const float climbRate = amplitude/nb_tA;//give the size of the increase step
			const float deltaX = amplitude/downRate; //give the size of the slope
			float hill= baseline+climbRate;//first step of the rise
			const int downHillStep = nb_tB+nb_tA+deltaX; 
			
			cimg_for_inX(image,0,nb_tB,z){image(z)=baseline;}
			cimg_for_inX(image,nb_tB,nb_tB+nb_tA,y){
				image(y)=hill;
				hill += climbRate;
			}
			cimg_for_inX(image,nb_tB+nb_tA,downHillStep,x){
				image(x)= hill;
				hill -= downRate;
			}
			cimg_for_inX(image,downHillStep,image.width()-1,w){image(w)=baseline;}
			

		/*	Loop without cimg for 
		 * //! 1. Crteate the rbaseline on graph
			fillBaseline(baseline);//call the function from Signal class
			
			for( int i=nb_tB; i<nb_tB+nb_tA; i++){//duration of the amplitude 
				image(i)=hill;
				//! 2. Increase the rise of the amplitude
				hill += climbRate;//incrementing the rise
			}//for (duration of amplitude)

			//! 3. Start the descent of the amplitude
			for(int i=nb_tB+nb_tA; i <downHillStep; i++){//duration of the descent
				image(i)= hill;
				//! 4. decrease the amplitude with the step 
				hill -= downRate;//decrement with the descent step 
			}//for(duration of the descent)
*/

		}
	private:	
		float downRate;

};//class Tri

//! Create a graph with a Signal 
/**
 *   Create a pac curve signal 
 * **/
class Pac : public Constant{
	public :
		//! Create a graph with a Signal 
		/**
		 * \param img : [out] 
		 * \param width : [in]
		 * \param baseline : [in]
		 * \param amplitude : [in]
		 * \param nb_tB : [in] time before add the amplitude 
		 * \param rate1 : [in]
		 * \param rate2 : [in]
		 * 
		 *   Create a pac curve signal 
		 * **/
		Pac(int width,float baseline,float amplitude,int nb_tB,float rate1,float rate2) : Constant(width,baseline){
			this->amplitude=amplitude;
			this->nb_tB=nb_tB;
			this->rate1=rate1;
			this->rate2=rate2;
		}
		//! \todo COMMENT
		void setSignal(){
			fillBaseline(baseline);//call the function from Signal class
			double t = 0;
			for( int i=nb_tB; i<image.width(); i++){
				this->image(i)=amplitude*(exp(-t/rate1)-exp(-t/rate2))+baseline;
				t++;
			}
			//As the gap beetwen rate is bigger, the curve will be pack
		}
		
	private:
		float amplitude;
		int nb_tB;	
		double rate1;
		double rate2;

};//class Pac

//!Signal Factory 
/**
 * \param img : [out] 
 * \param width : [in]
 * \param baseline : [in]
 * \param amplitude : [in]
 * \param nb_tB : [in] time before add the amplitude 
 * \param nb_tA : [in] duration of the amplitude 
 * \param downRate : [in] step to decrease the slope
 * \param rate1 : [in]
 * \param rate2 : [in]
 * 
 *  To choose witch signal to create 
 * **/
class SignalFactory{
	public:
		static Signal *NewSignal(char type,float width,float baseline,float amplitude, int nb_tB,int nb_tA,float downRate,float rate1,float rate2){
			switch(type){//Selection of the curve
				case 1 :
				return new Constant(width,baseline);
				case 2 :
				return new Rect(width,baseline,amplitude,nb_tB,nb_tA);
				case 3:
				return new Tri(width,baseline,amplitude,nb_tB,nb_tA,downRate);
				case 4:
				return new Pac(width,baseline,amplitude,nb_tB,rate1,rate2);
				default:
				return NULL;
			}
		}
};//Class SignalFactory

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
 
  const char type = cimg_option("-type",4,"type of the signal 1:constant, 2:rect, 3:tri, 4:pac");
  const int width = cimg_option("-n", 1000,"Width of image");
  const float baseline = cimg_option("-b", 10.0,"Baseline of the graph");
  const float amplitude = cimg_option("-a", 100.0,"Amplitude to add at the baseline");
  const int nb_tB = cimg_option("-tb", 100,"Time before adding the amplitude to the baseline");
  const int nb_tA = cimg_option("-ta", 100,"Duration of the baseline increase");
  const float downRate = cimg_option("-rate", 0.5,"Step for the slope in case of signal 3:tri");
  const float rate1 = cimg_option("-r1",170,"rate1 in case of signal 4: pac");
  const float rate2 = cimg_option("-r2",100,"rate2 in case of signal 4: pac");

  
   if(show_help) {/*print_helps(std::cerr);*/return 0;}
  //}CLI option

  Signal/*<float>*/* signal = SignalFactory::NewSignal(type,width,baseline,amplitude,nb_tB,nb_tA,downRate,rate1,rate2);
  /*
  Signal<int> signalU = SignalFactory::NewSignal(type,width,baseline,amplitude,nb_tB,nb_tA,downRate,rate1,rate2);
  */
  
  if(signal ==NULL){std::cerr<<"Error type of signal doesn't exist"<<std::endl; return 1;}
  signal->setSignal();
  
  #if cimg_display!=0
  if(show) signal->display();
  #endif
  
  //if(file_o) signal->image.save(file_o);

  return 0;
}//main

