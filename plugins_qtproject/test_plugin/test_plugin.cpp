#include "test_plugin.hpp"

TestPlugin::TestPlugin
()
{
	QStringList tmp = { "*.cpp", "*.hpp" } ;

	extensions_ = tmp ;
	language_ = "Test" ;

	rank_ = USHRT_MAX - 1 ; // default value
}

TestPlugin::~TestPlugin
()
{

}

CC_Flag
TestPlugin::get_type
(
	const std::string & line
)
{
	Q_UNUSED( line ) ;
	return CC_Flag::CC_ERROR ;
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)

	Q_EXPORT_PLUGIN2( plugin_TestPlugin, TestPlugin )

#endif
