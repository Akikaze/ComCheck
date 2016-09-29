#include "console_ui.hpp"
#include "core.hpp"

void
ConsoleUI::commands
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	// display like man
}

void
ConsoleUI::analyze
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;
	core_->make_report() ;
}

void
ConsoleUI::directory
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	QString directory ;
	std::string tmp ;
	bool try_again = true ;

	while( try_again )
	{
		// ask for a directory
		if( param_list.empty() )
		{
			std::cout << "Address of project folder ? " ;

			// get answer
			std::getline( std::cin, tmp ) ;
			directory = tmp.c_str() ;

			// flush
			tmp = "" ;
		}
		else
		{
			for( int i = 0 ; i < param_list.size() ; ++i )
			{
				// directory
				if( QString( param_list[ i ] ) == "-d" )
				{
					// get the answer in the next parameter
					directory = param_list[ ++i ] ;
				}
			}

			param_list.clear() ;
		}

		// check if the directory exists
		QDir test( directory ) ;
		if( directory.isEmpty() || test.exists() )
		{
			try_again = false ;
		}
		else
		{
			std::cout << "This directory is not found by the system." << std::endl ;
		}
	}

	if( !( directory.isEmpty() ) )
	{
		// release a potential previous tree view
		core_->release_tree() ;

		// set the directory in the core
		core_->set_directory( directory ) ;
	}
	else
	{
		bufferize_text( "Nothing was done." ) ;
	}

	display_buffer() ;
}

void
ConsoleUI::help
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;
	std::cout << std::endl ;

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

void
ConsoleUI::info
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	const QString directory = core_->get_directory() ;
	if( !( directory.isEmpty() ) )
	{
		bufferize_text( color_text( "Project directory: ", CUI_White ) ) ;
		bufferize_text( core_->get_directory() ) ;
	}

	const IPlugin * plugin = core_->get_plugin() ;
	if( plugin != nullptr )
	{
		bufferize_text( color_text( "Project language: ", CUI_White ) ) ;
		bufferize_text( core_->get_plugin()->get_language() ) ;
	}

	display_buffer() ;
}

void
ConsoleUI::language
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	QString language ;
	QList< IPlugin * > list = core_->get_list_plugins() ;
	IPlugin * plugin = nullptr ;
	std::string tmp ;
	bool try_again = true ;

	while( try_again )
	{
		// ask for a plugin language
		if( param_list.empty() )
		{
			std::cout << "The system handles:" << std::endl ;
			for( int i = 0 ; i < list.size() ; ++i )
			{
				tmp = color_text( list[ i ]->get_language(), CUI_White ).toStdString() ;
				std::cout << "\t" << tmp << std::endl ;
				tmp = "" ;
			}

			std::cout << "Language? " ;

			// get answer
			std::getline( std::cin, tmp ) ;
			language = tmp.c_str() ;

			// flush
			tmp = "" ;
		}
		else
		{
			for( int i = 0 ; i < param_list.size() ; ++i )
			{
				// language
				if( QString( param_list[ i ] ) == "-l" )
				{
					// get the answer in the next parameter
					language = param_list[ ++i ] ;
				}
			}

			param_list.clear() ;
		}

		if( language.isEmpty() )
		{
			try_again = false ;
		}
		else
		{
			plugin = core_->find_plugin( language ) ;

			if( plugin != nullptr )
			{
				try_again = false ;
			}
			else
			{
				std::cout << "This language is not handled by the system." << std::endl ;
			}
		}
	}

	if( !( language.isEmpty() ) )
	{
		// choose the plugin in the core
		core_->set_plugin( plugin ) ;
	}
	else
	{
		bufferize_text( "Nothing was done." ) ;
	}

	display_buffer() ;
}

void
ConsoleUI::preparation
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	if( core_->create_tree_view() == nullptr )
	{
		bufferize_text( "This project folder doesn't contain any file which match with language's extensions." ) ;
	}

	display_buffer() ;
}

void
ConsoleUI::tree
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	const CC_Folder * folder = core_->get_root() ;

	if( folder != nullptr )
	{
		bufferize_text( folder->name ) ;

		display_tree( folder ) ;
	}
	else
	{
		bufferize_text( "The system can't find the project directory. There is three possibilities: you have not chosen a project folder with the command 'directory', you have not launch the command 'preparation' or you have but there is no file in this project folder for this language." ) ;

	}

	display_buffer() ;
}
