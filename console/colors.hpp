#ifndef COLORS_HPP
#define COLORS_HPP

#include <QtPlugin>

#ifdef Q_OS_UNIX

#define RST  "\x1B[0m"
#define F_RED  "\x1B[31m"
#define F_GRN  "\x1B[32m"
#define F_YEL  "\x1B[33m"
#define F_BLU  "\x1B[34m"
#define F_MAG  "\x1B[35m"
#define F_CYN  "\x1B[36m"
#define F_WHT  "\x1B[37m"

#define RED( x ) F_RED x RST
#define GRN( x ) F_GRN x RST
#define YEL( x ) F_YEL x RST
#define BLU( x ) F_BLU x RST
#define MAG( x ) F_MAG x RST
#define CYN( x ) F_CYN x RST
#define WHT( x ) F_WHT x RST

#define BOLD( x ) "\x1B[1m" x RST
#define UNDL( x ) "\x1B[4m" x RST

#define TITLE( x ) std::cout << BOLD( WHT( x ) ) << std::endl ;

#endif

#ifdef Q_OS_WIN

// not working but you've got the idea

#define RST system( "color" ) ;
#define F_01 system( "color 01" ) ;
#define F_02 system( "color 02" ) ;
#define F_03 system( "color 03" ) ;
#define F_04 system( "color 04" ) ;
#define F_05 system( "color 05" ) ;
#define F_06 system( "color 06" ) ;
#define F_07 system( "color 07" ) ;
#define F_08 system( "color 08" ) ;
#define F_09 system( "color 09" ) ;
#define F_0A system( "color 0A" ) ;
#define F_0B system( "color 0B" ) ;
#define F_0C system( "color 0C" ) ;
#define F_0D system( "color 0D" ) ;
#define F_0E system( "color 0E" ) ;
#define F_0F system( "color 0F" ) ;

#define T_01( x ) F_01 std::cout << x << std::endl RST
#define T_02( x ) F_02 std::cout << x << std::endl RST
#define T_03( x ) F_03 std::cout << x << std::endl RST
#define T_04( x ) F_04 std::cout << x << std::endl RST
#define T_05( x ) F_05 std::cout << x << std::endl RST
#define T_06( x ) F_06 std::cout << x << std::endl RST
#define T_07( x ) F_07 std::cout << x << std::endl RST
#define T_08( x ) F_08 std::cout << x << std::endl RST
#define T_09( x ) F_09 std::cout << x << std::endl RST
#define T_0A( x ) F_0A std::cout << x << std::endl RST
#define T_0B( x ) F_0B std::cout << x << std::endl RST
#define T_0C( x ) F_0C std::cout << x << std::endl RST
#define T_0D( x ) F_0D std::cout << x << std::endl RST
#define T_0E( x ) F_0E std::cout << x << std::endl RST
#define T_0F( x ) F_0F std::cout << x << std::endl RST

#define TITLE( x ) T_0F( x )

#endif

#endif // COLORS_HPP
