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
		/// \brief Setter for project's repository
		/// \param address Address of project's repository
		///
		inline void set_repository( const std::string & address )
			{ repository_ = address ; }
		
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
		
		// --- ATTRIBUTES ---
		std::string repository_ ; ///< Address of project's repository
		std::vector< Plugin * > plugins_ ; ///< List of plugins
		
		// --- STATIC ATTRIBUTES ---
		static Core * _instance_ ; ///< Single instance of Core		
} ;

#endif // CORE_HPP