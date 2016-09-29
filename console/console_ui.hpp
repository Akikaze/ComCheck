#ifndef CONSOLE_UI__HPP
#define CONSOLE_UI__HPP

#include <cstdio>
#include <cstdlib>

#include <unistd.h>

#include "iui.hpp"

#ifdef Q_OS_UNIX
#include <csignal>
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
} ;

enum CUI_TextColor
{
	CUI_Red,
	CUI_Green,
	CUI_Yellow,
	CUI_Blue,
	CUI_Magenta,
	CUI_Cyan,
	CUI_White
} ;

class ConsoleUI
: public IUI
{
	public :

		// --- CONSTRUCTORS ---

		///
		/// \brief Constructor
		/// \param parent Parent for this QObject
		///
		ConsoleUI( QObject * core, bool welcomed = true ) ;

		// --- DESTRUCTORS ---

		///
		/// \brief Destructor
		///
		virtual ~ConsoleUI() ;

	protected :

		// --- COMMANDS ---

		///
		/// \brief Display the result of the command 'analyze'
		/// \param param_list List of parameters for the command
		///
		void analyze( QStringList param_list ) ;

		///
		/// \brief Display the result of the command 'commands'
		/// \param param_list List of parameters for the command
		///
		void commands( QStringList param_list ) ;

		///
		/// \brief Display the result of the command 'directory'
		/// \param param_list List of parameters for the command
		///
		void directory( QStringList param_list ) ;

		///
		/// \brief Display the result of the command 'help'
		/// \param param_list List of parameters for the command
		///
		void help( QStringList param_list ) ;

		///
		/// \brief Display the result of the command 'info'
		/// \param param_list List of parameters for the command
		///
		void info( QStringList param_list ) ;

		///
		/// \brief Display the result of the command 'language'
		/// \param param_list List of parameters for the command
		///
		void language( QStringList param_list ) ;

		///
		/// \brief Display the result of the command 'move'
		/// \param param_list List of parameters for the command
		///
		void move( QStringList param_list ) ;

		///
		/// \brief Display the result of the command 'preparation'
		/// \param param_list List of parameters for the command
		///
		void preparation( QStringList param_list ) ;

		///
		/// \brief Display the result of the command 'tree'
		/// \param param_list List of parameters for the command
		///
		void tree( QStringList param_list ) ;

		// --- MEMBERS

		///
		/// \brief Add space to align text correctly
		/// \param line Text line
		/// \param alignment Left, center or right
		///
		QString align_line( QString line, CUI_TextAlignment alignment = CUI_LEFT ) ;

		///
		/// \brief Bufferize common text
		/// \param text Text
		///
		void bufferize_text( QString text = "" ) ;

		///
		/// \brief Bufferize title in color
		/// \param title Title
		///
		void bufferize_title( QString title ) ;

		///
		/// \brief Color text according to the OS
		/// \param text Text
		/// \param color Color for the text
		/// \return string with encrypted character for colorization
		///
		QString color_text( QString text, CUI_TextColor color ) ;

		///
		/// \brief Get the size of the console
		///
		static void console_size() ;

		///
		/// \brief Display the whole buffer which contain strings
		///
		void display_buffer() ;

		///
		/// \brief Display a name in tree view
		/// \param name Name of the file or the folder
		/// \param level Size of indentation
		///
		void display_name( QString name, bool isFile, unsigned int level ) ;

		///
		/// \brief Display the tree view
		/// \param folder Root of the tree view
		/// \param level Size of indentation
		///
		void display_tree( const CC_Folder * folder, unsigned int level = 0 ) ;

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

		QStringList buffer_ ; ///< Buffer of text
		CC_Folder * current_folder_ ; ///< Current folder
		bool welcomed_ ; ///< Signal for a nice welcoming message

		// --- OS EXLUSIVE ---

#ifdef Q_OS_UNIX
		static struct winsize _w_ ; ///<

		///
		/// \brief Get the size of the console
		///
		static void UNIX_console_size( int signum = 0 ) ;
#endif

#ifdef Q_OS_WIN
		static CONSOLE_SCREEN_BUFFER_INFO _csbi_ ; ///<

		///
		/// \brief Get the size of the console
		///
		static void WIN_console_size() ;
#endif

} ;

#endif // CONSOLE_UI__HPP
