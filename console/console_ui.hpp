#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include <csignal>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "colors.hpp"
#include "iui.hpp"
#include "std.hpp"

#ifdef Q_OS_UNIX
#include <sys/ioctl.h>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif

enum CUI_TextAlignment
{
	CUI_LEFT = 0,
	CUI_CENTER,
	CUI_RIGHT
};

class ConsoleUI
: public IUI
{
	public :

		// --- CONSTRUCTORS ---

		///
		/// \brief Constructor
		/// \param parent Parent for this QObject
		///
		ConsoleUI( QObject * parent = 0, bool welcomed = true ) ;

		// --- DESTRUCTORS ---

		///
		/// \brief Destructor
		///
		virtual ~ConsoleUI() ;

	protected :

		// --- COMMANDS ---

		///
		/// \brief Display the result of the command 'commands'
		/// \param param_list List of parameters for the command
		///
		void commands( QStringList param_list ) ;

		///
		/// \brief Display the result of the command 'help'
		/// \param param_list List of parameters for the command
		///
		void help( QStringList param_list ) ;

		// --- MEMBERS

		///
		/// \brief Add space to align text correctly
		/// \param line Text line
		/// \param alignment Left, center or right
		///
		QString align_line( QString line, CUI_TextAlignment alignment = CUI_LEFT ) ;

		///
		/// \brief Get the size of the console
		///
		static void console_size() ;

		///
		/// \brief Display common paragraph
		/// \param paragraph Paragraph
		///
		void display_paragraph( std::string text ) ;

		///
		/// \brief Display common text according to the console's size
		/// \param text Text
		///
		void display_text( std::string text, CUI_TextAlignment alignment = CUI_LEFT  ) ;

		///
		/// \brief Display title
		/// \param title Title
		///
		void display_title( std::string title ) ;

		///
		/// \brief Filter a command thanks to ' '
		/// \param command String of the command line
		/// \return List of strings for each part of the command
		///
		QStringList filter_command( QString command ) ;

		///
		/// \brief Launch the loop
		///
		virtual void process() ;

		///
		/// \brief Display the welcome message
		///
		void welcome() ;

		// --- ATTRIBUTES ---

		static unsigned int _cols_ ; ///< Number of columns in the console
		static unsigned int _rows_ ; ///< Number of rows in the console
		static QStringList _text_ ; ///<
		bool welcomed_ ; ///< Signal for a nice welcoming message

		// --- OS EXLUSIVE ---

#ifdef Q_OS_UNIX
		static struct winsize _w_ ; ///<

		///
		/// \brief Get the size of the console
		///
		static void UNIX_console_size() ;
#endif

#ifdef Q_OS_WIN
		static CONSOLE_SCREEN_BUFFER_INFO _csbi_ ; ///<

		///
		/// \brief Get the size of the console
		///
		static void WIN_console_size() ;
#endif

} ;

#endif // CONSOLE_UI_HPP
