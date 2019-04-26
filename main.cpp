/**
 * \brief minimal Hello program (doxygen syntax)
 * \author Sebastien COUDERT
**/

#include <string>         // std::string
#include <iostream>       // std::cout

//! hello starts here
/**
 * \warning \c argc and \c argv not used yet, see argp branch (e.g. git checkout argp)
**/
int main(int argc, char **argv)
{
  //! print hello message on terminal
  std::cout<<"Hello C++"<<std::endl;
  return 0;
}//main

