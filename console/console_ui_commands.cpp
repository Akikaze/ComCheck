#include "console_ui.hpp"
#include "core.hpp"

/*
'commands' exist to help the user and show the list of commands he can use
directly in the command line.

The idea is that the user will list the commands with this command and call
the command help following by one of this word.

The display of commands is a copy of what we can find in the shell (cf 'man stdio')
*/
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

	bufferize_text() ;
	bufferize_text( color_text( "\tFunction\tDescription", CUI_White ) ) ; // table format
	bufferize_text( color_text( "\t__________________________________________________________________", CUI_White ) ) ;
	bufferize_text() ;

	bufferize_text( color_text( "\tanalyze\t\t", CUI_White ) + "analyze and create a report of the current folder" ) ;
	bufferize_text( color_text( "\tcommands\t", CUI_White ) + "display each command and its description" ) ;
	bufferize_text( color_text( "\tclear\t\t", CUI_White ) + "clear the screen" ) ;
	bufferize_text( color_text( "\tdirectory\t", CUI_White ) + "choose the project directory" ) ;
	bufferize_text( color_text( "\texport\t\t", CUI_White ) + "export reports in HTML files" ) ;
	bufferize_text( color_text( "\thelp\t\t", CUI_White ) + "display more information about ComCheck or each command" ) ;
	bufferize_text( color_text( "\tinfo\t\t", CUI_White ) + "display information about the project and reports" ) ;
	bufferize_text( color_text( "\tlanguage\t", CUI_White ) + "choose the project language" ) ;
	bufferize_text( color_text( "\tmove\t\t", CUI_White ) + "change the current folder" ) ;
	bufferize_text( color_text( "\tpreparation\t", CUI_White ) + "create a tree view of the project directory" ) ;
	bufferize_text( color_text( "\tquit\t\t", CUI_White ) + "quit Comcheck" ) ;
	bufferize_text( color_text( "\treport\t\t", CUI_White ) + "display the report of the current folder" ) ;
	bufferize_text( color_text( "\ttree\t\t", CUI_White ) + "display a tree view of the project folder" ) ;
	bufferize_text() ;

	display_buffer() ;
}

/*
When the user launches an analyze, the system is going to check if this report was created before
or create it if it's not.

An analyze could only be made after the call of 'preparation'.
The creation of the report is followed by the call of the 'report' command without parameter.
*/

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
		current_report_ = core_->make_report( current_folder_ ) ;
		report( { "report" } ) ;
	}
	else
	{
		bufferize_text( color_text( "The system can't find the project directory. There are three possibilities: you have not chosen a project folder with the command 'directory', you have not launch the command 'preparation' or you have but there is no file in this project folder for this language.", CUI_Red ) ) ;
		display_buffer() ;
	}

}

