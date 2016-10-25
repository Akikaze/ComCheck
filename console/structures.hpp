#ifndef CC_STRUCTURE_HPP
#define CC_STRUCTURE_HPP

#include "qt.hpp"
#include "std.hpp"

// ===== ENUMS =====

///
/// \brief Type of a line
///
/// In a file or a report, the value 0 is the number of line but when a plugin
/// return 0, that signals an error
///
enum CC_Type
{
	// Binary description : comment & code
	ERROR = 0b00,
	NO_COMMENT = 0b01,
	ONLY_COMMENT = 0b10,
	MIX_LINE = 0b11,

	type_size
} ;

///
/// \brief Description of a comment
///
/// UNDEFINED should never appeared in a code. It is only used for initialize
/// a CC_Comment without using a real value.
///
enum CC_Desc
{
	// Binary description : (programmation comment) & (documentation) & !(professional use)
	UNDEFINED = 0b000,		// nothing => used for the initialization
	USELESS = 0b001,
	HEADER = 0b010,
	DOCUMENTATION = 0b011,
	TEMPORARY = 0b100,
	PROBLEM = 0b101,
	EVOLUTION = 0b110,
	NORMAL = 0b111,

	desc_size
} ;

// ===== STRUCTURES =====
// --- CC_Statistics ---

///
/// \brief Statistical value extract from a list
///
/// Definition of an average, a divergence, a variance and a median
///
struct CC_Statistics
{
	double average ;
	double variance ;
	double divergence ;

	double max ;
	double median ;
	double min ;

	// constructors
	CC_Statistics() {}
	CC_Statistics( QList< double > list )
	{
		/*
		 * Firstly, it is going to make a first loop to find min, max, size and average.
		 * Then, the median value will be found with the size.
		 * At the end, a second loop will give the variance and divergence.
		 */

		if( !( list.empty() ) )
		{
			unsigned int size = 1 ;
			double value = 0 ;

			QList< double >::iterator it = list.begin() ;

			// min, max, size and average

			value = ( *it ) ;
			average = value ;
			max = value ;
			min = value ;

			++it ;

			while( it != list.end() )
			{
				value = ( *it ) ;
				average += value ;

				if( max < value )
				{
					max = value ;
				}

				if( min > value )
				{
					min = value ;
				}

				++size ;
				++it ;
			}

			average /= ( double )( size ) ;

			// variance and divergence

			it = list.begin() ;
			variance = 0 ;

			while( it != list.end() )
			{
				value = ( *it ) - average ;
				variance += value * value ;

				++it ;
			}

			variance /= ( double )( size ) ;
			divergence = sqrt( variance ) ;

			// median
			std::sort( list.begin(), list.end() ) ;

			if( size % 2 == 0 )
			{
				median = ( list[ size / 2 - 1 ] + list[ size / 2 ] ) / 2 ;
			}
			else
			{
				median = list[ size / 2 ] ;
			}
		}
	}
} ;

// --- CC_File ---

struct CC_Folder ;

///
/// \brief Definition of a file for ComCheck
///
struct CC_File
{
	// definition
	QString		name ; ///< name of the file
	CC_Folder * folder ; ///< folder of the file

	// content
	bool analyzed ; ///< true if array is fulfill
	std::array< unsigned int, type_size > type ; ///< array for lines' type
	std::array< unsigned int, desc_size > description ; ///< array for lines' description

	// coverage
	double percentage ; ///< ratio comment / total
	QList< double > weight ; ///< weight values for each portion of comment + code
	CC_Statistics coverage ; ///< average, variance, divergence and median of weights
} ;

// --- CC_Folder ---

///
/// \brief Definition of a folder for ComCheck
///
struct CC_Folder
{
	// definition
	QString		name ; ///< name of the folder
	CC_Folder * parent ; ///< parent folder of the folder

	// content
	QList< CC_File * > list_files ; ///< list of file in this folder
	QList< CC_Folder * > list_folders ; ///< list of folder in this folder
} ;

// --- CC_Line ---

///
/// \brief Structure of a line
///
/// Definition of a line as the association of a type with a description
///
struct CC_Line
{
	CC_Type type ;
	CC_Desc description ;

	// constructor
	CC_Line() :
		type( ERROR ),
		description( UNDEFINED )
	{}
} ;

// --- CC_Report ---

///
/// \brief Definition of a report for ComCheck
///
struct CC_Report
{
	// definition
	 CC_Folder * folder ; ///< folder studied in this report

	// content
	std::array< unsigned int, type_size > type ; ///< array for all lines studied (type)
	std::array< unsigned int, desc_size > description ; ///< array for all lines studied (description)
	QList< CC_File * > list_files ; ///< list of files studied in the report

	// coverage
	QList< double > percentages ; ///< list of ratio comment / total
	CC_Statistics percentage ;///< average, variance, divergence and median of ratio comment / total

	QList< double > coverages ; ///< list of coverages' average
	CC_Statistics coverage ; ///< average, variance, divergence and median of coverages
} ;

#endif // STRUCTURE_HPP
