#include "console_ui.hpp"
#include "core.hpp"

///
/// \brief Display the result of the command 'commands'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::commands
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	// display the list of commands like function in the shell (cf 'man stdio')
	bufferize_text() ;
	bufferize_text( color_text( "\tFunction\tDescription", CUI_White ) ) ;
	bufferize_text( color_text( "\t__________________________________________________________________", CUI_White ) ) ;
	bufferize_text() ;

	bufferize_text( color_text( "\tanalyze\t\t", CUI_White ) + "analyze and create a report of the current folder" ) ;
	bufferize_text( color_text( "\tcommands\t", CUI_White ) + "display each command and its description" ) ;
	bufferize_text( color_text( "\tclear\t\t", CUI_White ) + "clear the screen" ) ;
	bufferize_text( color_text( "\tdirectory\t", CUI_White ) + "choose the project directory" ) ;
	bufferize_text( color_text( "\thelp\t\t", CUI_White ) + "display more information about ComCheck or each command" ) ;
	bufferize_text( color_text( "\tinfo\t\t", CUI_White ) + "display information about the project" ) ;
	bufferize_text( color_text( "\tlanguage\t", CUI_White ) + "choose the project language" ) ;
	bufferize_text( color_text( "\tmove\t\t", CUI_White ) + "change the current folder" ) ;
	bufferize_text( color_text( "\tpreparation\t", CUI_White ) + "create a tree view of the project directory" ) ;
	bufferize_text( color_text( "\tquit\t\t", CUI_White ) + "quit Comcheck" ) ;
	bufferize_text( color_text( "\treport\t\t", CUI_White ) + "display the report of the current folder" ) ;
	bufferize_text( color_text( "\ttree\t\t", CUI_White ) + "display a tree view of the project folder" ) ;
	bufferize_text() ;

	display_buffer() ;
}

///
/// \brief Display the result of the command 'analyze'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::analyze
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	if( current_folder_ != nullptr )
	{
		// create the report of the current folder
		current_report_ = core_->make_report( current_folder_ ) ;

		// display the report just after
		report( { "report" } ) ;
	}
	else
	{
		bufferize_text( color_text( "The system can't find the project directory. There are three possibilities: you have not chosen a project folder with the command 'directory', you have not launch the command 'preparation' or you have but there is no file in this project folder for this language.", CUI_Red ) ) ;
		display_buffer() ;
	}

}

///
/// \brief Display the result of the command 'directory'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::directory
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	QString directory_name ;
	std::string tmp ;
	bool try_again = true ;

	if( current_folder_ == nullptr )
	{
		while( try_again )
		{
			// ask for a directory
			if( param_list.empty() )
			{
				std::cout << "Address of project folder ? " ;

				// get answer
				std::getline( std::cin, tmp ) ;
				directory_name = tmp.c_str() ;

				// flush
				tmp = "" ;
			}
			else
			{
				// get the answer in the next parameter
				directory_name = param_list.front() ;
				param_list.clear() ;
			}

			// check if the directory exists
			QDir test( directory_name ) ;
			if( directory_name.isEmpty() || test.exists() )
			{
				// just quit
				try_again = false ;
			}
			else
			{
				bufferize_text( color_text( "This directory is not found by the system.", CUI_Red ) ) ;
				display_buffer() ;
			}
		}

		if( !( directory_name.isEmpty() ) )
		{
			// withdraw a potential slash at the end of the file
			if( directory_name.at( directory_name.size() - 1 ) == '/' )
			{
				directory_name = directory_name.left( directory_name.size() - 1 ) ;
			}

			// set the directory in the core
			core_->set_directory( directory_name ) ;
		}
		else
		{
			bufferize_text( color_text( "Nothing was done.", CUI_Red ) ) ;
		}
	}
	else
	{
		// release a potential previous tree view
		core_->release_tree() ;
		current_folder_ = nullptr ;

		// release potential reports
		core_->release_reports() ;
		current_report_ = nullptr ;

		if( param_list.empty() )
		{
			bufferize_text( color_text( "The directory defines the tree view. So changing it means recreate a new tree view. If you want to do that, just use the command 'directory -r <address>' or 'directory --reset <address>'.", CUI_Red ) ) ;
		}
		else if( param_list.front() == "-r" ||
				 param_list.front() == "--reset")
		{
			// release a potential previous tree view
			core_->release_tree() ;
			current_folder_ = nullptr ;

			param_list.erase( param_list.begin() ) ;

			// call language another time
			if( param_list.empty() )
			{
				directory( { "directory" } ) ;
			}
			else
			{
				directory( { "directory", param_list.front() } ) ;
			}
		}
	}

	display_buffer() ;
}

