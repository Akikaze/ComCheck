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
		bufferize_title( "What is ComCheck?" ) ;

		bufferize_text( "ComCheck is a little tool used for analyzing source code files and counting how many comments are written in these files, regardless of programming language.") ;
		bufferize_text() ;

		bufferize_text( "Obviously, C++ comments are not written like HTML comments which don't either look like PHP comments. But what is common between every langage is the fact that they require files which contain lines.") ;
		bufferize_text( "This is why ComCheck looks every folder of a project and lists every files before checking, for each file, the type of each line (code, comment or mix)." ) ;
		bufferize_text() ;

		bufferize_text( "But, for determining if a line is a comment, ComCheck needs to know how are written comments in every langage. And this is really hard for a static system. To be able to understand even future langage still not invented, ComCheck uses a system of modules that you can plug directly in ComCheck. By default, ComCheck brings a module for C++. But if you know how to use C++ and QtCreator, you can create your own module for a specific language." ) ;
		bufferize_text() ;

		bufferize_text( "A module defines the file extension, to avoid looking for JAVA comments in Ruby files, and how to get the type of a specific line. Those modules are stored in dynamic librairies (.so or .a and .dll according to your operating system)." ) ;
		bufferize_text() ;

		bufferize_title( "How to use ComCheck?" ) ;

		display_buffer() ;
	}
	else
	{
		// display help for a specific command
	}
}
