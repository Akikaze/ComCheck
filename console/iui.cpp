#include "iui.hpp"
#include "core.hpp"

IUI::IUI
(
	QObject * core
)
: QObject( core )
{
	core_ = ( Core * )( core ) ;
}

IUI::~IUI
()
{
	core_ = nullptr ;
}

void
IUI::run
()
{
	process() ;

	emit finished() ;
}
