///
/// \file core.cpp
/// \brief Source code for core system
/// \author David Feraud
/// \version 1
///

#include "core.hpp"

// --- STATIC ATTRIBUTES ---

Core * Core::_instance_ = NULL ;

// --- CONSTRUCTORS ---

Core::Core
()
: chosen_plugin_( NULL )
, directory_( "" )
{
	load_plugins() ;
}

// --- DESTRUCTORS ---

Core::~Core
()
{
	unload_plugins() ;
}

// --- GETTERS ---

Core *
Core::get_instance
()
{
	if( _instance_ == NULL )
	{
		_instance_ = new Core() ;
	}
	
	return _instance_ ;
}

// --- METHODS ---

void
Core::load_plugins
()
{
	unsigned char isDirectory = 0x4 ;
	std::string address = "./plugins" ;
	
	std::vector< std::string > list_directories ;
	DIR * directory = opendir( address.c_str() ) ;
	struct dirent * ent ;
	
	// List all directories
	if( directory != NULL )
	{
		while( ( ent = readdir( directory ) ) != NULL )
		{
			if( strcmp( ent->d_name, "." ) != 0 &&
				strcmp( ent->d_name, ".." ) != 0 &&
				ent->d_type == isDirectory )
			{
				list_directories.push_back( address + "/" + ent->d_name ) ;
			}
		}
		
		closedir( directory ) ;
	}
	else
	{
		std::cerr << "Error when loading plugins directory" << std::endl ;
	}
	
	// Create Plugin objects for each directory found
	for( std::vector< std::string >::const_iterator cit = list_directories.cbegin() ;
		 cit != list_directories.cend() ; ++cit )
	{
		plugins_.push_back( new Plugin( *cit ) ) ;
	}
	
	// Sort plugins by their rank
	std::sort( plugins_.begin(), plugins_.end(), PointerComparator() ) ;
}

void
Core::loop
()
{
	std::string choice = "" ;
	std::string value = "" ;

	while( choice != "quit" )
	{
		std::cout << std::endl ;
		//system( "clear" ) ;
		
		std::cout << "===== ComCheck =====" << std::endl ;
		
		std::cout << std::endl << "> " ;
		std::cin >> choice ;
		std::cout << std::endl ;
		
		if( choice == "analyze" )
		{
			if( step_ >= 3 )
			{
				std::cout << "ANALYZING" << std::endl ;
			}
			else
			{
				std::cout << "You need to launch the preparation before." << std::endl ;
			}
		}
		else if( choice == "choose" )
		{
			if( step_ == 1 )
			{
				std::cout << "Which language will be used for the analyze ?" << std::endl ;
				
				std::cout << std::endl << "> " ;
				std::cin >> value ;
				
				std::vector< Plugin * >::iterator it ;
				it = std::find_if( plugins_.begin(), plugins_.end(),
								   [&value] (Plugin * p) -> bool { return ( p->get_language() == value ) ; } ) ;
				
				if( it == plugins_.end() )
				{
					std::cout << "The language '" << value << "' is not supported." << std::endl ;
				}
				else
				{
					chosen_plugin_ = *it ;
				}
			}
			else
			{
				std::cout << "You can't change the language after having made the preparation." << std::endl ;
			}
		}
		else if( choice == "directory" )
		{
			if( step_ == 0 )
			{
				if( directory_ != "" )
				{
					std::cout << "You have already set the directory to : '" << directory_ << "'" << std::endl ;
				}
				else
				{
					std::cout << "Which directory will be analyzed ?" << std::endl ;
					
					std::cout << std::endl << "> " ;
					std::cin >> value ;
					
					if( value != "" )
					{
						set_directory( value ) ;
						step_ = 1 ;
					}
				}
			}
			else
			{
				std::cout << "It is useless to change the directory after having made the preparation." << std::endl ;
			}
		}
		else if( choice == "help" )
		{
			std::cout << "analyze : Launch the analyze." << std::endl ;
			std::cout << "choose : Choose a language for the analyze." << std::endl ;
			std::cout << "directory : Set the project for the analyze." << std::endl ;
			std::cout << "help : Display the list of commands." << std::endl ;
			std::cout << "info : Display information about the project." << std::endl ;
			std::cout << "list : List all languages supported." << std::endl ;
			std::cout << "prepare : Prepare the analyze." << std::endl ;
			std::cout << "report : Display the report after the analyze." << std::endl ;
			std::cout << "reset : Reset the directory and the whole analyze." << std::endl ;
			std::cout << "quit : Quit the program." << std::endl ;
		}
		else if( choice == "info" )
		{
			if( step_ >= 1 )
			{
				std::cout << "Directory : " << directory_ << std::endl ;
			}
			
			if( step_ >= 2 )
			{
				std::cout << "Language : " << chosen_plugin_->get_language() << std::endl ;
			}
			
			if( step_ >= 3 )
			{
				std::cout << "Preparation : " << files_.size() << " files counted" << std::endl ;				
			}
			
			if( step_ >= 4 )
			{
				std::cout << "Analyze : done" << std::endl ;
			}
		}
		else if( choice == "list" )
		{
			std::cout << "List of all languages supported :" << std::endl ;
			std::cout << plugins_ << std::endl ;
		}
		else if( choice == "prepare" )
		{
			if( step_ >= 2 )
			{
				std::cout << "PREPARING" << std::endl ;
			}
			else
			{
				std::cout << "You need to set the directory and the language before." << std::endl ;
			}
		}
		else if( choice == "report" )
		{
			if( step_ >= 4 )
			{
				std::cout << "REPORTING" << std::endl ;
			}
			else
			{
				std::cout << "You need to launch the analyze before." << std::endl ;
			}
		}
		else if( choice == "reset" )
		{
			directory_ = "" ;
			chosen_plugin_ = NULL ;
			step_ = 0 ;
			
			std::cout << "===== ######## =====" << std::endl ;
		}
		else if( choice != "quit" ) // last choice
		{
			std::cout << "This command is not recognized." << std::endl ;
			std::cout << "ask 'help' if you want information about your options." << std::endl ;
		}
	}
}

void
Core::unload_plugins
()
{
	while( plugins_.begin() != plugins_.end() )
	{
		delete plugins_[ 0 ] ;
	}
}
