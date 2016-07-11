#ifndef C_HPP
#define C_HPP

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
		/// \return Array which contains number of lines, commented lines, mixed lines and uncommented lines
		///
		/// This function make the magic part : it scans the File object by counting the number of lines.
		/// For each line, it figures out if the line contains comments or not.
		/// At the end, it stores the data in the File object.
		///
		virtual std::array< unsigned int, 4 > analyze( const std::string & line ) const ;
} ;

#endif // C_HPP
