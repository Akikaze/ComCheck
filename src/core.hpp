///
/// \file core.hpp
/// \brief Header for core system
/// \author David Feraud
/// \version 1
///

#ifndef CORE_HPP
#define CORE_HPP

#include <cstring>
#include <dirent.h>

#include "file.hpp"
#include "plugin.hpp"

/*
 * Core's step :
 * 0 -> start step
 * 1 -> directory set
 * 2 -> language set
 * 3 -> preparation done
 * 4 -> analyze done
 */

class Core
{
	public :
	
		// --- DESTRUCTORS ---
		///
		/// \brief Destructor
		///
		~Core() ;
		
		// --- GETTERS ---
		///
		/// \brief Getter for instance
		/// \return Single instance of Core
		///
		static Core * get_instance() ;
		
		// --- SETTERS ---
		///
		/// \brief Setter for project's directory
		/// \param address Address of project's directory
		///
		inline void set_directory( const std::string & address )
			{ directory_ = address ; }

		// --- METHODS ---
		///
		/// \brief Loop for text mode
		///
		void loop() ;

		///
		/// \brief Prepare the analyze
		///
		/// Looking for every files matching plugin's extensions and create a File object for each file.
		///
		void prepare() ;

	protected :
	
		// --- CONSTRUCTORS ---
		///
		/// \brief Constructor
		///
		Core() ;
		
		// --- METHODS ---
		///
		/// \brief Load plugins
		///
		void load_plugins() ;

		///
		/// \brief Unload plugins
		///
		void unload_plugins() ;
		
		// --- ATTRIBUTES ---
		Plugin * chosen_plugin_ ; ///< Index of the chosen plugin
		std::string directory_ ; ///< Address of project's repository
		std::vector< File * > files_ ; ///< List of files
		std::vector< Plugin * > plugins_ ; ///< List of plugins
		unsigned short step_ ; ///< Step of the analyze (cf comments in introduction)
		
		// --- STATIC ATTRIBUTES ---
		static Core * _instance_ ; ///< Single instance of Core		
} ;

#endif // CORE_HPP
