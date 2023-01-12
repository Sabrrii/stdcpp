/**
 * \mainpage Welcome on the graph function file 
 * \brief class programm with factory to print graph
 * \author Sebastien COUDERT and Sabri RABAHIA
 * 
**/

#include <string>         // std::string
#include <iostream>       // std::cout
//#include "../CImg/CImg.h"  // CImg

//#include <netcdfcpp.h>
#include "struct_parameter_NetCDF.h"
//#include "../CImg.Tool/CImg_NetCDF.h"
#include "../RockAMali/CDataGenerator_PAC.hpp"

#include <omp.h>
#include "../RockAMali/CDataGenerator_factory.hpp"
#include "../RockAMali/SDataTypes.hpp"

#include <list>
#include <vector>

using namespace std;
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
		//! Assign a width to the image
		/** 
		 * \param width : [in]
		 * **/
		virtual void setImage(int width){ image.assign(width);}
		Signal(){}
		//!Fill the graph with baseline
		/**
		 * \param baseline : [in]
		 * **/
		virtual void fillBaseline(float baseline) {image.fill(baseline);}
		//!Create the display to see the graph 
		/**
		 * \param title : [in]
		 * **/
		virtual void display(std::string title="Signal"){
			image.print("image");
			image.display_graph(title.c_str());
		}
		//! Virtual function to create the signal
		virtual void setSignal()=0;
	
};//class Signal

//!Create a Constant signal
/**
 * Create a graph with a constant signal
 * **/
template<class T>
class Constant : public Signal<T>{
	public:
		//!Constructor for constant signal 
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
		//! Create the constant signal 
		/**
		 * Mandatory function from the inheritance
		 * **/
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
		//!Constructor for  a graph with  an amplitude 
		/**
		 * \param image : [out] 
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
		//! Create the Rectangle signal 
		/**
		 * Mandatory function from the inheritance
		 * **/
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
		//! Constructor for  a graph with increasing amplitude 
		/**
		 * \param image : [out] 
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
		
		//! Create the Triangle signal 
		/**
		 * Mandatory function from the inheritance
		 * **/
		void setSignal(){
			const float climbRate = this->amplitude/this->nb_tA;//give the size of the increase step
			const float deltaX = this->amplitude/downRate; //give the size of the slope
			float hill= this->baseline+climbRate;//first step of the rise
			const float downHillStep = this->nb_tB+this->nb_tA+deltaX; 
			
			//! 1. Create the baseline on graph
			cimg_for_inX(this->image,0,this->nb_tB,i){this->image(i)=this->baseline;}
			//! 2. Increase the rise of the amplitude
			cimg_for_inX(this->image,this->nb_tB,this->nb_tB+this->nb_tA,i){
				this->image(i)=hill;
				hill += climbRate;//incrementing the rise
			}
			//! 3. Start the descent of the amplitude
			cimg_for_inX(this->image,this->nb_tB+this->nb_tA,downHillStep,i){
				this->image(i)= hill;
				hill -= downRate;//decrement with the descent step 
			}
			//! 4. Draw the baseline 
			cimg_for_inX(this->image,downHillStep,this->image.width()-1,i){this->image(i)=this->baseline;}

		}
	private:	
		float downRate;

};//class Tri

//! Create a graph with a double exponential  signal 
/**
 *   Create a pac curve signal 
 * **/
