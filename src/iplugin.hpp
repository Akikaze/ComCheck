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
		virtual ~IPlugin() = 0 ;
		
		// --- GETTERS ---
		///
		/// \brief Getter for extensions
		/// \return List of extensions for plugin's language
		///
		/// Return a list of extensions in a vector of string : { .c , .h , ... }.
		///
		virtual std::vector< std::string > get_extensions() = 0 ;
		
		///
		/// \brief Getter for language
		/// \return Language
		///
		/// Return the language in order to be known by the system.
		///
		virtual inline std::string get_language() = 0 ;
		
		///
		/// \brief Getter for rank
		/// \return Rank
		///
		/// Return a rank in order to sort each plugin
		///
		virtual inline unsigned short get_rank()
			{ return USHRT_MAX - 1 ; }
		
		// --- METHODS ---
		///
		/// \brief Analyze function
		/// \param filename Name of file ready for analyze
		/// \return Array which contains number of non-empty lines, commented lines, mixed lines and uncommented lines
		///
		/// This function make the magic part : it scans the file by counting the number of lines.
		/// For each line, it figures out if the line contains comments or not.
		/// At the end, it returns each type of comments in an array.
		///
		virtual std::array< unsigned int, 4 > analyze( const std::string & filename ) = 0 ;
} ;

#endif // IPLUGIN_HPP