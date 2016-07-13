///
/// \file plugin.hpp
/// \brief Header for plugin facade
/// \author David Feraud
/// \version 1
///

#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <dlfcn.h>

#include "file.hpp"
#include "iplugin.hpp"
#include "std.hpp"

class Plugin
: public IPlugin
{
	public :
		
		// --- CONSTRUCTORS ---
		///
		/// \brief Constructor
		/// \param address Address of plugin's directory
		///
		Plugin( const std::string & address ) ;
		
		// --- DESTRUCTORS ---
		///
		/// \brief Destructor
		///
		~Plugin() ;
		
		// --- GETTERS ---
		///
		/// \brief Getter for address
		/// \return Address of plugin's directory
		///
		inline const std::string get_adress() const
			{ return address_ ; }
		
		///
		/// \brief Getter for extensions
		/// \return List of extensions for plugin's language
		///
		virtual const std::vector< std::string > get_extensions() const
			{ return extensions_ ; }
		
		///
		/// \brief Getter for language
		/// \return Language analyzed by the plugin
		///
		virtual inline const std::string get_language() const
			{ return language_ ; }
		
		///
		/// \brief Getter for rank
		/// \return Rank of the plugin
		///
		virtual inline const unsigned short get_rank() const
			{ return rank_ ; }
		
		// --- METHODS ---
		
		///
		/// \brief Analyze function from IPlugin
		/// \param line Line for the analyze
		/// \return 1 if the line is commented, 2 if it's a mix and 3 if it is uncommented
		///
		virtual unsigned short analyze( const std::string & line ) ;
		
		///
		/// \brief Tell if the plugin is correctly load
		/// \return True if the loading was successful, false otherwise
		///
		inline bool is_loaded() const
			{ return ( pointer_ != NULL ) ; }
		
		// --- OPERATORS ---
		///
		/// \brief Operator <
		/// \param plugin Plugin for the comparison
		/// \return true if the rank of this plugin is less than the parameter's rank
		///
		inline bool operator<( const Plugin & plugin ) const
			{ return ( rank_ < plugin.get_rank() ) ; }
				
	protected :
		
		// --- CONSTRUCTORS ---
		///
		/// \brief Constructor
		///
		Plugin() ;
		
		// --- METHODS ---
		///
		/// \brief Find the library path thanks to the folder's address
		/// \param address Address of plugin's folder
		/// \return Address of the library
		///
		std::string find_library( const std::string & ) const ;
	
		// --- ATTRIBUTES ---
		IPlugin * pointer_ ; ///< Pointer on the plugin
		std::string address_ ; ///< Address of the plugin directory
		std::string language_ ; ///< Language of the plugin
		std::vector< File * > files_ ; ///< List of File objects
		std::vector< std::string > extensions_ ; ///< List of all extensions for the plugin's language
		unsigned short rank_ ; ///< Rank of the plugin
		void * lib_descriptor_ ; ///< Descriptor for the plugin
} ;


///
/// \brief Operator <<
/// \param os Output stream
/// \param p Pointer on a plugin
/// \return Stream with plugin language in it
///
inline std::ostream & operator<<( std::ostream & os, const Plugin * p )
	{ os << p->get_language() ;
	  return os ; }

#endif // PLUGIN_HPP
