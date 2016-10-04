#include "iui.hpp"
#include "core.hpp"

///
/// \brief Constructor
/// \param parent Parent for this QObject
///
IUI::IUI
(
	QObject * core
)
: QObject( core )
{
	// get a pointer to the core
	core_ = ( Core * )( core ) ;
}

///
/// \brief Destructor
///
IUI::~IUI
()
{
	// release the core
	core_ = nullptr ;
}

///
/// \brief Slot to run an interface
///
void
IUI::run
()
{
	// launch the interface
	process() ;

	// emit the stop signal
	emit finished() ;
}
