///
/// \file file.hpp
/// \brief Header for file object
/// \author David Feraud
/// \version 1
///

#ifndef FILE_HPP
#define FILE_HPP

#include "std.hpp"

class File
{
    public :

        // --- CONSTRUCTORS ---
        
        ///
        /// \brief Constructor
        /// \param name Name of the file
        ///
        File( const std::string & name ) ;

        // --- DESTRUCTORS ---
        ///
        /// \brief Destructor
        ///
        ~File() ;

        // --- GETTERS / SETTERS ---
        ///
        /// \brief Getter on the name
        ///
        std::string get_name()
            { return name_ ; }

        ///
        /// \brief Getter All lines info
        /// \return Array which contains number of lines, commented lines, mixed lines and uncommented lines
        ///
        std::array< unsigned int, 4 > get_report()
            { std::array< unsigned int, 4 > output = { all_lines_, nbr_comments_, nbr_mix_, nbr_uncomments_ } ;
              return output ; }

        // --- METHODS ---
        ///
        /// \brief Store every info about the file
        /// \param all_lines Number of lines in the file
        /// \param nbr_comments Number of commented lines
        /// \param nbr_mix Number of mixed lines
        /// \param nbr_uncomments Number of uncommented lines
        ///
        void store_info( const unsigned int & all_lines,
                         const unsigned int & nbr_comments,
                         const unsigned int & nbr_mix,
                         const unsigned int & nbr_uncomments ) ;

    protected :
		
		// --- CONSTRUCTORS ---
		///
		/// \brief Constructor
		///
		File() ;

        // --- ATTRIBUTES ---
        std::string name_ ; ///< Name of the file
        unsigned int all_lines_ ; ///< Number of non-empty lines in the file
        unsigned int nbr_comments_ ; ///< Number of commented lines in the file
        unsigned int nbr_mix_ ; ///< Number of mixed lines in the file (line with code and comment)
        unsigned int nbr_uncomments_ ; ///< Numnber of uncommented lines in the file
} ;

#endif // FILE_HPP
