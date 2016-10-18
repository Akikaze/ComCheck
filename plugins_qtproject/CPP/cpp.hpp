#ifndef TEST_HPP
#define TEST_HPP

#include <qglobal.h>

#include "iplugin.hpp"

class CPP_Plugin
: public QObject
, public IPlugin
{
	Q_OBJECT

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

	Q_PLUGIN_METADATA( IID "ComCheck.IPlugin" )

#endif

	Q_INTERFACES( IPlugin )

    public :

		// --- CONSTRUCTORS ---

		///
		/// \brief Constructor
		///
		CPP_Plugin() ;

		// --- DESTRUCTORS ---

		///
		/// \brief Destructor
		///
		virtual ~CPP_Plugin() ;

		// --- GETTERS ---

		///
		/// \brief Get the type of a line
		/// \param Line of the file
		/// \return CC_Line which describe the line
		///
		virtual CC_Line get_type( const std::string & line ) ;

	protected :

		// --- GETTERS ---

		///
		/// \brief Get the description of a comment
		/// \param Comment in a line
		/// \return CC_Desc
		///
		virtual CC_Desc get_description( const std::string & comment ) ;

		// --- MEMBERS ---

		///
		/// \brief Return an iterator on a position return by find_first_of
		/// \param line Line which is going to call find_first_of
		/// \param string String parameter for the find_first_of call
		/// \return String iterator
		///
		std::string::iterator ffo_iterator( std::string & line, const std::string & string ) const ;

		// --- ATTRIBUTES ---

		bool block_comment_ ; ///< Signal for a block comment already started
		CC_Desc block_description_ ; ///< Description of a block comment
} ;

#endif // TEST_HPP
