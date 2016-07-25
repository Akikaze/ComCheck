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
, step_( 0 )
{
	load_plugins() ;
}

// --- DESTRUCTORS ---

Core::~Core
()
{
	reset() ;
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
Core::analyze
()
{
	std::array< unsigned int, 4 > counters ;
	unsigned short tmp = 0 ;
	std::ifstream stream ;
	std::string line ;
	std::string name ;
	std::vector< File * >::const_iterator cit ;
	unsigned count = 0 ;
	
	for( cit = files_.cbegin() ; cit != files_.cend() ; ++cit )
	{
		name = ( *cit )->get_name() ;
		stream.open( name, std::ios::in ) ;
		
		if( stream )
		{
			counters = { 0, 0, 0, 0 } ;
			
			while( getline( stream, line ) )
			{
				clear_line( line ) ; // remove all useless characters
				
				if( !( line.empty() ) )
				{
					tmp = chosen_plugin_->analyze( line ) ; // get back information about the line
					
					counters[ 0 ]++ ; // add a line
					
					if( tmp > 0 && tmp < 4 )
					{
						counters[ tmp ]++ ; // add info about the line
					}
				}
			}
			
			stream.close() ;
			( *cit )->store_info( counters ) ; // set info in the File object
		}
		else
		{
			std::cerr << "Error loading file : " << name << std::endl ;
		}
		
		++count ;
	}
}

void
Core::clear_line
(
	std::string & line
)
{
	unsigned int i = 0 ;
	unsigned int length = line.length() ;
	
	while( i < length )
	{
		if( int( line[ i ] ) == 9 || // tab
			int( line[ i ] ) == 10 || // new line
			int( line[ i ] ) == 32 ) // ' '
		{
			line.erase( line.begin() + i ) ;
			--length ;
		}
		else
		{
			++i ;
		}
	}
}

std::vector< std::string >
Core::list_files
()
{
	std::vector< std::string > result ;
	std::vector< std::string > list_directories ;
	list_directories.push_back( directory_ ) ;
	
	DIR * directory ;
	unsigned char isDirectory = 0x4 ;
	unsigned char isFile = 0x8 ;
	struct dirent * ent ;
	
	while( !( list_directories.empty() ) )
	{
		directory = opendir( list_directories[ 0 ].c_str() ) ;
		
		while( ( ent = readdir( directory ) ) != NULL )
		{			
			if( ent->d_type == isDirectory &&
				strcmp( ent->d_name, "." ) != 0 &&
				strcmp( ent->d_name, ".." ) != 0 )
			{
				list_directories.push_back( list_directories[ 0 ] + "/" + ent->d_name ) ;
			}
			else if( ent->d_type == isFile )
			{
				std::string name = ent->d_name ;
				size_t position = name.find_last_of( "." ) ;
				
				if( position != name.npos )
				{
					name = name.substr( position ) ;
					std::vector< std::string > extensions = chosen_plugin_->get_extensions() ;
					
					if( std::find( extensions.begin(), extensions.end(), name )  != extensions.end() )
					{
						result.push_back( list_directories[ 0 ] + "/" + ent->d_name ) ;
					}
				}
			}
		}
		
		closedir( directory ) ;
		list_directories.erase( list_directories.begin() ) ;
	}
	
	return result ;
}

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
		
		std::cout << "===== ComCheck =====" << std::endl ;
		
		std::cout << std::endl << "> " ;
		std::cin >> choice ;
		std::cout << std::endl ;
		
		if( choice == "analyze" )
		{
			if( step_ >= 3 )
			{
				std::cout << "ANALYZING" << std::endl ;
				analyze() ;
				step_ = 4 ;
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
					step_ = 2 ;
				}
			}
			else
			{
				std::cout << "You can't change the language after having made the preparation." << std::endl ;
			}
		}
		else if( choice == "directory" )
		{
			if( step_ < 3 )
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
				prepare() ;
				step_ = 3 ;
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
				report() ;
			}
			else
			{
				std::cout << "You need to launch the analyze before." << std::endl ;
			}
		}
		else if( choice == "reset" )
		{
			reset() ;			
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
Core::prepare
()
{
	std::vector< std::string > list = list_files() ;
	std::vector< std::string >::const_iterator cit ;
	unsigned int count = 0 ;
	
	for( cit = list.cbegin() ; cit != list.cend() ; ++cit )
	{
		files_.push_back( new File( *cit ) ) ;
		++count ;
	}
}

void
Core::report
()
{
	std::vector< File * >::const_iterator cit ;
	std::array< unsigned int, 4 > counters = { 0, 0, 0, 0 } ;
	
	for( cit = files_.cbegin() ; cit != files_.cend() ; ++cit )
	{
		std::cout << ( *cit )->get_name() << " : " << std::endl ;
		std::cout << ( *cit )->get_report() << std::endl ;
		counters += ( *cit )->get_report() ;
	}
	
	std::cout << std::endl ;
	std::cout << "===== REPORT =====" << std::endl ;
	std::cout << "Lines : " << counters[ 0 ] << std::endl ;
	std::cout << "Commented : " << counters[ 1 ] << std::endl ;
	std::cout << "Mixed : " << counters[ 2 ] << std::endl ;
	std::cout << "Uncommented : " << counters[ 3 ] << std::endl ;
	std::cout << std::endl ;
}

void
Core::reset
()
{
	directory_ = "" ;
	chosen_plugin_ = NULL ;
	
	while( files_.begin() != files_.end() )
	{
		delete files_[ 0 ] ;
		files_.erase( files_.begin() ) ;
	}
	
	step_ = 0 ;
}

void
Core::unload_plugins
()
{
	for( std::vector< Plugin * >::iterator it = plugins_.begin() ;
		 it != plugins_.end() ; ++it )
	{		
		delete *it ;
	}
}
