/**
 * \file factory.cpp
 * \brief ArgP minimal program (i.e. template)
 * \author Sebastien COUDERT
 * \ref https://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns#Factory
 * usage: g++ -o factory factory.cpp && ./factory -V | tail -n 1 && ./factory -v -d NumExo2_ExoGam && echo && ./factory -v -d NumExo2_NEDA && echo "  ERROR TESTS:" && ./factory -v -d NumExo2_error && echo && ./factory -v -d NumExo

**/

//ArgP
#include <error.h>
#include <argp.h>

#include <string>         // std::string
#include <iostream>       // std::cout

#define VERSION "v0.0.7e"

//Program option/documentation
//{argp
//! [argp] version of program
const char *argp_program_version=VERSION;
//! [argp] author of program
const char *argp_program_bug_address="sebastien.coudert@ganil.fr";
//! [argp] documentation of program
static char doc[]=
   "factory: factory program based on factory design pattern\
\n  factory."VERSION"\
\n\
examples:\n\
  factory --help\n\
  factory -v\n\
  factory -V\n\
  factory -d laptop\n\
  factory --usage";

//! [argp] A description of the arguments
static char args_doc[] = "";

//! [argp] The options and its description
static struct argp_option options[]=
{
  {"verbose",  'v', 0, 0,         "Produce verbose output" },
  {"module",   'd', "module", 0,  "create the module from factory" },
//default options
  { 0 }
};//options (CLI)

//! [argp] Used by main to communicate with parse_option
struct arguments
{
  //! verbose mode
  int verbose;
  //! module value
  std::string module;
};//arguments (CLI)

//! [argp] Parse a single option
static error_t
parse_option(int key, char *arg, struct argp_state *state)
{
  //Get the input argument from argp_parse
  struct arguments *arguments=(struct arguments *)(state->input);
  switch (key)
  {
    case 'v':
      arguments->verbose=1;
      break;
    case 'd':
      arguments->module=arg;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }//switch
  return 0;
}//parse_option


//! [argp] print argument values
void print_args(struct arguments *p_arguments)
{
  printf (".verbose=%s\n.module=\"%s\"\n"
  , p_arguments->verbose?"yes":"no"
  , p_arguments->module.c_str()
  );
}//print_args

//! [argp] setup argp parser
static struct argp argp = { options, parse_option, args_doc, doc };

//}argp


//{factory

class Device
{
 protected:
  std::string name;
  virtual void set_name(std::string device_name) {name=device_name; std::cout<<name<<"/"<<__func__<<"(\""<<name<<"\")"<<std::endl;}
 public:
  //! constructor
  Device() {name="none";}
  virtual void Run() = 0;
  virtual void Stop() = 0;
  //! destructor (need at least empty one)
  virtual ~Device() {}
};//Device
class Virtex6: public Device
{
 public:
  Virtex6() {set_name("Virtex6");}
  virtual void Run()  {std::cout<<name<<" run"<<std::endl; mHibernating = false;}
  virtual void Stop() {mHibernating = true;}
  //! destructor (need at least empty one)
  virtual ~Virtex6() {}
 private:
  bool mHibernating;//Whether or not the machine is hibernating
};//Virtex6
class V6_NEDA: public Virtex6
{
 public:
  V6_NEDA() {set_name("V6_NEDA");}
  //! destructor (need at least empty one)
  virtual ~V6_NEDA() {}
};//V6_NEDA

//! device factory
/**
 *  \note instanciation based on name (i.e. string, as done only one time)
**/
class DeviceFactory
{
  public:
  static Device *NewDevice(const std::string &name)
  {
    if(name == "Virtex6")
      return new Virtex6;
    if(name == "V6_NEDA")
      return new V6_NEDA;
    std::cerr<<"Device name is unknown, i.e. \""<<name<<"\"."<<std::endl;
    return NULL;
  }//NewDevice
};//DeviceFactory