///
/// \brief Display the result of the command 'export'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::export_HTML
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	if( current_report_ != nullptr )
	{
		core_->export_HTML() ;
	}
	else
	{
		bufferize_text( color_text( "Impossible to export reports because you have not made any reports.", CUI_Red ) ) ;
	}
}

///
/// \brief Display the result of the command 'help'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::help
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	if( param_list.isEmpty() )
	{
		// display common help
		bufferize_text() ;
		bufferize_title( "What is ComCheck?" ) ;
		bufferize_text() ;

		bufferize_text( "ComCheck is a little tool used for analyzing source code files and counting how many comments are written in these files, regardless of programming language.") ;
		bufferize_text() ;

		bufferize_text( "Obviously, C++ comments are not written like HTML comments which don't either look like PHP comments. But what is common between every langage is the fact that they require files which contain lines. This is why ComCheck looks every folder of a project and lists every files before checking, for each file, the type of each line (code, comment or mix)." ) ;
		bufferize_text() ;

		bufferize_text( "But, for determining if a line is a comment, ComCheck needs to know how are written comments in every langage. And this is really hard for a static system. To be able to understand even future langage still not invented, ComCheck uses a system of modules that you can plug directly in ComCheck. By default, ComCheck brings a module for C++. But if you know how to use C++ and QtCreator, you can create your own module for a specific language." ) ;
		bufferize_text() ;

		bufferize_text( "A module defines the file extension, to avoid looking for JAVA comments in Ruby files, and how to get the type of a specific line. Those modules are stored in dynamic librairies (.so or .a and .dll according to your operating system)." ) ;
		bufferize_text() ;

		bufferize_title( "How to use ComCheck?" ) ;
		bufferize_text() ;

		bufferize_text( "ComCheck analyze every interesting files in a specific folder and create a report which contains the number of lines, the number of comments, the percentage of comments in a file, the average percentage of comments in the whole report, ... To do so, ComCheck requires a project directory and a project language. When these two pieces of information are given. ComCheck is going to create a tree view where the root is the project folder and every leaf is a file defined as interesting by the language." ) ;
		bufferize_text() ;

		bufferize_text( "When the tree view is created, you can make a report on the project directory, or you can move inside to create more precise report. But you can't go outside the tree. If you want to see a folder before the root, you need to change the project directory and recreate the whole tree. It is the same problem with the language: during the creation of the tree, some files were ignored because of a wrong extension. So changing the language requires to recreate the tree another time." ) ;
		bufferize_text() ;

		bufferize_text( "An analyze is going to check, from the folder, every file above this folder. At this moment, you can't limit an analyze to one folder and ignore other folders inside this one." ) ;
		bufferize_text() ;

		bufferize_text( "To know more about the possibility, you can use the command " + color_text( "commands", CUI_White ) + " to display the list of commands and use " + color_text( "help <COMMAND>", CUI_White ) + " to get more info about a specific command." ) ;\
		bufferize_text() ;
	}
	else
	{
		// display help for a specific command

		if( param_list.front() == "commands" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " commands" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " commands" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "Display every command associated with a short description." ) ;
			bufferize_text() ;
		}
		else if( param_list.front() == "clear" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " clear" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " clear" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "Nothing as much as calling " + color_text( "system( \"clear\" ) ;", CUI_White ) + " on UNIX architecture and " + color_text( "system( \"cls\" ) ;", CUI_White ) + " on Windows." ) ;
			bufferize_text() ;
		}
		// COMMANDS
		else if( param_list.front() == "analyze" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " analyze" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " analyze" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "Analyze a tree view from the current folder thanks to a plugin and make a report counting code lines, comments, ... This analyze is not limited to one folder. It is going to check every interesting file in every folder contained in the current one." ) ;
			bufferize_text() ;
		}
		else if( param_list.front() == "directory" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " directory" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " directory [OPTION] [ADDRESS]" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "This allows the user to choose a project folder. The tree view will be created from this folder. Changing it means to reset the previous tree view. " + color_text( "ADDRESS", CUI_White ) + " can be absolute or relative." ) ;

			bufferize_text( "\t" + color_text( "-r", CUI_White ) + ", " + color_text( "--reset", CUI_White ) ) ;
			bufferize_text( "reset a tree view" ) ;

			bufferize_text() ;
		}
		else if( param_list.front() == "export" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " export" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " export" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "Export every report made during the execution in HTML file." ) ;
			bufferize_text() ;
		}
		else if( param_list.front() == "help" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " help" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " help [COMMAND]" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "Do you really need a description of the command " + color_text( "help", CUI_White ) + " ? So it displays a information about a ComCheck or a specific command written at the same place as " + color_text( "COMMAND", CUI_White ) + "." ) ;
			bufferize_text() ;
		}
		else if( param_list.front() == "info" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " info" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " info" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "At the beginning of a normal execution, the command " + color_text( "info", CUI_White ) + " displays nothing. But it changes when you choose the project directory, when you choose the language, when the tree view is created, when you have done some reports, ..." ) ;
			bufferize_text() ;
		}
		else if( param_list.front() == "language" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " language" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " language [OPTION] [LANGUAGE]" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "This allows the user to choose between every plugin the right one for a project. Information is the plugin will serve as a filter for the creation of the tree view. This is why the tree view depends on the language. Thus, changing the language means recreate the tree view and reset every analyze done until now. " + color_text( "LANGUAGE", CUI_White ) + " is the name defined by the plugin. The command " + color_text( "language", CUI_White ) + " can list every plugin so the user can find the best one for its use." ) ;
			bufferize_text() ;

			bufferize_text( "\t" + color_text( "-r", CUI_White ) + ", " + color_text( "--reset", CUI_White ) ) ;
			bufferize_text( "reset a tree view" ) ;

			bufferize_text() ;
		}
		else if( param_list.front() == "move" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " move" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " move [FOLDER]" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "When the tree view is created, by default the current folder is the root of the tree view. But if you want to make a more precise analyze of a specific folder. You need to change the current folder and remake a new analyze. The command " + color_text( "move", CUI_White ) + " give the possibility to climb the tree and change the folder." ) ;
			bufferize_text() ;
		}
		else if( param_list.front() == "preparation" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " preparation" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " preparation" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "The tree view is made from the project folder. It is defined as the root of the tree. It is created thanks to the plugin to be sure that every file in it is interesting. That means no useless files, no useless folders." ) ;
			bufferize_text() ;
		}
		else if( param_list.front() == "report" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " report" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " report [OPTION]" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "This command display the report of the current folder. The user can display a more precise report by using options. When a user analyze a folder, a report is created and stored in a list of reports. That means he can make several reports at the same time. But the " + color_text( "report", CUI_White ) + " command display only one report. To change the report displayed, you need to change move in the right folder and use the command " + color_text( "analyze", CUI_White ) + " another time. But if the report was already make, it is just going to display the first report. It is not going to create a copy of the first report." ) ;
			bufferize_text() ;

			bufferize_text( "\t" + color_text( "-f", CUI_White ) + ", " + color_text( "--files", CUI_White ) ) ;
			bufferize_text( "display the list of files analyzed by the report and use a color to describe the percentage of comments compare to the average percentage. A red file is 20% under the average percentage and a green one is 20% upper. It also displays a short description of the file:" ) ;
			bufferize_text( color_text( "../console/main.cpp", CUI_Yellow ) + " " + color_text( "%", CUI_White ) + " 33.3333 " + color_text( "TOT:", CUI_White ) + " 18 " + color_text( "COM:", CUI_White ) + " 6 " + color_text( "MIX:", CUI_White ) + " 0 " + color_text( "COD:", CUI_White ) + " 12" ) ;
			bufferize_text( "\t" + color_text( "%", CUI_White ) + " percentage" ) ;
			bufferize_text( "\t" + color_text( "TOT", CUI_White ) + " number of lines" ) ;
			bufferize_text( "\t" + color_text( "COM", CUI_White ) + " number of pure comments" ) ;
			bufferize_text( "\t" + color_text( "MIX", CUI_White ) + " number of mixed lines" ) ;
			bufferize_text( "\t" + color_text( "CODE", CUI_White ) + " number of pure code lines" ) ;
			bufferize_text() ;

			bufferize_text( "\t" + color_text( "-h", CUI_White ) + ", " + color_text( "--histogram", CUI_White ) ) ;
			bufferize_text( "display an histogram which shows every files and their value compared to the average" ) ;
			bufferize_text() ;

			bufferize_text( "\t" + color_text( "-t", CUI_White ) + " [NUMBER], " + color_text( "--top", CUI_White ) + " [NUMBER]" ) ;
			bufferize_text( "display the " + color_text( "NUMBER", CUI_White ) + " worst files and the " + color_text( "NUMBER", CUI_White ) + " best files (in term of percentage). By default, " + color_text( "NUMBER", CUI_White ) + " is 5." ) ;

			bufferize_text() ;
		}
		else if( param_list.front() == "tree" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " tree" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " tree" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "Use a recursive function to display every folder and every file that are contained in the current folder." ) ;
			bufferize_text() ;
		}
		// QUIT
		else if( param_list.front() == "quit" )
		{
			bufferize_text( color_text( "NAME -", CUI_White ) + " quit" ) ;
			bufferize_text( color_text( "SYNOPSIS -", CUI_White ) + " quit" ) ;
			bufferize_text( color_text( "DESCRIPTION - ", CUI_White ) + "Close the execution and release every piece of memory that was allocate during the execution." ) ;
			bufferize_text() ;
		}
		else
		{
			// display an error
			bufferize_text( color_text( "There is no command called " + param_list.front() + ".", CUI_Red ) ) ;
			commands( { "commands" } ) ;
		}
	}
	param_list.clear() ;

	display_buffer() ;
}

