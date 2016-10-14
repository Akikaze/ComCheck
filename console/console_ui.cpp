#include "console_ui.hpp"

unsigned int ConsoleUI::_cols_ = 0 ;
unsigned int ConsoleUI::_rows_ = 0 ;

#ifdef Q_OS_UNIX
struct winsize ConsoleUI::_w_ ;
#endif

#ifdef Q_OS_WIN
CONSOLE_SCREEN_BUFFER_INFO ConsoleUI::_csbi_ = CONSOLE_SCREEN_BUFFER_INFO() ;
#endif

///
/// \brief Constructor
/// \param parent Parent for this QObject
///
ConsoleUI::ConsoleUI
(
	QObject * core,
	bool welcomed
)
// use the interface constructor
: IUI( core )
, buffer_( QStringList() )
, current_folder_( nullptr )
, current_report_( nullptr )
, welcomed_( welcomed )
{

#ifdef Q_OS_UNIX
	// clean the console
	system( "clear" ) ;

	// handle the signal of WINdow CHange
	signal( SIGWINCH, UNIX_console_size ) ;
#endif

#ifdef Q_OS_WIN
	// clean the console
	system( "cls" ) ;
#endif

	// get size of the console
	console_size() ;

	if( _cols_ == 0 || _rows_ == 0 )
	{
		std::cout << "Impossible to get console size." << std::endl ;
		std::cout << "By default, the size will be 80x60." << std::endl ;

		// default values
		_cols_ = 80 ;
		_rows_ = 60 ;
	}
}

///
/// \brief Destructor
///
ConsoleUI::~ConsoleUI
()
{

}

///
/// \brief Add space to align text correctly
/// \param line Text line
/// \param alignment Left, center or right
///
QString
ConsoleUI::align_line
(
	QString line,
	CUI_TextAlignment alignment
)
{
	// blank is going to be add in the line to push text to the right or in the center
	QString blank = "" ;
	unsigned int spaces = _cols_ - line.length() ;

	// if it is a centering, you need to push same space on each side
	if( alignment == CUI_CENTER )
	{
		spaces /= 2 ;
	}

	// fullfil blank
	for( unsigned int i = 0 ; i < spaces ; ++i )
	{
		blank += ' ' ;
	}

	// push text thanks to blank
	if( alignment == CUI_CENTER )
	{
		line = blank + line + blank ;
	}
	else if( alignment == CUI_LEFT )
	{
		line = line + blank ;
	}
	else
	{
		line = blank + line ;
	}

	return line ;
}

///
/// \brief Bufferize common text
/// \param text Text
///
void
ConsoleUI::bufferize_text
(
	QString text
)
{
	// by default, go to the next line
	if( text.isEmpty() )
	{
		buffer_.push_back( "" ) ;
	}
	else
	{
		unsigned int pos_string = 0 ;

		// if the text is too long
		if( ( unsigned int )( text.length() ) > _cols_ )
		{
			// find the closest space to create a subline
			while( ( unsigned int )( text.length() ) > _cols_ )
			{
				unsigned int pos_real = 0 ;
				unsigned int pos_space = 0 ;
				pos_string = 0 ;

				// while the text is not ended
				while( pos_string < ( unsigned int )( text.length() ) && pos_real < _cols_ )
				{
					// if it is a backslash (like with color_text)
					if( text[ pos_string ] == 27 )
					{
						// jump after the #
						do
						{
							++pos_string ;
						}
						while( text[ pos_string ] != QChar( '#' ) ) ;

						// remove the #
						text.remove( pos_string, 1 ) ;
					}
					else
					{
						// if it is a space, save the position
						if( text[ pos_string ] == QChar( ' ' ) )
						{
							pos_space = pos_string ;
						}
					}

					++pos_real ;
					++pos_string ;
				}

				// weird case when the text line is bigger than possible but the real line is shorter
				if( pos_string == ( unsigned int )( text.length() ) && pos_real < _cols_ )
				{
					// it can appear if we use ton of colors in the same line
					break ;
				}

				// subdivide the text to get one line
				buffer_.push_back( text.left( pos_space ) ) ;

				// loop on the rest of the text
				text.remove( 0, pos_space + 1 ) ;
			}
		}

		pos_string = 0 ;

		// just withdraw remain of #
		while( pos_string < ( unsigned int )( text.length() ) )
		{
			if( text[ pos_string ] == QChar( '#' ) )
			{
				text.remove( pos_string, 1 ) ;
			}
			else
			{
				++pos_string ;
			}
		}

		// add the line in the buffer
		buffer_.push_back( text ) ;
	}
}