/*
To find the root of the tree-view, the system requires a root folder.
The user can choose the directory with this command.

Before calling 'preparation', the user can modify the directory without any
problem. But when the tree-view is created, a modification of the directory
means the destruction of the tree-view and every reports made inside.

To change the directory after calling 'preparation', the user needs to use
the parameter '-r' or '--reset' to release the tree-view and start everything
from scratch.
*/

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
			if( param_list.empty() )
			{
				// The user doesn't give a name after the command, so the system ask for one

				std::cout << "Address of project folder ? " ;
				std::getline( std::cin, tmp ) ;
				directory_name = tmp.c_str() ;
				tmp = "" ;
			}
			else
			{
				// The system will use the name given in parameter

				directory_name = param_list.front() ;
				param_list.clear() ;
			}

			/*
			If the user doesn't give a name, the system will quit the 'directory' command.
			The same thing happens if the directory exists.
			*/

			QDir test( directory_name ) ;
			if( directory_name.isEmpty() || test.exists() )
			{
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
			/*
			To be coherent, the name of the directory must not end by a slash.
			But sometimes, user could are used to write the last slash so the system
			erase this last character before saving it.
			*/

			if( directory_name.at( directory_name.size() - 1 ) == '/' )
			{
				directory_name = directory_name.left( directory_name.size() - 1 ) ;
			}

			core_->set_directory( directory_name ) ;
		}
		else
		{
			bufferize_text( color_text( "Nothing was done.", CUI_Red ) ) ;
		}
	}
	else
	{
		/*
		If the folder is already defined and the tree-view created, that means the system
		should release everything (tree-view and reports) if the user wants a new root.
		*/

		if( param_list.empty() )
		{
			bufferize_text( color_text( "The directory defines the tree view. So changing it means recreate a new tree view. If you want to do that, just use the command 'directory -r <address>' or 'directory --reset <address>'.", CUI_Red ) ) ;
		}
		else if( param_list.front() == "-r" ||
				 param_list.front() == "--reset")
		{
			core_->release_tree() ;
			current_folder_ = nullptr ;

			core_->release_reports() ;
			current_report_ = nullptr ;

			param_list.erase( param_list.begin() ) ;

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

/*
If we want to use ComCheck on several project rapidly, we can only use a console report
that will disappear at the end of the execution.

To allows user to watch report even after the end of the execution, the system can export
reports in HTML directly after an analyze.
*/

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

/*
The help command is a simple man for the system.

By just using 'help', the user can display a small resume of the use of ComCheck and
how it works.

But by adding the name of a command after 'help', the system will describe the behavior
of this command and precise every parameter that can be used with this command.
*/

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
		// Common help contains a description of ComCheck, its use, its work, ...

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
		// Specific help shows name, description and how to use specific commands.

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

/*
The 'info' command shows the evolution of an execution.

There is 4 differents steps :
Step 1 - When a directory is chosen, the system shows it.
Step 2 - When a language is chosen, the system shows it.
Step 3 - After the 'preparation' call, the system shows the current folder.
Step 4 - After the first 'analyze', the system shows the current report and
the list of folder already analyzed.
*/

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

	// jump a line
	if( !( buffer_.empty() ) )
	{
		bufferize_text() ;
	}

	if( current_folder_ != nullptr )
	{
		bufferize_text( color_text( "Current folder: ", CUI_White ) + current_folder_->name ) ;
	}

	if( current_report_ != nullptr )
	{
		bufferize_text( color_text( "Current report folder: ", CUI_White ) + current_report_->folder->name ) ;
	}

	QList< QPair< CC_Folder *, CC_Report * > > map = core_->get_map_reports() ;
	if( map.size() > 1 )
	{
		bufferize_text() ;
		bufferize_text( color_text( "List of folder already analyzed: ", CUI_White )  ) ;

		for( int i = 0 ; i < map.size() ; ++i )
		{
			bufferize_text( map[ i ].first->name ) ;
		}
	}

	display_buffer() ;
}

/*
To be able to find interesting files from the root folder, the user needs to define
the language of the source files. The choice of the language defines several precise
extension that will be searched in the name of each file.

Before calling 'preparation', the user can modify the language without any
problem. But when the tree-view is created, a modification of the language
means the destruction of the tree-view and every reports made inside.

To change the language after calling 'preparation', the user needs to use
the parameter '-r' or '--reset' to release the tree-view and start everything
from scratch.
*/

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
			if( param_list.empty() )
			{
				/*
				The user doesn't give a language after the command, so the system ask for one.
				But to be sure that plugins are correctly loaded, the system shows before the
				list of languages currently supported.
				*/

				std::cout << "The system handles:" << std::endl ;
				for( int i = 0 ; i < list.size() ; ++i )
				{
					bufferize_text( "\t" + color_text( list[ i ]->get_language(), CUI_White, false ) ) ;
				}
				display_buffer() ;

				std::cout << "Language? " ;

				std::getline( std::cin, tmp ) ;
				language_name = tmp.c_str() ;
				tmp = "" ;
			}
			else
			{
				// The system will use the language given in parameter

				language_name = param_list.front() ;
				param_list.clear() ;
			}

			/*
			If the user doesn't give a language, the system will quit the 'language' command.
			If there is a language, the system should check if a plugin is associated to this
			name and ask a new try if it doesn't.
			*/

			if( language_name.isEmpty() )
			{
				try_again = false ;
			}
			else
			{
				plugin = core_->find_plugin( language_name ) ;

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
			core_->set_plugin( plugin ) ;
		}
		else
		{
			bufferize_text( color_text( "Nothing was done.", CUI_Red ) ) ;
		}
	}
	else
	{
		/*
		If the language is already defined and the tree-view created, that means the system
		should release everything (tree-view and reports) if the user wants another language.
		*/

		if( param_list.empty() )
		{
			bufferize_text( color_text( "The language defines the tree view. So changing it means recreate a new tree view. If you want to do that, just use the command 'language -r <language_name>' or 'language --reset <language_name>'.", CUI_Red ) ) ;
		}
		else if( param_list.front() == "-r" ||
				 param_list.front() == "--reset")
		{
			current_folder_ = nullptr ;
			core_->release_tree() ;

			core_->release_reports() ;
			current_report_ = nullptr ;

			param_list.erase( param_list.begin() ) ;

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

/*
The 'move' command has two goals:
1 - shows the position of the user, the files which could be analyzed in this folder
and the list of folders reachable from this one.
2 - move the position of the user in a precise folder by adding the name of a folder
present in this folder (or the parent name) at the end of the command.
*/

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

	if( current_folder_ != nullptr )
	{
		QList< CC_Folder * >::const_iterator cit_Folder ;

		if( param_list.empty() )
		{
			/*
			Without parameter, the system will show what is inside the current folder.
			It starts by interesting files in the folder, then it shows the list of
			folder that can be reach from here.
			*/

			name = current_folder_->name + "/" ;

			bufferize_text( color_text( "Current folder: ", CUI_White ) + name ) ;
			bufferize_text() ;

			QList< CC_File * >::const_iterator cit_File ;

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

			if( !( current_folder_->list_folders.empty() ) )
			{
				bufferize_text( "You can move to: " ) ;

				for( cit_Folder = ( current_folder_->list_folders ).constBegin() ;
					 cit_Folder != ( current_folder_->list_folders ).constEnd() ;
					 ++cit_Folder )
				{
					// It is important to withdraw the slash at the end, to be coherent.

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
			/*
			To check if the parameter is a real name, the system should only care
			about the name of folders and no more the complete address.

			First of all, we create a list of all folders that could be reach in this folder.
			Then we check if the last part of the address of one of these folder (name)
			matches with the name given in parameter.
			*/

			QList< CC_Folder * > list_folders = current_folder_->list_folders ;
			if( current_folder_->parent != nullptr )
			{
				list_folders.push_front( current_folder_->parent ) ;
			}

			QString address = param_list.front() ;
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

/*
After deciding the directory and the language for its project. The system should be able
create a tree-view from this directory and sort files to be sure that only interesting
files in this tree-view.

The 'preparation' command is going to call the creation of the tree-view by the Core.
*/

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

	current_folder_ = core_->create_tree_view() ;

	if( current_folder_ == nullptr )
	{
		bufferize_text( color_text( "This project folder doesn't contain any file which match with language's extensions. Maybe you forgot to choose the language.", CUI_Red ) ) ;
	}

	display_buffer() ;
}

/*
The 'report' command display the current report. But it is possible that the folder analyzed in
the current report is not the same as the current position of the user.

To change the current report, the user need to call the 'analyze' command before.

Display a report means three possibilites :
1 - the user wants information about lines.
2 - the user wants more precise information about comments.
3 - the user wants both type of information.

By using this command, you can display information in different shapes :
1 - default information : statistics values (deviation, ...).
2 - short description for each files.
3 - histogram for comparing levels.
4 - top list that shows best and worst files.

So the user can choose a possibility for the type of data and a special way to display it by
composing the parameters (-a, -c, -l) and (-f, -t, -h).
*/

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
		if( current_report_->folder != current_folder_ )
		{
			bufferize_text( color_text( "Be careful, you are not displaying the report of the current folder.", CUI_Yellow ) ) ;
			bufferize_text( "If you want a report about the current folder, you need to use 'analyze' before." ) ;
			bufferize_text() ;
		}

		bool show_comments_info = false ;
		bool show_lines_info = false ;
		char show_what = ' ' ;

		while( !( param_list.empty() ) )
		{
			// which info
			if( param_list.front() == "-a" ||
				param_list.front() == "--all" )
			{
				show_comments_info = true ;
				show_lines_info = true ;
			}
			else if( param_list.front() == "-c" ||
					 param_list.front() == "--comments" )
			{
				show_comments_info = true ;
			}
			else if( param_list.front() == "-l" ||
					 param_list.front() == "--lines" )
			{
				show_lines_info = true ;
			}
			// which representation
			else if( param_list.front() == "-f" ||
					 param_list.front() == "--files" )
			{
				show_what = 'f' ;
			}
			else if( param_list.front() == "-h" ||
					 param_list.front() == "--histogram" )
			{
				show_what = 'h' ;
			}
			else if( param_list.front() == "-t" ||
					 param_list.front() == "--top" )
			{
				show_what = 't' ;
			}
			else
			{
				// TODO display error
			}

			param_list.erase( param_list.begin() ) ;
		}

		if( show_comments_info == false && show_lines_info == false )
		{
			show_lines_info = true ; // by default, the system shows line information
		}


		bufferize_text( color_text( "Report folder: ", CUI_White ) + color_text( current_report_->folder->name, CUI_Blue ) ) ;
		bufferize_text( color_text( "Number of files: ", CUI_White ) + QString::number( current_report_->list_files.size() ) ) ;
		bufferize_text() ;

		switch( show_what )
		{
			case ' ' :
			{
				if( show_lines_info )
				{
					display_array_type( current_report_->type ) ;
					display_statistics( current_report_->percentage ) ;
				}

				if( show_comments_info )
				{
					display_array_description( current_report_->description ) ;
					display_statistics( current_report_->coverage ) ;
				}
			} break ;

			case 'f' :
			{
				CC_File * file = nullptr ;
				CUI_TextColor tc = CUI_Yellow ;
				double threshold = 0 ;
				double value = 0 ;
				QString line = "" ;

				QList< CC_File * >::const_iterator cit ;
				for( cit = current_report_->list_files.constBegin() ;
					 cit != current_report_->list_files.constEnd() ;
					 ++cit )
				{
					line = "" ;
					file = ( *cit ) ;

					if( show_lines_info )
					{
						tc = CUI_Yellow ;

						threshold = current_report_->percentage.average ;
						value = file->percentage ;

						// change the color
						if( value < 0.8 * threshold )
						{
							tc = CUI_Red ;
						}
						else if( value > 1.2 * threshold )
						{
							// good file
							tc = CUI_Green ;
						}

						// display its information
						line = color_text( file->name, tc ) ;
						line += " " + color_text( "% ", CUI_White ) + QString::number( value ) ;
						line += " " + display_array_type( file->type, true ) ;

						bufferize_text( line ) ;
					}

					if( show_comments_info )
					{
						tc = CUI_Yellow ;

						threshold = current_report_->coverage.average ;
						value = file->coverage.average ;

						// change the color
						if( value < 0.8 * threshold )
						{
							// bad file
							tc = CUI_Red ;
						}
						else if( value > 1.2 * threshold )
						{
							// good file
							tc = CUI_Green ;
						}

						// display its information
						line = color_text( file->name, tc ) ;
						line += " " + color_text( "% ", CUI_White ) + QString::number( value ) ;
						line += " " + display_array_description( file->description, true ) ;

						bufferize_text( line ) ;
					}
				}

				bufferize_text() ;
			} break ;

			case 'h' :
			{
				// display a histogram
				draw_histogram( show_lines_info, show_comments_info ) ;
				bufferize_text() ;
			} break ;

			case 't' :
			{
				CC_File * file = nullptr ;
				CUI_TextColor tc = CUI_Yellow ;
				double value = 0 ;
				QList< QPair< double, CC_File * > > sorted_list ;
				QString line = "" ;
				unsigned int number = 5 ;

				bufferize_text( "By default, it display the top 5 of best and worst file" ) ;
				bufferize_text() ;

				for( int i = 0 ; i < current_report_->list_files.size() ; ++i )
				{
					file = current_report_->list_files[ i ] ;
					value = 0 ;

					if( show_lines_info )
					{
						value = file->percentage ;
					}
					else
					{
						value = file->coverage.average ;
					}

					sorted_list.push_back( qMakePair( value, file ) ) ;
				}

				std::sort( sorted_list.begin(), sorted_list.end() ) ;

				if( show_lines_info )
				{
					bufferize_text( "Best and worst files: ratio comments / lines" ) ;
				}
				else
				{
					bufferize_text( "Best and worst files: coverage" ) ;
				}

				for( unsigned i = 0 ; i < number ; ++i )
				{
					value = sorted_list[ i ].first ;
					file = sorted_list[ i ].second ;

					tc = CUI_Red ;
					line = color_text( file->name, tc ) ;

					// display its information

					if( show_lines_info )
					{
						line += " " + color_text( "% ", CUI_White ) + QString::number( value ) ;
						line += " " + display_array_type( file->type, true ) ;
					}
					else
					{
						line += " " + color_text( "% ", CUI_White ) + QString::number( value ) ;
						line += " " + display_array_description( file->description, true ) ;
					}

					bufferize_text( line ) ;
				}

				bufferize_text() ;

				for( unsigned i = 0 ; i < number ; ++i )
				{
					value = sorted_list[ sorted_list.size() - 1 - i ].first ;
					file = sorted_list[ sorted_list.size() - 1 - i ].second ;

					tc = CUI_Green ;
					line = color_text( file->name, tc ) ;

					// display its information

					if( show_lines_info )
					{
						line += " " + color_text( "% ", CUI_White ) + QString::number( value ) ;
						line += " " + display_array_type( file->type, true ) ;
					}
					else
					{
						line += " " + color_text( "% ", CUI_White ) + QString::number( value ) ;
						line += " " + display_array_description( file->description, true ) ;
					}

					bufferize_text( line ) ;
				}

				bufferize_text() ;

				if( show_lines_info && show_comments_info )
				{
					report( { "report", "-c", "-t" } ) ;
				}
			} break ;

			default :
				// TODO display error
				break ;
		}
	}
	else
	{
		bufferize_text( color_text( "The system can't find a report for this position. You need to use 'analyze' to create the report before using 'report'.", CUI_Red ) ) ;
	}

	display_buffer() ;
}

/*
To display the whole contain of the tree-view, the user can use the command 'tree'.
It works exactly like the shell command 'tree' but shows only interesting file sorted
thanks to the language.
*/

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
		bufferize_text( color_text( current_folder_->name, CUI_Blue ) ) ;
		display_tree( current_folder_ ) ;
	}
	else
	{
		bufferize_text( color_text( "The system can't find the project directory. There are three possibilities: you have not chosen a project folder with the command 'directory', you have not launch the command 'preparation' or you have but there is no file in this project folder for this language.", CUI_Red ) ) ;

	}

	display_buffer() ;
}
