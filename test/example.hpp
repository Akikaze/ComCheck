/* HEAD head comment on
two lines */

#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

// --- EXAMPLE ---

class Example
{
	public :
	
		// --- CONSTRUCTORS ---
		
		///
		/// \brief Default constructor
		///
		Example() ;
		
		// --- DESTRUCTORS ---
		
		///
		/// \brief Destructor
		///
		~Example() ;
	
	protected :
	
		// --- ATTRIBUTES ---
		
		bool attribute_1_ ; ///< first attribute
		unsigned int attribute_2_ ; ///< second attribute	
} ;

#endif

// TEMP this document contains 39 lines but only 27 are real lines ( 15 comments line, 2 mixed )
/* BUGS there are 2 head comments, 4 useless comments, 8 doc comments
TODO and of course, 1 temp comment, 1 bugs comment and 1 todo comments */
