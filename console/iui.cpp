#include "iui.hpp"
#include "core.hpp"

/*
Every interface require a pointer on the core to be able to call it.
This is why the constuction of the IUI store the pointer in core_.
*/
IUI::IUI
(
	QObject * core
)
: QObject( core )
{
	core_ = ( Core * )( core ) ;
}

/*
A destruction of an interface means the end of the execution and the
destruction of the Core. Thus, there is no need to destroy the core here.
We just need to release it correctly.
*/
IUI::~IUI
()
{
	core_ = nullptr ;
}

/*
The run function is called at the start of the execution.
It calls the process method to display the interface.

When the end of the process method means the end of the execution. So the
finished signal is emitted to be catch by the QCoreApplication which is going
to destroy the Core.
*/
void
IUI::run
()
{
	process() ;
	emit finished() ;
}
