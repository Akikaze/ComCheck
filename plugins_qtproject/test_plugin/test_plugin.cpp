#include "test_plugin.hpp"

TestPlugin::TestPlugin
()
{
	QStringList tmp = { "*.cpp", "*.hpp" } ;

	extensions_ = tmp ;
	language_ = "Test" ;
	rank_ = USHRT_MAX - 1 ;
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
    return CC_Flag::ERROR ;
}
