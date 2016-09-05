#include "qt.hpp"
#include "core.hpp"

int main( int argc, char ** argv )
{
	Core c( argc, argv ) ;

	c.create_tree_view() ;
	c.make_report() ;

	return c.exec() ;
}
