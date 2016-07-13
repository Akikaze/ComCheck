///
/// \file iplugin.hpp
/// \brief Interface for each plugin
/// \author David Feraud
/// \version 1
///

#ifndef IPLUGIN_HPP
#define IPLUGIN_HPP

#include <climits>

#include "std.hpp"

///
/// \class IPlugin
/// \brief Interface for each plugin
///
class IPlugin
{
	public :
	
		// --- CONSTRUCTORS ---
		///
		/// \brief Constructor
		///
		IPlugin() ;
		
		// --- DESTRUCTORS ---
		///
		/// \brief Destructor
		///
		virtual ~IPlugin() ;
		
		// --- GETTERS ---
		///
		/// \brief Getter for extensions
		/// \return List of extensions for plugin's language
		///
		/// Return a list of extensions in a vector of string : { .c , .h , ... }.
		///
		virtual const std::vector< std::string > get_extensions() const = 0 ;
		
		///
		/// \brief Getter for language
		/// \return Language
		///
		/// Return the language in order to be known by the system.
		///
		virtual const std::string get_language() const = 0 ;
		
		///
		/// \brief Getter for rank
		/// \return Rank
		///
		/// Return a rank in order to sort each plugin
		///
		virtual inline const unsigned short get_rank() const
			{ return USHRT_MAX - 1 ; }
		
		// --- METHODS ---
		///
		/// \brief Analyze function
		/// \param file Reference on a File object
		/// \return 1 if the line is commented, 2 if it's a mix and 3 if it is uncommented
		///
		/// This method make the magic part : it analyzes the line in order
		/// to understand if it's a commented line, a mixed line or a uncommented
		/// line. It returns 1 if it is commented, 2 if it is mixed and 3 if 
		/// it is not commented.
		///
		virtual unsigned short analyze( const std::string & line ) = 0 ;
} ;

#endif // IPLUGIN_HPP