template<class T>
class Pac : public Constant<T>{
	public :
		//! Constructor for the double exponential  signal
		/**
		 * \param image : [out] 
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
		//! Create the double exponential  signal
		/**
		 * Mandatory function from the inheritance
		 * **/
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
 * \param image : [out] 
 * \param width : [in]
 * \param baseline : [in]
 * \param amplitude : [in]
 * \param nb_tB : [in] time before add the amplitude 
 * \param nb_tA : [in] duration of the amplitude 
 * \param downRate : [in] step to decrease the slope
 * \param rate1 : [in]
 * \param rate2 : [in]
 * 
 *  To choose witch signal create 
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
 * 
 * Permit to use the data variable from the file 
 * **/
int readFile(float  &amplitude, float &baseline, int &nb_tB, int &nb_tA, std::string i){
  std::string fi = i;//file parameters.nc
  ///parameter class
  CParameterNetCDF fp;
  //open file
  int error=fp.loadFile((char *)fi.c_str());
  if(error){std::cerr<<"loadFile return "<< error <<std::endl;return error;}
  
  ///Rectangle signal
    int process; std::string process_name="rectangle";
    //load process variable
    error = fp.loadVar(process,&process_name);
    if(error){std::cerr<<"loadVar return" <<error<<std::endl;return error;}
    std::cout<<process_name<<"="<<process<<std::endl;
    
    ///amplitude
    std::string attribute_name = "amplitude";
    if ((error = fp.loadAttribute(attribute_name,amplitude))!=0){
   	 std::cerr<<"Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    }
std::cout<<attribute_name<<"="<<amplitude<<std::endl;    

    ///baseline
    attribute_name = "baseline";
    if ((error = fp.loadAttribute(attribute_name,baseline))!=0){
   	 std::cerr<<"Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    }
std::cout<<attribute_name<<"="<<baseline<<std::endl;

    ///nb_tB
    attribute_name = "nb_tB";
    if ((error =fp.loadAttribute(attribute_name,nb_tB))!=0){
   	 std::cerr<<"Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    }    
std::cout<<attribute_name<<"="<<nb_tB<<std::endl;

    ///nb_tA
    attribute_name = "nb_tA";
    if ((error =fp.loadAttribute(attribute_name,nb_tA))!=0){
   	 std::cerr<<"Error while loading "<<process_name<<":"<<attribute_name<<" attribute"<<std::endl;
    }
std::cout<<attribute_name<<"="<<nb_tA<<std::endl;

return 0;
};

//!Save the image into the saveImg.nc file
/**
 * \param signal : [in]
 * **/
int writeDataFile(Signal<int> *signal){
//dimension names
  vector<string> dim_names;
  string dim_time="dimF";
  dim_names.push_back("dimS");

//variable names (and its unit)
///single
  string var_name="signal";
  string unit_name="none";


/*CImg test*/
  string/**/ fo="saveImg.nc";
  typedef int Tdata;//char, int, float, double, unsigned char (byte) ok
  CImgNetCDF<Tdata> cimgSave;
  cout << "CImgNetCDF::saveNetCDFFile(" << fo << ",...) return " << cimgSave.saveNetCDFFile((char*)fo.c_str()) << endl;
  cout << "CImgNetCDF::addNetCDFDims(" << fo << ",...) return " << cimgSave.addNetCDFDims(signal->image,dim_names,dim_time) << endl;
  cout << "CImgNetCDF::addNetCDFVar(" << fo << ",...) return " << cimgSave.addNetCDFVar(signal->image,var_name,unit_name) << endl;
  //add attributes
  if (!(cimgSave.pNCvar->add_att("long_name","fake test signal, e.g. data (lib/class CImgNetCDF)"))) std::cerr<<"error: while adding attribute long name (NC_ERROR)."<<std::endl;
	cout << "CImgNetCDF::addNetCDFData(" << fo << ",...) return " << cimgSave.addNetCDFData(signal->image) << endl;
	
  //add global attributes
  cimgSave.pNCFile->add_att("library","CImg_NetCDF");
  cimgSave.pNCFile->add_att("library_version",CIMG_NETCDF_VERSION);
  cout << endl;
/**/

   cout << "*** SUCCESS writing example file " << fo << "!" << endl;
   return 0;
}


//! Initialisation of the file for NetCDF save
/**
 * \param cimgSave : [in]  object for NetCDF save
 * \param listImgSign : [in] list of image
 * \param fileName : [in] where save the image
 * **/
int InitListFile(CImgListNetCDF<float> *cimgSave, CImgList<float> listImgSign, string fileName){
//dimension names
  vector<string> dim_names;
  string dim_time="dimF";
  dim_names.push_back("dimS");
    
  //variable names
  vector<string> var_names;
  vector<string> unit_names;
  
  for(int i=0;i<listImgSign.size();i++){
	char str[10] = "signal";
	char t[4];
	sprintf(t,"%d",i);
	strcat(str,t);
	var_names.push_back(str);
	unit_names.push_back("pixel");
  }

  cimgSave->saveNetCDFFile((char*)fileName.c_str());
  cimgSave->addNetCDFDims(listImgSign,dim_names,dim_time);
  cimgSave->addNetCDFVar(listImgSign,var_names,unit_names);
  
  return 0;
}


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
  const int ns = cimg_option("-ns",1,"number of duplication of the signal ");
  const std::string generator_type=cimg_option("--gen_factory","count","gen a peak");
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
  /*
	Signal<int> *signalInt = SignalFactory<int>::NewSignal(type,width,baseline,amplitude,nb_tB,nb_tA,downRate,rate1,rate2);
	Signal<float> *signalFloat = SignalFactory<float>::NewSignal(type,width,baseline,amplitude,nb_tB,nb_tA,downRate,rate1,rate2);
	if(signalInt ==NULL){std::cerr<<"Error type of signal doesn't exist"<<std::endl; return 1;}
	if(signalFloat ==NULL){std::cerr<<"Error type of signal doesn't exist"<<std::endl; return 1;}

	signalInt->setSignal();
    signalFloat->setSignal();
      
    #if cimg_display!=0
    collateGraph(signalInt,signalFloat);
    #endif
    
    //Init the file to save 
    CImgList<float> listImgSig (signalFloat->image,signalInt->image);
    CImgListNetCDF<float> cimgSave;
	InitListFile(&cimgSave,listImgSig,"saveListImg.nc");
	
	
	CImg <float> visu(signalFloat->image.width(),1,1,ns);
	//Duplicate signal for saving and display them 	
	for(int i=0;i<ns;++i){
		amplitude += i*100;
		//Signal 0 with ncview
		signalFloat = SignalFactory<float>::NewSignal(type,width,baseline,amplitude,nb_tB,nb_tA,downRate,rate1,rate2);
		signalFloat->setSignal();
		
		//Signal 1 with ncview
		signalInt =  SignalFactory<int>::NewSignal(type,width,baseline,amplitude,nb_tB,nb_tA,downRate,rate1,rate2);
		signalInt->setSignal();
		
		//add the the signal into the display
		visu.get_shared_channel(i)=signalFloat->image;
		
		listImgSig = {signalFloat->image,signalInt->image};
		cimgSave.addNetCDFData(listImgSig );
		
	}
	//print the dislplay with all the signals
	visu.display_graph("Merged graph");*/
  
    
    std::vector<std::string> generator_type_list;CDataGenerator_factory<Tdata, Taccess>::show_factory_types(generator_type_list);std::cout<<std::endl;

	int nbuffer = 1;
  //OpenMP locks
  omp_lock_t print_lock;omp_init_lock(&print_lock);
  
    //! circular buffer
  CImgList<Tdata> images(nbuffer,width,1,1,1);
  images[0].fill(0);
  images[0].print("image",false);
  //access locking
  omp_lock_t lck;omp_init_lock(&lck);
  
   //! thread locks
  std::vector<omp_lock_t*> locks;locks.push_back(&print_lock);locks.push_back(&lck);
  
    //! access and status of buffer
  CImg<Taccess> access(nbuffer,1,1,1);
  access.fill(0);//free
  access.print("access (free state)",false);fflush(stderr);
  
  std::cout << "test1" <<std::endl;
	CDataGenerator<Tdata, Taccess> *generate=CDataGenerator_factory<Tdata, Taccess>::NewCDataGenerator 
      (generator_type, generator_type_list, locks);
       std::cout << "test2" <<std::endl;  
    for (unsigned int i=0;i<ns;++i){
		generate->iteration(access,images ,0,i);
		images[0].display_graph();
	}
  

  return 0;
}//main

