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

	if( current_folder_ != nullptr )
	{
		current_report_ = core_->make_report( current_folder_ ) ;
		report( { "report" } ) ;
	}
	else
	{
		bufferize_text( color_text( "The system can't find the project directory. There are three possibilities: you have not chosen a project folder with the command 'directory', you have not launch the command 'preparation' or you have but there is no file in this project folder for this language.", CUI_Red ) ) ;
	}

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
					if( parm_list.size() > i + 1 )
					{
						// get the answer in the next parameter
						directory = param_list[ ++i ] ;
					}
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
		current_folder_ = nullptr ;

		if( directory.at( directory.size() - 1 ) == '/' )
		{
			directory = directory.left( directory.size() - 1 ) ;
		}

		// set the directory in the core
		core_->set_directory( directory ) ;
	}
	else
	{
		bufferize_text( color_text( "Nothing was done.", CUI_Red ) ) ;
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

		bufferize_text( "Obviously, C++ comments are not written like HTML comments which don't either look like PHP comments. But what is common between every langage is the fact that they require files which contain lines. This is why ComCheck looks every folder of a project and lists every files before checking, for each file, the type of each line (code, comment or mix)." ) ;
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
		bufferize_text( color_text( "Project directory: ", CUI_White ) + core_->get_directory() ) ;
	}

	const IPlugin * plugin = core_->get_plugin() ;
	if( plugin != nullptr )
	{
		bufferize_text( color_text( "Project language: ", CUI_White ) + core_->get_plugin()->get_language() ) ;
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
                tmp = color_text( list[ i ]->get_language(), CUI_White, false ).toStdString() ;
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
		bufferize_text( color_text( "Nothing was done.", CUI_Red ) ) ;
	}

	display_buffer() ;
}

void
ConsoleUI::move
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;
	QString name = "" ;
	bool possible_move = false ;
	size_t pos = 0 ;

	if( current_folder_ != nullptr )
	{
		QList< CC_Folder * >::const_iterator cit_Folder ;

		if( param_list.empty() )
		{
			name = current_folder_->name + "/" ;

			bufferize_text( color_text( "Current folder: ", CUI_White ) + name ) ;
			bufferize_text() ;

			QList< CC_File * >::const_iterator cit_File ;

			if( !( current_folder_->list_files.empty() ) )
			{
				bufferize_text( "In this folder, you can analyze:" ) ;

				for( cit_File = ( current_folder_->list_files ).constBegin() ;
					 cit_File != ( current_folder_->list_files ).constEnd() ;
					 ++cit_File )
				{
					name = ( *cit_File )->name ;
					pos = name.toStdString().find_last_of( '/' ) ;

					if( pos != name.toStdString().npos )
					{
						name = name.right( name.size() - pos - 1 ) ;
					}

					bufferize_text( "\t" + color_text( name, CUI_Yellow ) ) ;
				}

				bufferize_text() ;
			}

			if( !( current_folder_->list_folders.empty() ) )
			{
				bufferize_text( "You can move to: " ) ;

				for( cit_Folder = ( current_folder_->list_folders ).constBegin() ;
					 cit_Folder != ( current_folder_->list_folders ).constEnd() ;
					 ++cit_Folder )
				{
					name = ( *cit_Folder )->name ;
					pos = name.toStdString().find_last_of( '/' ) ;

					if( pos != name.toStdString().npos )
					{
						name = name.right( name.size() - pos - 1 ) ;
					}

					bufferize_text( "\t" + color_text( name, CUI_Blue ) ) ;
				}

				bufferize_text() ;
				possible_move = true ;
			}

			if( current_folder_->parent != nullptr )
			{
				name = current_folder_->parent->name ;
				pos = name.toStdString().find_last_of( '/' ) ;

				if( pos != name.toStdString().npos )
				{
					name = name.right( name.size() - pos - 1 ) ;
				}

				bufferize_text( "You can go back to: " + color_text( name, CUI_Blue ) ) ;
				possible_move = true ;
			}

			if( possible_move == true )
			{
				bufferize_text( "You just need to write 'move <name_folder>' to change the current folder." ) ;
			}
		}
		else
		{
			QList< CC_Folder * > list_folders = current_folder_->list_folders ;
			if( current_folder_->parent != nullptr )
			{
				list_folders.push_front( current_folder_->parent ) ;
			}

			QString address = param_list[ 0 ] ;
			cit_Folder = list_folders.constBegin() ;

			while( name != address && cit_Folder != list_folders.constEnd() )
			{
				name = ( *cit_Folder )->name ;
				pos = name.toStdString().find_last_of( '/' ) ;

				if( pos != name.toStdString().npos )
				{
					name = name.right( name.size() - pos - 1 ) ;
				}

				if( name == address )
				{
					current_folder_ = ( *cit_Folder ) ;
				}

				++cit_Folder ;
			}

			if( name == address )
			{
				bufferize_text( "The current folder has changed." ) ;
			}
			else
			{
				bufferize_text( color_text( "Impossible to move to this folder. Check if ", CUI_Red ) + address + color_text( " is the following list.", CUI_Red ) ) ;
				move( { "move" } ) ;
			}
		}
	}
	else
	{
		bufferize_text( color_text( "Your project folder was not prepared by the system.", CUI_Red ) ) ;
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
	current_folder_ = core_->create_tree_view() ;

	if( current_folder_ == nullptr )
	{
		bufferize_text( color_text( "This project folder doesn't contain any file which match with language's extensions. Maybe you forgot to choose the language.", CUI_Red ) ) ;
	}

	display_buffer() ;
}

