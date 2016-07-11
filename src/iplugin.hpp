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
		///
		/// This function make the magic part : it scans the File object by counting the number of lines.
		/// For each line, it figures out if the line contains comments or not.
		/// At the end, it stores the data in the File object.
		///
		virtual std::array< unsigned int, 4 > analyze( const std::string & line ) const = 0 ;
} ;

#endif // IPLUGIN_HPP
