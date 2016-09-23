#include "console_ui.hpp"

void
ConsoleUI::commands
(
	QStringList param_list
)
{
	Q_UNUSED( param_list ) ;

	// display like man
}

void
ConsoleUI::help
(
	QStringList param_list
)
{
	std::cout << std::endl ;
	param_list.erase( param_list.begin() ) ;

	if( param_list.isEmpty() )
	{
		display_title( "What is ComCheck?" ) ;

		display_paragraph( "ComCheck is a little tool used for analyzing source code files and counting how many comments are written in these files, regardless of programming language.") ;
		/*
		std::cout << "ComCheck is a little tool used for analyzing source code files and counting how" << std::endl ;
		std::cout << "many comments are written in these files, regardless of programming language." << std::endl ;
		*/

		std::cout << std::endl ;

		display_paragraph( "Obviously, C++ comments are not written like HTML comments which don't either look like PHP comments. But what is common between every langage is the fact that they require files which contain lines.") ;
		/*
		std::cout << "Obviously, C++ comments are not written like HTML comments which don't either" << std::endl ;
		std::cout << "look like PHP comments. But what is common between every langage is the fact" << std::endl ;
		std::cout << "that they require files which contain lines." << std::endl ;
		*/

		display_paragraph( "This is why ComCheck looks every folder of a project and lists every files before checking, for each file, the type of each line (code, comment or mix)." ) ;
		/*
		std::cout << "This is why ComCheck looks every folder of a project and lists every files" << std::endl ;
		std::cout << "before checking, for each file, the type of each line (code, comment or mix)." << std::endl ;
		*/

		std::cout << std::endl ;

		display_paragraph( "But, for determining if a line is a comment, ComCheck needs to know how are written comments in every langage. And this is really hard for a static system. To be able to understand even future langage still not invented, ComCheck uses a system of modules that you can plug directly in ComCheck. By default, ComCheck brings a module for C++. But if you know how to use C++ and QtCreator, you can create your own module for a specific language." ) ;
		/*
		std::cout << "But, for determining if a line is a comment, ComCheck needs to know how are" << std::endl ;
		std::cout << "written comments in every langage. And this is really hard for a static system." << std::endl ;
		std::cout << "To be able to understand even future langage still not invented, ComCheck uses" << std::endl ;
		std::cout << "a system of modules that you can plug directly in ComCheck. By default, ComCheck" << std::endl ;
		std::cout << "brings a module for C++. But if you know how to use C++ and QtCreator, you can" << std::endl ;
		std::cout << "create your own module for a specific language." << std::endl ;
		*/

		std::cout << std::endl ;

		display_paragraph( "A module defines the file extension, to avoid looking for JAVA comments in Ruby files, and how to get the type of a specific line. Those modules are stored in dynamic librairies (.so or .a and .dl according to your operating system)." ) ;
		/*
		std::cout << "A module defines the file extension, to avoid looking for JAVA comments in Ruby" << std::endl ;
		std::cout << "files, and how to get the type of a specific line. Those modules are stored in" << std::endl ;
		std::cout << "dynamic librairies (.so or .a and .dl according to your operating system)." << std::endl ;
		*/

		std::cout << std::endl ;

		display_title( "How to use ComCheck?" ) ;
	}
	else
	{
		// display help for a specific command
	}
}