///
/// \brief Bufferize title in color
/// \param title Title
///
void
ConsoleUI::bufferize_title
(
	QString title
)
{
	// just color the text before bufferize it
	buffer_.push_back( color_text( "\t" + title, CUI_White, false ) ) ;
}

///
/// \brief Color text according to the OS
/// \param text Text
/// \param color Color for the text
/// \param add_hashtag Add a mark at the end of the codage. False if you are not using bufferize_text
/// \return string with encrypted character for colorization
///
QString
ConsoleUI::color_text
(
	QString text,
	CUI_TextColor color,
	bool add_hashtag
)
{
	// if you need to sgnal the end of a colorization mark
	QString hashtag = "" ;

	// by default, it is bold (to be clearer)
	QString tmp = "\x1B[1m" ; // bold

	// QString tmp = "\x1B[4m" ; // underline

#ifdef Q_OS_WIN
	// change the color because Blue is invisible
	if( color == CUI_Blue )
	{
		color = CUI_Cyan ;
	}
#endif

	// define codage for each color
	switch( color )
	{
		case CUI_Red : tmp += "\x1B[31m" ; break ;
		case CUI_Green : tmp += "\x1B[32m" ; break ;
		case CUI_Yellow : tmp += "\x1B[33m" ; break ;
		case CUI_Blue : tmp += "\x1B[34m" ; break ;
		case CUI_Magenta : tmp += "\x1B[35m" ; break ;
		case CUI_Cyan : tmp += "\x1B[36m" ;	break ;
#ifdef Q_OS_UNIX
		case CUI_White : tmp += "\x1B[37m" ; break ;
#endif
#ifdef Q_OS_WIN
		case CUI_White : tmp += "\x0" ; break ;
#endif
	}

	// add # if it is required
	if( add_hashtag == true )
	{
		hashtag = "#" ;
	}

	// recreate the line frame by color
	tmp += hashtag + text ;
	tmp += "\x1B[0m" ; // reset
	text = tmp + hashtag ;

	// return the text
	return text ;
}

///
/// \brief Get the size of the console
///
void
ConsoleUI::console_size
()
{
	// call the onsole_size method for each operator

#ifdef Q_OS_UNIX
	UNIX_console_size() ;
#endif

#ifdef Q_OS_WIN
	WIN_console_size() ;
#endif
}

///
/// \brief Display a array correctly
/// \param array Array
/// \param shortcut Require a beautiful description or the minimal
/// \return QString if shortcut is activated
///
QString
ConsoleUI::display_array
(
	const std::array< unsigned int, CC_Flag::CC_Flag_Size > & array,
	bool shortcut
)
{
	QString result = "" ;

	// if you want a long description like a report array
	if( shortcut == false )
	{
		// bufferize text
		bufferize_text( color_text( "Total number of lines: ", CUI_White ) + QString::number( array[ 0 ] ) ) ;
		bufferize_text() ;

		bufferize_text( color_text( "Number of comment line: ", CUI_White ) + QString::number( array[ 1 ] ) ) ;
		bufferize_text( color_text( "Number of mixed line: ", CUI_White ) + QString::number( array[ 2 ]  ) ) ;
		bufferize_text( color_text( "Number of pure code line: ", CUI_White ) + QString::number( array[ 3 ]  ) ) ;
		bufferize_text() ;
	}
	// or a short description like a file array
	else
	{
		// or write directly on a line
		result += color_text( "TOT: ", CUI_White ) + QString::number( array[ 0 ] ) + " " ;
		result += color_text( "COM: ", CUI_White ) + QString::number( array[ 1 ] ) + " " ;
		result += color_text( "MIX: ", CUI_White ) + QString::number( array[ 2 ] ) + " " ;
		result += color_text( "COD: ", CUI_White ) + QString::number( array[ 3 ] ) + " " ;
	}

	// the return is used when you require a short description
	return result ;
}

