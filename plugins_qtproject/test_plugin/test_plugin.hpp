#ifndef TEST_HPP
#define TEST_HPP

#include <qglobal.h>

#include "iplugin.hpp"

class TestPlugin
: public QObject
, public IPlugin
{
	Q_OBJECT

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)

	Q_PLUGIN_METADATA( IID "ComCheck.IPlugin" )

#endif

	Q_INTERFACES( IPlugin )

    public :

        TestPlugin() ;
		virtual ~TestPlugin() ;

		virtual CC_Flag get_type( const std::string & line ) ;
} ;

#endif // TEST_HPP
