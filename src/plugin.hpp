///
/// \file plugin.hpp
/// \brief Header for plugin facade
/// \author David Feraud
/// \version 1
///

#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <dlfcn.h>

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
		inline std::string get_adress() const
			{ return address_ ; }
		
		///
		/// \brief Getter for extensions
		/// \return List of extensions for plugin's language
		///
		virtual std::vector< std::string > get_extensions()
			{ return extensions_ ; }
		
		///
		/// \brief Getter for language
		/// \return Language analyzed by the plugin
		///
		virtual inline std::string get_language()
			{ return language_ ; }
		
		///
		/// \brief Getter for rank
		/// \return Rank of the plugin
		///
		virtual inline unsigned short get_rank()
			{ return rank_ ; }
		
		// --- METHODS ---
		///
		/// \brief Analyze function
		/// \param filename Name of file ready for analyze
		/// \return Array which contains number of non-empty lines, commented lines, mixed lines and uncommented lines
		///
		virtual std::array< unsigned int, 4 > analyze( const std::string & filename ) ;
		
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
		IPlugin * pointer_ ;
		std::string address_ ;
		std::string language_ ;
		std::vector< std::string > extensions_ ;
		unsigned short rank_ ;
		void * lib_descriptor_ ;
} ;

#endif // PLUGIN_HPP