///
/// \brief Display the result of the command 'info'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::info
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	// display the project directory
	const QString directory = core_->get_directory() ;
	if( !( directory.isEmpty() ) )
	{
		bufferize_text( color_text( "Project directory: ", CUI_White ) + core_->get_directory() ) ;
	}

	// display the project language
	const IPlugin * plugin = core_->get_plugin() ;
	if( plugin != nullptr )
	{
		bufferize_text( color_text( "Project language: ", CUI_White ) + core_->get_plugin()->get_language() ) ;
	}

	// jump a line
	if( !( buffer_.empty() ) )
	{
		bufferize_text() ;
	}

	// display the current folder
	if( current_folder_ != nullptr )
	{
		bufferize_text( color_text( "Current folder: ", CUI_White ) + current_folder_->name ) ;
	}

	// display the current report folder
	if( current_report_ != nullptr )
	{
		bufferize_text( color_text( "Current report folder: ", CUI_White ) + current_report_->folder->name ) ;
	}

	// display list of reports folder
	QList< QPair< CC_Folder *, CC_Report * > > map = core_->get_map_reports() ;
	if( map.size() > 1 )
	{
		bufferize_text() ;
		bufferize_text( color_text( "List of folder already analyzed: ", CUI_White )  ) ;

		// display each report folder
		for( int i = 0 ; i < map.size() ; ++i )
		{
			bufferize_text( map[ i ].first->name ) ;
		}
	}

	display_buffer() ;
}

