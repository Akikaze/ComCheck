#ifndef CC_STRUCTURE_HPP
#define CC_STRUCTURE_HPP

#include "qt.hpp"
#include "std.hpp"

// ===== STRUCTURES =====

// --- CC_Flag ---

///
/// \brief Flag for ComCheck
///
/// In a file or a report, the value 0 is the number of line but when a plugin
/// return 0, that signals an error
///
enum CC_Flag
{
	CC_ERROR = 0,
	CC_COMMENT,
	CC_MIXED,
	CC_CODE,
	CC_Flag_Size
} ;

// --- CC_Statistics ---

struct CC_Statistics
{
	double average ; ///< average value
	double variance ; ///< variance value
	double divergence ; ///< divergence value
	double median ; ///< median value
} ;

// --- CC_File ---

struct CC_Folder ;

///
/// \brief Definition of a file for ComCheck
///
struct CC_File
{
	// definition
	QString	name ; ///< name of the file
	CC_Folder * folder ; ///< folder of the file

	// content
	bool analyzed ; ///< true if array is fulfill
	std::array< unsigned int, CC_Flag::CC_Flag_Size > array ; ///< array for lines' type

	// percentages
	double com_cod ; ///< ratio comment / code
	double com_tot ; ///< ratio comment / total
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

// --- CC_Report ---

///
/// \brief Definition of a report for ComCheck
///
struct CC_Report
{
	// definition
	 CC_Folder * folder ; ///< folder studied in this report

	// content
	std::array< unsigned int, CC_Flag::CC_Flag_Size > array ; ///< array for all lines studied
	QList< CC_File * > list_files ; ///< list of files studied in the report

	// list ratio
	QList< double > list_com_cod ; ///< list of ratio com_cod
	QList< double > list_com_tot ; ///< list of ratio com_tot

	// statistics
	CC_Statistics cc_statistics ; ///< statistics values about com_cod
	CC_Statistics ct_statistics ; ///< statistics values about com_tot
} ;

#endif // STRUCTURE_HPP
