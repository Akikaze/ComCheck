#include "qt.hpp"
#include "core.hpp"

/*
Main entrance to ComCheck

The main creates a Core, plugs an interface to it.
If everything works, it is going to launch the interface and wait for a signal.

At the end, it will destroy the Core.
*/

int main( int argc, char ** argv )
{
	Core * c = new Core( argc, argv ) ;
	IUI * user_interface = c->create_UI() ;

	// create_UI return a nullptr only if the automatic flag is used
	if( user_interface != nullptr )
	{
		QObject::connect( user_interface, SIGNAL( finished() ), c, SLOT( quit() ) ) ;
		QTimer::singleShot( 0, user_interface, SLOT( run() ) ) ; // Execute the interface
		c->exec() ;
	}

	delete c ;
	return 0 ;
}