///
/// \brief Display the whole buffer which contain strings
///
void
ConsoleUI::display_buffer
()
{
	unsigned int count = 0 ;
	bool endByEnter = true ;

	// while the buffer is not empty
	while( ! buffer_.empty() )
	{
		// write the next line
		std::cout << buffer_.first().toStdString() ;
		count++ ;

		if( count < _rows_ )
		{
			// jump
			std::cout << std::endl ;
		}
		else
		{
			endByEnter = false ;
		}

		// withdraw this line
		buffer_.erase( buffer_.begin() ) ;

		// complicated condition but simple result :
		// if the buffer is bigger than the console size, wait for a Enter before displaying another line
		while( ( ! buffer_.empty() ) &&
			   ( count > _rows_ - 1 ) &&
			   ( getchar() != 10 ) ) ;
	}

	// just to have a better display
	if( endByEnter == false )
	{
		std::cout << std::endl ;
		std::cout << std::endl ;
	}
}

///
/// \brief Display a name in tree view
/// \param name Name of the file or the folder
/// \param isFile Check if the name should be colored or not
/// \param level Size of indentation
///
void
ConsoleUI::display_name
(
	QString name,
	bool isFile,
	unsigned int level
)
{
	unsigned int pos = name.toStdString().find_last_of( '/' ) + 1 ;
	name = name.right( name.size() - pos ) ;

	// if its a folder
	if( !isFile )
	{
		// write it in blue
		name = color_text( name, CUI_Blue ) ;
	}

	// default prefix
	QString prefix = " " ;

	if( level != 0 )
	{
		// this case is for align the first line
		prefix += "|   " ;
	}

	for( unsigned int i = 1 ; i < level ; ++i )
	{
		// add as much prefix as the level
		prefix += "|   " ;
	}

	// add the last prefix
	prefix += "|---" ;

	// bufferize the name with the prefix
	bufferize_text( prefix + name ) ;
}

///
/// \brief Display a report average, variance and divergence
/// \param report Report
///
void
ConsoleUI::display_report
(
	CC_Report * report
)
{
	CC_Statistics stats ;

	// start by com_tot
	stats = report->ct_statistics ;

	bufferize_text( color_text( "Ratio comments / total - Average: ", CUI_White ) + QString::number( stats.average ) ) ;
	bufferize_text( color_text( "Ratio comments / total - Variance: ", CUI_White ) + QString::number( stats.variance ) ) ;
	bufferize_text( color_text( "Ratio comments / total - Divergence: ", CUI_White ) + QString::number( stats.divergence ) ) ;
	bufferize_text( color_text( "Ratio comments / total - Median: ", CUI_White ) + QString::number( stats.median ) ) ;

	// jump
	bufferize_text() ;

	// then com_cod
	stats = report->cc_statistics ;

	bufferize_text( color_text( "Ratio comments / codes - Average: ", CUI_White ) + QString::number( stats.average ) ) ;
	bufferize_text( color_text( "Ratio comments / codes - Variance: ", CUI_White ) + QString::number( stats.variance ) ) ;
	bufferize_text( color_text( "Ratio comments / codes - Divergence: ", CUI_White ) + QString::number( stats.divergence ) ) ;
	bufferize_text( color_text( "Ratio comments / codes - Median: ", CUI_White ) + QString::number( stats.median ) ) ;

	// jump
	bufferize_text() ;
}

///
/// \brief Display the tree view
/// \param folder Root of the tree view
/// \param level Size of indentation
///
void
ConsoleUI::display_tree
(
	const CC_Folder * folder,
	unsigned int level
)
{
	// display files in first
	QList< CC_File * > list_files = folder->list_files ;
	for( int i = 0 ; i < list_files.size() ; ++i )
	{
		display_name( list_files[ i ]->name, true, level ) ;
	}

	// and folders after
	QList< CC_Folder * > list_folders = folder->list_folders ;
	for( int i = 0 ; i < list_folders.size() ; ++i )
	{
		// display the name
		display_name( list_folders[ i ]->name, false, level ) ;

		// and be recursive
		display_tree( list_folders[ i ], level + 1 ) ;
	}
}

