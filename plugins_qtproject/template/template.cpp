#include "template.hpp"

TemplatePlugin::TemplatePlugin
()
{
	QStringList tmp = { "*.???" } ;

	extensions_ = tmp ;
	language_ = "Template" ;

	rank_ = USHRT_MAX - 1 ; // default value
}

TemplatePlugin::~TemplatePlugin
()
{

}

CC_Line
TemplatePlugin::get_type
(
	const std::string & line
)
{
	Q_UNUSED( line ) ;
	return CC_Flag::CC_ERROR ;
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)

	Q_EXPORT_PLUGIN2( plugin_Template, TemplatePlugin )

#endif
