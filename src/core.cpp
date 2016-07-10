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
				strcmp( ent->d_name, ".." ) != 0  )
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

	while( choice != "quit" )
	{
		std::cin >> choice ;
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
