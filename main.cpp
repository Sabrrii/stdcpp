/**
 * \mainpage Welcome on the graph function file 
 * \brief class programm with factory to print graph
 * \author Sebastien COUDERT and Sabri RABAHIA
 * 
**/

#include <string>         // std::string
#include <iostream>       // std::cout
#include "../CImg/CImg.h"  // CImg

#include <netcdfcpp.h>
#include "struct_parameter_NetCDF.h"
using namespace cimg_library;


//! Virtual class for all graph
/**
 * \param image : [out]
 * \param width : [in] 
 * \param baseline : [in]
 * **/
template<class T>
class Signal{
	public:
		CImg<T> image;
		T baseline;
		virtual void setImage(int width){ image.assign(width);}
		Signal(){}
		virtual void fillBaseline(float baseline) {image.fill(baseline);}
		virtual void display(std::string title="Signal"){
			image.print("image");
			image.display_graph(title.c_str());
		}
		virtual void setSignal()=0;
	
};//class Signal

//!Create a Constant signal
/**
 * Create a graph with a constant signal
 * **/
template<class T>
class Constant : public Signal<T>{
	public:
		//!Create a constant graph
		/**
		 * \param width : [in] 
		 * \param baseline : [in]
		 * 
		 * Basic graph with a flat baseline
		 * **/
		Constant(float width,T baseline){
			this->setImage(width);
			this->baseline=baseline;
		}
		void setSignal(){
			this->fillBaseline(this->baseline);//call the function from Signal class
		}
};//class Constant
//!Create Rectangle signal
/**
 *  Create a graph with a rectangle signal 
 * **/
template<class T>
class Rect : public Constant<T>{
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
		Rect(float width,T baseline,T amplitude,int nb_tB,int nb_tA): Constant<T>(width,baseline){
			this->amplitude=amplitude;
			this->nb_tB=nb_tB;
			this->nb_tA=nb_tA;
		}
		void setSignal(){
			this->fillBaseline(this->baseline);//call the function from Signal class
			for( int i=0; i<nb_tA; i++){//duration of the amplitude 
				this->image(nb_tB+i)=this->baseline+this->amplitude;
			}//for (duration of amplitude)
		}
	protected:
		T amplitude;
		int nb_tB;
		int nb_tA;
};//class Rect

//!Create triangle signal
/**
 *  Create a graph with a triangle signal 
 * **/