///
/// \brief Draw report's histogram
///
void
ConsoleUI::draw_histogram
()
{
	double average = current_report_->ct_statistics.average ;
	QList< CC_File * >::const_iterator cit ;

	QList< double > list ;

	// console size
	unsigned int c_height = _rows_ ;
	unsigned int c_width = _cols_ - 3 - 2 - 2 ; // number_cols - number_character_for_percent - character_for_column(space + |) - borders

	// histogram size
	unsigned int h_height = 0 ;
	unsigned int h_width = 0 ;

	// get min and max value
	unsigned int max_value = 0 ;
	unsigned int min_value = 100 ;

	for( cit = current_report_->list_files.constBegin() ; cit != current_report_->list_files.constEnd() ; ++cit )
	{
		list.push_back( ( *cit )->com_tot ) ;

		if( list.back() < min_value )
		{
			min_value = list.back() ;
		}

		if( list.back() > max_value )
		{
			max_value = list.back() ;
		}
	}

	// get histogram size
	h_height = 100 ; // max_value - min_value
	h_width = list.size() ;

	while( h_width > c_width )
	{
		double min_gap = 100 ;
		unsigned int lower_pos = list.size() ;

		for( int i = 1 ; i < list.size() ; ++i )
		{
			if( abs( list[ i ] - list[ i - 1 ] ) < min_gap )
			{
				lower_pos = i - 1 ;
				min_gap = abs( list[ i ] - list[ i - 1 ] ) ;
			}
		}

		double local_average = ( double )( list[ lower_pos ] + list[ lower_pos + 1 ] ) / ( double )( 2 ) ;
		list[ lower_pos ] = local_average ;
		list.erase( list.begin() + lower_pos + 1 ) ;

		h_width = list.size() ;
	}

	// definition of a line
	QString line = "" ;
	unsigned int gap = 1 ;

	if( h_height > c_height )
	{
		gap = h_height / c_height + 1;
	}

	int index = 100 ;
	bool average_drawn = false ;

	while( index >= 0 )
	{
		line = align_line( "     |" ) ;

		QString index_string = QString::number( index ) ;
		for( int i = 10 ; i < 1000 ; i *= 10 )
		{
			if( index < i )
			{
				index_string = " " + index_string ;
			}
		}

		for( int i = 0 ; i < 3 ; ++i )
		{
			line[ i + 1 ] = index_string[ i ] ;
		}

		if( average_drawn == false &&
			index < average )
		{
			for( unsigned int i = 6 ; i < c_width ; i++ )
			{
				line[ i ] = '=' ;
			}

			average_drawn = true ;
		}

		for( int i = 0 ; i < list.size() ; ++i )
		{
			if( list[ i ] > index )
			{
				line[ i + 6 ] = '*' ;
			}
		}

		bufferize_text( line ) ;
		index-= gap ;
	}

	line = " " ;
	for( unsigned int i = 1 ; i < c_width ; ++i )
	{
		line += '_' ;
	}

	bufferize_text( line ) ;
}

///
/// \brief Filter a command thanks to ' '
/// \param command String of the command line
/// \return List of strings for each part of the command
///
QStringList
ConsoleUI::filter_command
(
	QString command
)
{
	// Split the whole line into words
	QStringList list = command.split( ' ' ) ;

	// to go throught the list and erase all empty element
	QStringList::iterator it = list.begin() ;

	while( it != list.end() )
	{
		// it was two spaces next to each other
		if( it->isEmpty() )
		{
			list.erase( it ) ;
		}
		else
		{
			++it ;
		}
	}

	return list ;
}

