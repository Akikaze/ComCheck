#ifndef TEMPLATE_HPP
#define TEMPLATE_HPP

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

} ;

#endif // TEMPLATE_HPP
