#ifndef TEST_HPP
#define TEST_HPP

#include <qglobal.h>

#include "iplugin.hpp"

class TemplatePlugin
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
		TemplatePlugin() ;

		// --- DESTRUCTORS ---

		///
		/// \brief Destructor
		///
		virtual ~TemplatePlugin() ;

		// --- GETTERS ---

		///
		/// \brief Get the type of a line
		/// \param Line of the file
		/// \return CC_Flag which describe the line
		///
		virtual CC_Flag get_type( const std::string & line ) ;
} ;

#endif // TEST_HPP
