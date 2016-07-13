///
/// \file plugin.cpp
/// \brief Source code for plugin facade
/// \author David Feraud
/// \version 1
///

#include "plugin.hpp"

// --- CONSTRUCTORS ---

Plugin::Plugin
()
{
	
}

Plugin::Plugin
( 
	const std::string & address
)
{
	address_ = address ;
	pointer_ = NULL ;
	
	extensions_ = std::vector< std::string >() ;
	language_ = "" ;
	rank_ = USHRT_MAX ;
	
	char * error ;
	
	// open the shared library
	lib_descriptor_ = dlopen( find_library( address ).c_str(), RTLD_LAZY ) ;
	if ( ( error = dlerror() ) == NULL )
	{
		// find the load function
		IPlugin * ( * load )() = ( IPlugin * ( * )() ) dlsym ( lib_descriptor_, "load" ) ;
		
		if ( ( error = dlerror() ) == NULL )
		{
			// create the pointer to the plugin
			pointer_ = ( IPlugin * ) load() ;
			
			// find plugin's information
			extensions_ = pointer_->get_extensions() ;
			language_ = pointer_->get_language() ;
			rank_ = pointer_->get_rank() ;
		}
		else
		{
			std::cout << error << std::endl ;
		}
	}
	else
	{
		std::cout << error << std::endl ;
	}
}

// --- DESTRUCTORS ---

Plugin::~Plugin
()
{	
	if( pointer_ != NULL )
	{
		// release the plugin
		delete pointer_ ;
	}
	
	if( lib_descriptor_ )
	{
		// end the read of the shared library
		dlclose( lib_descriptor_ ) ;
	}
}

// --- METHODS ---

unsigned short
Plugin::analyze
(
	const std::string & line
)
{
	return pointer_->analyze( line ) ;
}

std::string
Plugin::find_library
(
	const std::string & address
)
const
{
	size_t last_slash = 0 ;
	std::string library ;
	std::string::const_reverse_iterator crit = address.crbegin() ;
	
	// try to find the last / in order to get only shared library's folder's name
	while( crit != address.crend() && *crit != '/' )
	{
		++last_slash ;
		++crit ;
	}
	last_slash = address.length() - last_slash ;
	
	// create the path to the library by adding the folder's name and .so
	library = address + "/" + address.substr( last_slash, address.length() - last_slash ) + ".so" ;
	
	return library ;
}