template<class T> 
class Tri : public Rect<T>{
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
		Tri(float width,T baseline,T amplitude,int nb_tB,int nb_tA,float downRate) : Rect<T>(width,baseline,amplitude,nb_tB,nb_tA){
			this->downRate=downRate;
		}
		void setSignal(){
			const float climbRate = this->amplitude/this->nb_tA;//give the size of the increase step
			const float deltaX = this->amplitude/downRate; //give the size of the slope
			float hill= this->baseline+climbRate;//first step of the rise
			const float downHillStep = this->nb_tB+this->nb_tA+deltaX; 
			
			cimg_for_inX(this->image,0,this->nb_tB,i){this->image(i)=this->baseline;}
			cimg_for_inX(this->image,this->nb_tB,this->nb_tB+this->nb_tA,i){
				this->image(i)=hill;
				hill += climbRate;
			}
			cimg_for_inX(this->image,this->nb_tB+this->nb_tA,downHillStep,i){
				this->image(i)= hill;
				hill -= downRate;
			}
			cimg_for_inX(this->image,downHillStep,this->image.width()-1,i){this->image(i)=this->baseline;}
			

		/* Loop without cimg for 
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
template<class T>
class Pac : public Constant<T>{
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
		Pac(int width,T baseline,T amplitude,int nb_tB,float rate1,float rate2) : Constant<T>(width,baseline){
			this->amplitude=amplitude;
			this->nb_tB=nb_tB;
			this->rate1=rate1;
			this->rate2=rate2;
		}
		//! \todo COMMENT
		void setSignal(){
			this->fillBaseline(this->baseline);//call the function from Signal class
			double t = 0;
			for( int i=this->nb_tB; i<this->image.width(); i++){
				this->image(i)=this->amplitude*(exp(-t/rate1)-exp(-t/rate2))+this->baseline;
				t++;
			}
			//As the gap beetwen rate is bigger, the curve will be pack
		}
		
	private:
		T amplitude;
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
template<class T>
class SignalFactory{
	public:
		static Signal<T> *NewSignal(char type,float width,T baseline,T amplitude, int nb_tB,int nb_tA,float downRate,float rate1,float rate2){
			switch(type){//Selection of the curve
				case 1 :
				return new Constant<T>(width,baseline);
				case 2 :
				return new Rect<T>(width,baseline,amplitude,nb_tB,nb_tA);
				case 3:
				return new Tri<T>(width,baseline,amplitude,nb_tB,nb_tA,downRate);
				case 4:
				return new Pac<T>(width,baseline,amplitude,nb_tB,rate1,rate2);
				default:
				return NULL;
			}
		}
};//Class SignalFactory

//!Read into a binary file 
/**
 * \param baseline : [in]
 * \param amplitude : [in]
 * \param nb_tB : [in] time before add the amplitude 
 * \param nb_tA : [in] duration of the amplitude 
 * \param i : [in] file name to read 
 * **/
int readFile(float  &amplitude, float &baseline, int &nb_tB, int &nb_tA, std::string i){
  std::string fi = i;//file parameters.nc
  ///parameter class
  CParameterNetCDF fp;
  //open file
  int error=fp.loadFile((char *)fi.c_str());
  if(error){std::cerr<<"loadFile return "<< error <<std::endl;return error;}
  
  ///Rectangle
    int process; std::string process_name="rectangle";
    //load process variable
    error = fp.loadVar(process,&process_name);
    if(error){std::cerr<<"loadVar return" <<error<<std::endl;return error;}
    std::cout<<process_name<<"="<<process<<std::endl;
    
    ///amplitude
    std::string attribute_name = "amplitude";
    if (error = fp.loadAttribute(attribute_name,amplitude)!=0){
   	 std::cerr<<"Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    }
std::cout<<attribute_name<<"="<<amplitude<<std::endl;    

    ///baseline
    attribute_name = "baseline";
    if (error = fp.loadAttribute(attribute_name,baseline)!=0){
   	 std::cerr<<"Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    }
std::cout<<attribute_name<<"="<<baseline<<std::endl;

    ///nb_tB
    attribute_name = "nb_tB";
    if (error =fp.loadAttribute(attribute_name,nb_tB)!=0){
   	 std::cerr<<"Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    }    
std::cout<<attribute_name<<"="<<nb_tB<<std::endl;

    ///nb_tA
    attribute_name = "nb_tA";
    if (error =fp.loadAttribute(attribute_name,nb_tA)!=0){
   	 std::cerr<<"Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    }
std::cout<<attribute_name<<"="<<nb_tA<<std::endl;

return 0;
};

//!Merge graph into one display
/**
 * \param signal : [in] signal to merge
 * \param sig : [in] signal to merge 
 * \param visu : [out] CImg with both merged graph 
 * **/
int collateGraph(Signal<int> *signal,Signal<float> *sig){
	CImg <float> visu(signal->image.width(),1,1,2);
	visu.get_shared_channel(0)=signal->image;
	visu.get_shared_channel(1)=sig->image;
	visu.display_graph("Merged graph");
	return 0;
}


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
  float baseline = cimg_option("-b", 10.0,"Baseline of the graph");
  float amplitude = cimg_option("-a", 100.0,"Amplitude to add at the baseline");
  int nb_tB = cimg_option("-tb", 100,"Time before adding the amplitude to the baseline");
  int nb_tA = cimg_option("-ta", 100,"Duration of the baseline increase");
  const float downRate = cimg_option("-rate", 0.5,"Step for the slope in case of signal 3:tri");
  const float rate1 = cimg_option("-r1",170.0,"rate1 in case of signal 4: pac");
  const float rate2 = cimg_option("-r2",100.0,"rate2 in case of signal 4: pac");
  const std::string i = cimg_option("-i", "","file name  to read (parameters.nc)");
  
   if(show_help) {/*print_helps(std::cerr);*/return 0;}
  //}CLI option

  ///Check if the option is empty or not 
  if(i!=""){
	readFile(amplitude,baseline,nb_tB,nb_tA,i);
	Signal<int> *signal = SignalFactory<int>::NewSignal(2,width,baseline,amplitude,nb_tB,nb_tA,downRate,rate1,rate2);
	signal->setSignal();
	
	#if cimg_display!=0
    if(show) signal->display();
	#endif
	
	return 1;
  }
  
  
	Signal<int> *signal = SignalFactory<int>::NewSignal(type,width,baseline,amplitude,nb_tB,nb_tA,downRate,rate1,rate2);
	Signal<float> *signalU = SignalFactory<float>::NewSignal(type,width,baseline,amplitude,nb_tB,nb_tA,downRate,rate1,rate2);
	if(signal ==NULL){std::cerr<<"Error type of signal doesn't exist"<<std::endl; return 1;}
	if(signalU ==NULL){std::cerr<<"Error type of signal doesn't exist"<<std::endl; return 1;}
	
	signal->setSignal();
    signalU->setSignal();
	
	collateGraph(signal,signalU);
	

///if want both graph in one display comment the code below so the graph's windows doesn't pop when you close the collate graph window
   #if cimg_display!=0
   if(show) signal->display();
   if(show) signalU->display("signal Float");
   #endif
  
  //if(file_o) signal->image.save(file_o);

  return 0;
}//main

