#include "qt.hpp"
#include "core.hpp"

int main( int argc, char ** argv )
{
	Core c( argc, argv ) ;

	IUI * user_interface = c.create_UI( &c ) ;
	QObject::connect( user_interface, SIGNAL( finished() ), &c, SLOT( quit() ) ) ;
	QTimer::singleShot( 0, user_interface, SLOT( run() ) ) ;

	/*
	c.create_tree_view() ;
	c.make_report() ;
	*/

	return c.exec() ;
}
