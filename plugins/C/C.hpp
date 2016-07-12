#ifndef C_HPP
#define C_HPP

#include <iostream>
#include <string>

#include "../../src/iplugin.hpp"

class C
: public IPlugin
{
	public :
		
		// --- CONSTRUCTORS ---
		///
		/// \brief Constructor
		///
		C() ;
		
		// --- DESTRUCTORS ---
		///
		/// \brief Destructor
		///
		virtual ~C() ;
		
		// --- GETTERS ---
		///
		/// \brief Getter for extensions
		/// \return List of extensions for plugin's language
		///
		inline virtual const std::vector< std::string > get_extensions() const
			{ std::vector< std::string > extensions = { ".c", ".h" } ;
			  return extensions ; }
		
		///
		/// \brief Getter for language
		/// \return Language
		///
		/// Return the language in order to be known by the system.
		///
		inline virtual const std::string get_language() const
			{ return "C" ; }
		
		// --- METHODS ---
		///
		/// \brief Analyze function
		/// \param line Line for the analyze
		/// \return 1 if the line is commented, 2 if it's a mix and 3 if it is uncommented
		///
		/// This function make the magic part : it scans the File object by counting the number of lines.
		/// For each line, it figures out if the line contains comments or not.
		/// At the end, it stores the data in the File object.
		///
		virtual unsigned short analyze( const std::string & line ) ;
	
	protected :
	
		// --- METHODS ---
		///
		/// \brief Return an iterator on a position return by find_first_of
		/// \param line Line which is going to call find_first_of
		/// \param string String parameter for the find_first_of call
		/// \return String iterator
		///
		std::string::iterator ffo_iterator( std::string & line, const std::string & string ) const ;
	
		// --- ATTRIBUTES ---
		bool comment_started ;
} ;

#endif // C_HPP
