#ifndef COLORS_HPP
#define COLORS_HPP

#include <QtPlugin>

#ifdef Q_OS_UNIX

#define F_RST  "\x1B[0m"
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

#define TITLE( x ) BOLD( WHT( x ) )

#endif

#ifdef Q_OS_WIN



#endif

#endif // COLORS_HPP
