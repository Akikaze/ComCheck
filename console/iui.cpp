#include "iui.hpp"

IUI::IUI
(
	QObject * parent
)
	: QObject( parent )
{

}

IUI::~IUI
()
{

}

void
IUI::run
()
{
	process() ;

	emit finished() ;
}
