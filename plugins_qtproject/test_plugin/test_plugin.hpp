#ifndef TEST_HPP
#define TEST_HPP

#include "iplugin.hpp"

class TestPlugin
: public QObject
, public IPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA( IID "ComCheck.IPlugin" )
	Q_INTERFACES( IPlugin )

    public :

        TestPlugin() ;
		virtual ~TestPlugin() ;

		virtual CC_Flag get_type( const std::string & line ) ;
} ;

#endif // TEST_HPP