///
/// \brief Display the result of the command 'language'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::language
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	QString language_name ;
	QList< IPlugin * > list = core_->get_list_plugins() ;
	IPlugin * plugin = nullptr ;
	std::string tmp ;
	bool try_again = true ;

	if( current_folder_ == nullptr )
	{
		while( try_again )
		{
			// ask for a plugin language
			if( param_list.empty() )
			{
				std::cout << "The system handles:" << std::endl ;
				for( int i = 0 ; i < list.size() ; ++i )
				{
					bufferize_text( "\t" + color_text( list[ i ]->get_language(), CUI_White, false ) ) ;
				}
				display_buffer() ;

				std::cout << "Language? " ;

				// get answer
				std::getline( std::cin, tmp ) ;
				language_name = tmp.c_str() ;

				// flush
				tmp = "" ;
			}
			else
			{
				// get the answer in the next parameter
				language_name = param_list.front() ;
				param_list.clear() ;
			}

			if( language_name.isEmpty() )
			{
				// just quit
				try_again = false ;
			}
			else
			{
				plugin = core_->find_plugin( language_name ) ;

				// check if the plugin exists
				if( plugin != nullptr )
				{
					try_again = false ;
				}
				else
				{
					bufferize_text( color_text( "This language is not handled by the system.", CUI_Red ) ) ;
					display_buffer() ;
				}
			}
		}

		if( !( language_name.isEmpty() ) )
		{
			// choose the plugin in the core
			core_->set_plugin( plugin ) ;
		}
		else
		{
			bufferize_text( color_text( "Nothing was done.", CUI_Red ) ) ;
		}
	}
	else
	{
		if( param_list.empty() )
		{
			bufferize_text( color_text( "The language defines the tree view. So changing it means recreate a new tree view. If you want to do that, just use the command 'language -r <language_name>' or 'language --reset <language_name>'.", CUI_Red ) ) ;
		}
		else if( param_list.front() == "-r" ||
				 param_list.front() == "--reset")
		{
			// release a potential previous tree view
			current_folder_ = nullptr ;
			core_->release_tree() ;

			// release potential reports
			core_->release_reports() ;
			current_report_ = nullptr ;

			param_list.erase( param_list.begin() ) ;

			// call language another time
			if( param_list.empty() )
			{
				language( { "language" } ) ;
			}
			else
			{
				language( { "language", param_list.front() } ) ;
			}
		}
	}

	display_buffer() ;
}

