#include "template.hpp"

TemplatePlugin::TemplatePlugin
()
{
	set_prefix() ;

	QStringList tmp = { "*.???" } ;

	extensions_ = tmp ;
	language_ = "Template" ;

	rank_ = USHRT_MAX - 1 ; // default value
}

TemplatePlugin::~TemplatePlugin
()
{

}

CC_Desc
TemplatePlugin::get_description
(
	const std::string & comment
)
{
	Q_UNUSED( comment ) ;
	return UNDEFINED ;
}

CC_Line
TemplatePlugin::get_type
(
	const std::string & line
)
{
	Q_UNUSED( line ) ;
	return CC_Line() ;
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)

	Q_EXPORT_PLUGIN2( plugin_Template, TemplatePlugin )

#endif