void
ConsoleUI::report
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	if( current_report_ != nullptr )
	{
		if( param_list.empty() )
		{
			bufferize_text( color_text( "Report folder: ", CUI_White ) + color_text( current_report_->folder->name, CUI_Blue ) ) ;
			bufferize_text() ;

			display_array( current_report_->array );
			display_report( current_report_ ) ;

		}
		else
		{
			QList< CC_File * >::const_iterator cit ;

			for( int i = 0 ; i < param_list.size() ; ++i )
			{
				// files
				if( QString( param_list[ i ] ) == "-f" ||
					QString( param_list[ i ] ) == "--files" )
				{
					unsigned int pos = 0 ;
					QString line = "" ;

					for( cit = current_report_->list_files.constBegin() ;
						 cit != current_report_->list_files.constEnd() ;
						 ++cit )
					{
						line = "" ;
						CC_File * file = ( *cit ) ;
						CUI_TextColor tc = CUI_Yellow ;

						if( current_report_->percents[ pos ] < 0.8 * current_report_->average )
						{
							tc = CUI_Red ;
						}

						if( current_report_->percents[ pos ] > 1.2 * current_report_->average )
						{
							tc = CUI_Green ;
						}

						line = color_text( file->name, tc ) ;
						line += " " + color_text( "% ", CUI_White ) + QString::number( current_report_->percents[ pos ] ) ;
						line += " " + display_array( file->array, true ) ;

						bufferize_text( line ) ;
						pos++ ;
					}

					bufferize_text() ;
				}

				// histogram
				if( QString( param_list[ i ] ) == "-h" ||
					QString( param_list[ i ] ) == "--histogram" )
				{

				}

				// level
				if( QString( param_list[ i ] ) == "-l" ||
					QString( param_list[ i ] ) == "--level" )
				{
					unsigned int number = ( param_list[ ++i ] ).toInt() ;

					QList< double > trick = current_report_->percents ;
					QList< double >::iterator it ;

					unsigned int decimal = 1 ;
					QString tmp = QString::number( trick.size() ) ;
					for( int i = 0 ; i < tmp.length() ; ++i )
					{
						decimal *= 10 ;
					}

					for( int i = 0 ; i < trick.size() ; ++i )
					{
						trick[ i ] *= decimal ;
						trick[ i ] += i ;
					}

					std::sort( trick.begin(), trick.end() ) ;

					QString line ;
					CC_File * file ;
					int pos = 0 ;

					/*

					bufferize_text( QString::number( number ) + " worst commented file:" ) ;
					for( unsigned int i = 0 ; i < number ; ++i )
					{
						pos = ( int )( trick[ i ] - ( ( int )( trick[ i ] / decimal ) * decimal ) ) ;
						file = current_report_->list_files[ pos ] ;

						line = color_text( file->name, CUI_Red ) ;
						line += " " + color_text( "% ", CUI_White ) + QString::number( current_report_->percents[ pos ] ) ;
						line += " " + display_array( file->array, true ) ;

						bufferize_text( line ) ;
					}
					bufferize_text() ;

					bufferize_text( QString::number( number ) + " best commented file:" ) ;
					for( unsigned int i = 1 ; i < number + 1 ; ++i )
					{
						std::cout << trick[ trick.size() - i ] - ( ( int )( trick[ trick.size() - i ] / decimal ) * decimal ) << std::endl ;
						pos = ( trick[ trick.size() - i ] - ( int )( trick[ trick.size() - i ] ) ) * decimal ;
						std::cout << pos << std::endl ;
						file = current_report_->list_files[ pos ] ;

						line = color_text( file->name, CUI_Green ) ;
						line += " " + color_text( "% ", CUI_White ) + QString::number( current_report_->percents[ pos ] ) ;
						line += " " + display_array( file->array, true ) ;

						bufferize_text( line ) ;
					}

					bufferize_text() ;

					*/
				}
			}

			param_list.clear() ;
		}
	}
	else
	{
		bufferize_text( color_text( "The system can't find a report for this position. You need to use 'analyze' to create the report before using 'report'.", CUI_Red ) ) ;
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

	if( current_folder_ != nullptr )
	{
		bufferize_text( color_text( current_folder_->name, CUI_Blue ) ) ;
		display_tree( current_folder_ ) ;
	}
	else
	{
		bufferize_text( color_text( "The system can't find the project directory. There are three possibilities: you have not chosen a project folder with the command 'directory', you have not launch the command 'preparation' or you have but there is no file in this project folder for this language.", CUI_Red ) ) ;

	}

	display_buffer() ;
}
