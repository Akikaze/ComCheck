///
/// \file main.cpp
/// \brief Main file
/// \author David Feraud
/// \version 1
///

#include "main.hpp"

///
/// \brief Main function
/// \param argc Counter of arguments
/// \param argv Pointer on arguments' list
/// \return EXIT_SUCCESS
///
int
main
(
	int argc,
	char ** argv
)
{
	Core * core = Core::get_instance() ;
	core->loop() ;
	delete core ;

	return EXIT_SUCCESS ;
}
