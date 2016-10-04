#include "qt.hpp"
#include "core.hpp"

// Gate to ComCheck

int main( int argc, char ** argv )
{
	// Creation of the core
	Core * c = new Core( argc, argv ) ;

	// Creation of the interface
	IUI * user_interface = c->create_UI() ;

	// Link the destruction of the interface with the destruction of the core
	QObject::connect( user_interface, SIGNAL( finished() ), c, SLOT( quit() ) ) ;

	// Execute the interface
	QTimer::singleShot( 0, user_interface, SLOT( run() ) ) ;
	c->exec() ;

	// Delete the core
	delete c ;

	return 0 ;
}
