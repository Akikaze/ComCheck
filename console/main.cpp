#include "qt.hpp"
#include "core.hpp"

int main( int argc, char ** argv )
{
	Core * c = new Core( argc, argv ) ;

	IUI * user_interface = c->create_UI() ;
	QObject::connect( user_interface, SIGNAL( finished() ), c, SLOT( quit() ) ) ;
	QTimer::singleShot( 0, user_interface, SLOT( run() ) ) ;

	c->exec() ;
	delete c ;

	return 0 ;
}
