/**
 * \file openmp.cpp
 * \brief OpenMP minimal hello program (i.e. template)
 * \author Sebastien COUDERT
**/

//ArgP
#include <error.h>
#include <argp.h>
#include <stdlib.h> //atoi

#include <string.h>
//#include <string> #for C++

//OpenMP
#include <omp.h>

#define VERSION "v0.0.1"

//Program option/documentation
//{argp
//! [argp] version of program
const char *argp_program_version=VERSION;
//! [argp] author of program
const char *argp_program_bug_address="sebastien.coudert@ganil.fr";
//! [argp] documentation of program
static char doc[]=
   "openmp: OpenMP minimal/template program\
\n  openmp."VERSION"\
\n\
examples:\n\
  openmp --help\n\
  openmp -v\n\
  openmp -v -n 4 -s 'Hello_OpenMP_thread#'\n\
  openmp -V\n\
  openmp --usage";

//! [argp] A description of the arguments
static char args_doc[] = "";

//! [argp] The options and its description
static struct argp_option options[]=
{
  {"verbose",  'v', 0, 0,           "Produce verbose output" },
  {"count",    'n', "VALUE", 0,     "thread count (default -1 for dynamic number)" },
  {"message",  's', "STRING", 0,    "display message" },
//default options
  { 0 }
};//options (CLI)

//! [argp] Used by main to communicate with parse_option
struct arguments
{
  //! verbose mode (boolean)
  int verbose;
  //! thread count
  int threadCount;
  //! display message
  char* message;
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
    case 'n':
      arguments->threadCount=atoi(arg);
      break;
    case 's':
      arguments->message=arg;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }//switch
  return 0;
}//parse_option


//! [argp] print argument values
void print_args(struct arguments *p_arguments)
{
  printf (".verbose=%s\n.threadCount=%d\n.message=%s\n"
  , p_arguments->verbose?"yes":"no"
  , p_arguments->threadCount
  , p_arguments->message
  );
}//print_args

//! [argp] setup argp parser
static struct argp argp = { options, parse_option, args_doc, doc };

//}argp


//! CLI option parse and ...
int main(int argc, char **argv)
{
  //CLI arguments
  struct arguments arguments;
  arguments.verbose=0;
  arguments.threadCount=-1;
  arguments.message="Hello OpenMP #";

//! - print default option values (static)
  if(0)//0 or 1
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

  if(arguments.threadCount>0)
  {//user number of thread
    omp_set_dynamic(0);
    omp_set_num_threads(arguments.threadCount);
  }//user
  #pragma omp parallel
  {
    int ti=omp_get_thread_num(),tn=omp_get_num_threads();
    printf("%s%d/%d.\n",arguments.message,ti,tn);
  }//parallel section
  return 0;
}//main
