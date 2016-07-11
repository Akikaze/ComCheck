#include "C.hpp"

extern "C"
IPlugin *
load
()
{
	return new C ;
}

C::C
()
: IPlugin()
{
	
}

C::~C
()
{
	
}

std::array< unsigned int, 4 >
C::analyze
(
	const std::string & line
)
const
{
	
}