///
/// \brief Launch the loop
///
void
ConsoleUI::process
()
{
	bool close = false ;
	std::string tmp = "" ;
	QString command = "" ;
	QStringList param_list ;

	// display the welcome message
	if( welcomed_ == true )
	{
		welcome() ;
		display_buffer() ;
	}

	while( ! close )
	{
		// avoid the remain of parameters
		param_list.clear() ;

		// get the command line
		std::cout << color_text( "CC:> ", CUI_Green, false ).toStdString() ;

		std::getline( std::cin, tmp ) ;
		command = QString( tmp.c_str() ).simplified() ;

		tmp = "" ;

		// check the command
		if( !( command.isEmpty() ) )
		{
			// create a list of parameters
			param_list = filter_command( command ) ;
			command = "" ;

			// check which function need to be called
			if( param_list.front() == "commands" )
			{
				// display each command and its description
				commands( param_list ) ;
			}
			else if( param_list.front() == "clear" )
			{
#ifdef Q_OS_UNIX
				// clear the screen
				system( "clear" ) ;
#endif

#ifdef Q_OS_WIN
				// clear the screen
				system( "cls" ) ;
#endif
			}
			// COMMANDS
			else if( param_list.front() == "analyze" )
			{
				// analyze and create a report of the current folder
				analyze( param_list ) ;
			}
			else if( param_list.front() == "directory" )
			{
				// choose the project directory
				directory( param_list ) ;
			}
			else if( param_list.front() == "export" )
			{
				// choose the project directory
				export_HTML( param_list ) ;
			}
			else if( param_list.front() == "help" )
			{
				// display more information about ComCheck or each command
				help( param_list ) ;
			}
			else if( param_list.front() == "info" )
			{
				// display information about the project
				info( param_list ) ;
			}
			else if( param_list.front() == "language" )
			{
				// choose the project language
				language( param_list ) ;
			}
			else if( param_list.front() == "move" )
			{
				// change the current folder
				move( param_list ) ;
			}
			else if( param_list.front() == "preparation" )
			{
				// create a tree view of the project directory
				preparation( param_list ) ;
			}
			else if( param_list.front() == "report" )
			{
				// display the report of the current folder
				report( param_list ) ;
			}
			else if( param_list.front() == "tree" )
			{
				// display a tree view of the project folder
				tree( param_list ) ;
			}
			// QUIT
			else if( param_list.front() == "quit" )
			{
				// quit Comcheck
				close = true ;
			}
			else
			{
				// unrecognized command
				bufferize_text( color_text( "This command is not defined. Use 'commands' to list them.", CUI_Red ) ) ;
				display_buffer() ;
			}
		}
	}

	// color line
	command = "\t" ;
	for( int i = 0 ; i < 4 ; ++i )
	{
		for( int j = 0 ; j < 6 ; ++j )
		{
			command += color_text( "_", ( CUI_TextColor )( j ), false ) ;
		}
	}

	// display ending message
	bufferize_text() ;
	bufferize_text( color_text( "\tGoodbye, see you later !", CUI_White ) ) ;
	buffer_.push_back( command ) ;
	bufferize_text() ;

	display_buffer() ;
}

///
/// \brief Display the welcome message
///
void
ConsoleUI::welcome
()
{
	// display the title
	bufferize_text() ;
	buffer_.push_back( color_text( align_line( "/================\\", CUI_CENTER ), CUI_White, false ) ) ;
	buffer_.push_back( color_text( align_line( "|    COMCHECK    |", CUI_CENTER ), CUI_White, false ) ) ;
	buffer_.push_back( color_text( align_line( "\\================/", CUI_CENTER ), CUI_White, false ) ) ;
	bufferize_text() ;

	// display a little message
	bufferize_text( "ComCheck is a little tool used for analysing source code files and counting how many comments are written in these files, regardless of programming language." ) ;
	bufferize_text() ;

	bufferize_text( "If you don't know how to use it, maybe the command 'help' could do its job !" ) ;
	bufferize_text() ;

	display_buffer() ;
}

#ifdef Q_OS_UNIX
///
/// \brief Get the size of the console
///
void
ConsoleUI::UNIX_console_size
(
	int signum
)
{
	// get console size UNIX way
	Q_UNUSED( signum ) ;

	ioctl( STDOUT_FILENO, TIOCGWINSZ, &_w_ ) ;
	ConsoleUI::_cols_ = _w_.ws_col ;
	ConsoleUI::_rows_ = _w_.ws_row ;
}
#endif

#ifdef Q_OS_WIN
///
/// \brief Get the size of the console
///
void
ConsoleUI::WIN_console_size
()
{
	// get console size Windows way
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &_csbi_ ) ;
	ConsoleUI::_cols_ = _csbi_.srWindow.Right - _csbi_.srWindow.Left ;
	ConsoleUI::_rows_ = _csbi_.srWindow.Bottom - _csbi_.srWindow.Top ;
}
#endif