class Computer
{
 protected:
  std::string name;
  virtual void set_name(std::string module_name) {name=module_name; std::cout<<name<<"/"<<__func__<<"(\""<<name<<"\")"<<std::endl;}
 public:
  //! constructor
  Computer() {name="none";}
  virtual void Run() = 0;
  virtual void Stop() = 0;
  //! destructor
  virtual ~Computer() {};//without this, you do not call Laptop or Desktop destructor in this example!
};//Computer
class Laptop: public Computer
{
 public:
  Laptop() {set_name("Laptop");}
  void Run()  {std::cout<<name<<" run"<<std::endl; mHibernating = false;}
  void Stop() {mHibernating = true; }
  virtual ~Laptop() {};//because we have virtual functions, we need virtual destructor
 private:
  bool mHibernating;//Whether or not the machine is hibernating
};//Laptop
class Desktop: public Computer
{
 public:
  Desktop() {set_name("Desktop");}
  void Run()  {std::cout<<name<<" run"<<std::endl;mOn = true; }
  void Stop() {mOn = false;}
  virtual ~Desktop() {}
 private:
  bool mOn;//Whether or not the machine has been turned on
};//Desktop

//! numexo2 module
class NumExo2: public Computer
{
 protected:
  //! Virtex6 device (from factory)
  Device *v6;
 public:
  //! constructor
  //* \note do not instanciate especially v6 */
  NumExo2() {set_name("NumExo2");}
  virtual void create_v6(std::string device_name) {v6=DeviceFactory::NewDevice(device_name);}
  virtual void Run()  {std::cout<<name<<" run"<<std::endl;mOn = true; if(v6!=NULL) v6->Run(); else std::cerr<<"Virtex6 is not instanciated (may be name error)"<<std::endl; }
  void Stop() {mOn = false;}
  virtual ~NumExo2() {}
 private:
  bool mOn;//Whether or not the machine has been turned on
};//NumExo2
class NumExo2_ExoGam: public NumExo2
{
 public:
  NumExo2_ExoGam() {set_name("NumExo2_ExoGam");create_v6("Virtex6");}
  virtual ~NumExo2_ExoGam() {}
};//NumExo2_ExoGam
class NumExo2_NEDA: public NumExo2
{
 public:
  NumExo2_NEDA() {set_name("NumExo2_NEDA");create_v6("V6_NEDA");}
//  void Run()  {std::cout<<name<<" run"<<std::endl; }
  virtual ~NumExo2_NEDA() {}
};//NumExo2_NEDA
class NumExo2_error: public NumExo2
{
 public:
  NumExo2_error() {set_name("NumExo2_error");create_v6("V6_error");}
//  void Run()  {std::cout<<name<<" run"<<std::endl; }
  virtual ~NumExo2_error() {}
};//NumExo2_error

//! computer factory
/**
 *  \note instanciation based on name (i.e. string, as done only one time)
**/
class ComputerFactory
{
  public:
  static Computer *NewComputer(const std::string &name)
  {
    if(name == "laptop")
      return new Laptop;
    if(name == "desktop")
      return new Desktop;
    if(name == "NumExo2_ExoGam")
      return new NumExo2_ExoGam;
    if(name == "NumExo2_NEDA")
      return new NumExo2_NEDA;
    if(name == "NumExo2_error")
      return new NumExo2_error;
    std::cerr<<"Module name is unknown, i.e. \""<<name<<"\"."<<std::endl;
    return NULL;
  }//NewComputer
};//ComputerFactory

//}factory


//! CLI option parse and ...
int main(int argc, char **argv)
{
  //CLI arguments
  struct arguments arguments;
  arguments.verbose=0;
  arguments.module="none";

//! - print default option values (static)
  if(0)
  {
    printf("default values:\n");
    print_args(&arguments);
  }//print default option values

//! - Parse arguments (see parse_option) and eventually show usage, help or version and exit for these lasts
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  ///print option values
  if(arguments.verbose)
  {
    printf("command line option values:\n");
    print_args(&arguments);
  }//print default option values


  if(0)
  {
    Virtex6 v6;      v6.Run();
    V6_NEDA v6n;     v6n.Run();
    Laptop laptop;   laptop.Run();
    Desktop desktop; desktop.Run();
  }//objects
  if(0)
  {
    Device *v6=DeviceFactory::NewDevice("Virtex6"); if(v6!=NULL)  v6->Run();
    Device *v6n=DeviceFactory::NewDevice("V6_NEDA");if(v6n!=NULL) v6n->Run();
  }//factory objects

  std::cout<<"computer=\""<<arguments.module<<"\""<<std::endl;
  Computer* computer=ComputerFactory::NewComputer(arguments.module);
  if(computer!=NULL) computer->Run();

  return 0;
}//main