///
/// \brief Display the result of the command 'move'
/// \param param_list List of parameters for the command
///
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

	// check if you have a tree view
	if( current_folder_ != nullptr )
	{
		QList< CC_Folder * >::const_iterator cit_Folder ;

		// by default, display every possibilities
		if( param_list.empty() )
		{
			name = current_folder_->name + "/" ;

			bufferize_text( color_text( "Current folder: ", CUI_White ) + name ) ;
			bufferize_text() ;

			QList< CC_File * >::const_iterator cit_File ;

			// display list of files that can be analyzed in this folder
			if( !( current_folder_->list_files.empty() ) )
			{
				bufferize_text( "In this folder, you can find:" ) ;

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

			// display possible movement
			if( !( current_folder_->list_folders.empty() ) )
			{
				bufferize_text( "You can move to: " ) ;

				for( cit_Folder = ( current_folder_->list_folders ).constBegin() ;
					 cit_Folder != ( current_folder_->list_folders ).constEnd() ;
					 ++cit_Folder )
				{
					// display the name correctly to be sure that the user can fo there
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

			// display the parent folder to allow back step
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
				// display the command
				bufferize_text( "You just need to write 'move <name_folder>' to change the current folder." ) ;
			}
		}
		else
		{
			// create a list of possibilities
			QList< CC_Folder * > list_folders = current_folder_->list_folders ;
			if( current_folder_->parent != nullptr )
			{
				list_folders.push_front( current_folder_->parent ) ;
			}

			QString address = param_list.front() ;
			cit_Folder = list_folders.constBegin() ;

			// check if this address is reachable from here
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

			// display the result
			if( name == address )
			{
				bufferize_text( "The current folder has changed." ) ;
			}
			else
			{
				bufferize_text( color_text( "Impossible to move to this folder. Check if ", CUI_Red ) + address + color_text( " is the following list.", CUI_Red ) ) ;

				// display possibilities if there is an error
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

///
/// \brief Display the result of the command 'preparation'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::preparation
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;
	// create the tree view
	current_folder_ = core_->create_tree_view() ;

	if( current_folder_ == nullptr )
	{
		// display error if the folder is useless
		bufferize_text( color_text( "This project folder doesn't contain any file which match with language's extensions. Maybe you forgot to choose the language.", CUI_Red ) ) ;
	}

	display_buffer() ;
}

///
/// \brief Display the result of the command 'report'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::report
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	if( current_report_ != nullptr )
	{
		// display a warning if the user is not at the right place
		if( current_report_->folder != current_folder_ )
		{
			bufferize_text( color_text( "Be careful, you are not displaying the report of the current folder.", CUI_Yellow ) ) ;
			bufferize_text( "If you want a report about the current folder, you need to use 'analyze' before." ) ;
			bufferize_text() ;
		}

		if( param_list.empty() )
		{
			// display a little report with average, ...
			bufferize_text( color_text( "Report folder: ", CUI_White ) + color_text( current_report_->folder->name, CUI_Blue ) ) ;
			bufferize_text() ;

			display_array( current_report_->array );
			display_report( current_report_ ) ;

		}
		else
		{
			// display a precise report
			QList< CC_File * >::const_iterator cit ;

			for( int i = 0 ; i < param_list.size() ; ++i )
			{
				// files
				if( QString( param_list[ i ] ) == "-f" ||
					QString( param_list[ i ] ) == "--files" )
				{
					// list every file colored in accordance with the average
					QString line = "" ;

					for( cit = current_report_->list_files.constBegin() ;
						 cit != current_report_->list_files.constEnd() ;
						 ++cit )
					{
						line = "" ;
						CC_File * file = ( *cit ) ;
						CUI_TextColor tc = CUI_Yellow ;

						// change the color
						if( ( *cit )->com_tot < 0.8 * current_report_->ct_statistics.average )
						{
							// bad file
							tc = CUI_Red ;
						}

						if( ( *cit )->com_tot > 1.2 * current_report_->ct_statistics.average )
						{
							// good file
							tc = CUI_Green ;
						}

						// display its information
						line = color_text( file->name, tc ) ;
						line += " " + color_text( "% ", CUI_White ) + QString::number( ( *cit )->com_tot ) ;
						line += " " + display_array( file->array, true ) ;

						bufferize_text( line ) ;
					}

					bufferize_text() ;
				}

				// histogram
				if( QString( param_list[ i ] ) == "-h" ||
					QString( param_list[ i ] ) == "--histogram" )
				{
					// display a histogram
					draw_histogram() ;
					bufferize_text() ;
				}

				// level
				if( QString( param_list[ i ] ) == "-t" ||
					QString( param_list[ i ] ) == "--top" )
				{
/*
					// display best and worst files
					CC_File * file = nullptr ;
					QString line = "" ;
					unsigned int number = 5 ;
					unsigned int pos = 0 ;

					if( param_list.size() > i + 1 )
					{
						// get the number of files
						number = ( param_list[ ++i ] ).toInt() ;
					}
					else
					{
						bufferize_text( "By default, it display the top 5 of best and worst file" ) ;
					}

					// create a list for sorting
					QList< QPair< double, int > > sort_list ;
					for( int i = 0 ; i < current_report_->percents.size() ; ++i )
					{
						sort_list.push_back( qMakePair( current_report_->percents[ i ], i ) ) ;
					}

					std::sort( sort_list.begin(), sort_list.end() ) ;

					bufferize_text( QString::number( number ) + " worst commented file:" ) ;
					for( unsigned int i = 0 ; i < number ; ++i )
					{
						// get the worst file
						pos = sort_list[ i ].second ;
						file = current_report_->list_files[ pos ] ;

						// display its information
						line = color_text( file->name, CUI_Red ) ;
						line += " " + color_text( "% ", CUI_White ) + QString::number( current_report_->percents[ pos ] ) ;
						line += " " + display_array( file->array, true ) ;

						bufferize_text( line ) ;
					}
					bufferize_text() ;

					bufferize_text( QString::number( number ) + " best commented file:" ) ;
					for( unsigned int i = 1 ; i < number + 1 ; ++i )
					{
						// get the best file
						pos = sort_list[ sort_list.size() - i ].second ;
						file = current_report_->list_files[ pos ] ;

						// change the name
						line = file->name ;
						line.right( line.size() - current_report_->folder->name.size() ) ;

						// display its information
						line = color_text( line, CUI_Green ) ;
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

///
/// \brief Display the result of the command 'tree'
/// \param param_list List of parameters for the command
///
void
ConsoleUI::tree
(
	QStringList param_list
)
{
	param_list.erase( param_list.begin() ) ;

	if( current_folder_ != nullptr )
	{
		// display the name of the current folder
		bufferize_text( color_text( current_folder_->name, CUI_Blue ) ) ;

		// display the tree view from this current folder
		display_tree( current_folder_ ) ;
	}
	else
	{
		bufferize_text( color_text( "The system can't find the project directory. There are three possibilities: you have not chosen a project folder with the command 'directory', you have not launch the command 'preparation' or you have but there is no file in this project folder for this language.", CUI_Red ) ) ;

	}

	display_buffer() ;
}
