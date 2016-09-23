#ifndef CONSOLE_UI_HPP
#define CONSOLE_UI_HPP

#include <csignal>

#include "colors.hpp"
#include "iui.hpp"
#include "std.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef Q_OS_UNIX
#include <sys/ioctl.h>
#endif

#ifdef Q_OS_WIN
#include <windows.h>
#endif

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

		// --- ATTRIBUTES ---

		static unsigned int _cols_ ; ///< Number of columns in the console
		static unsigned int _lines_ ; ///< Number of lines currently used in the console
		static unsigned int _rows_ ; ///< Number of rows in the console

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
		/// \brief Display common paragraph
		/// \param paragraph Paragraph
		///
		void display_paragraph( std::string text ) ;

		///
		/// \brief Display common text according to the console's size
		/// \param text Text
		///
		void display_text( std::string text ) ;

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

		///
		/// \brief Handle the WINdow CHange signal
		///
		void WINCH_handler( int signum ) ;

		// --- ATTRIBUTES ---

		bool welcomed_ ; ///< Signal for a nice welcoming message
} ;

///
/// \brief Get information about the console (width, height, position)
///
void find_console( int signum ) ;

#endif // CONSOLE_UI_HPP
