#include "core.hpp"

Core::Core
(
	int & argc,
	char ** argv
)
: QCoreApplication( argc, argv )
, directory_( "" )
, interfaced_( false )
, plugin_( nullptr )
, report_( nullptr )
, root_( nullptr )
, UI_( nullptr )
, welcomed_( true )
{
	// get plugins
	list_plugins_ = list_plugins() ;

	// get attribute
	if( argc > 1 )
	{
		for( int i = 1 ; i < argc ; ++i )
		{
			// directory
			if( QString( argv[ i ] ) == "-d" ||
				QString( argv[ i ] ) == "--directory" )
			{
				directory_ = argv[ ++i ] ;
			}

			// language
			if( QString( argv[ i ] ) == "-l" ||
				QString( argv[ i ] ) == "--language" )
			{
				plugin_ = find_plugin( argv[ ++i ] ) ;
			}

			// interface
			if( QString( argv[ i ] ) == "-i" ||
				QString( argv[ i ] ) == "--interface" )
			{
				interfaced_ = true ;
			}

			// test
			if( QString( argv[ i ] ) == "-t" ||
				QString( argv[ i ] ) == "--test" )
			{
				directory_ = "../" ;
				interfaced_ = false ;
				plugin_ = find_plugin( "Test" ) ;
			}

			//welcomed
			if( QString( argv[ i ] ) == "-w" ||
				QString( argv[ i ] ) == "-welcomed" )
			{
				welcomed_ = false ;
			}
		}
	}
}

Core::~Core
()
{
	// release plugins
	while( !( list_plugins_.empty() ) )
	{
		delete list_plugins_[ 0 ] ;
		list_plugins_.erase( list_plugins_.begin() ) ;
	}

	// release reports
	while( !( map_reports_.empty() ) )
	{
		delete ( map_reports_[ 0 ].second ) ;
		map_reports_.erase( map_reports_.begin() ) ;
	}

	// release tree view
	if( root_ != nullptr )
	{
		release_tree() ;
	}

	delete UI_ ;

	plugin_ = nullptr ;
	report_ = nullptr ;
	UI_ = nullptr ;
}

void
Core::analyze_file
(
	CC_File * file
)
{
	std::ifstream ifs( file->name.toStdString(), std::ios::in ) ;

	if( ifs )
	{
		std::string line ;

		while( std::getline( ifs, line ) )
		{
			clear_line( line ) ; // remove all useless characters

			if( !( line.empty() ) )
			{
				CC_Flag flag = plugin_->get_type( line ) ;// get back information about the line

				file->array[ 0 ] += 1 ; // add a line

				if( flag != CC_Flag::CC_ERROR )
				{
					file->array[ flag ] += 1 ; // add info about the line
				}
			}
		}

		file->analyzed = true ;
	}
}

CC_Report *
Core::check_reports
(
	CC_Folder * folder
)
{
	CC_Report * result = nullptr ;
	QList< QPair< CC_Folder *, CC_Report * > >::const_iterator cit = map_reports_.constBegin() ;

	while( cit != map_reports_.constEnd() && result == nullptr )
	{
		if( ( *cit ).first == folder )
		{
			result = ( *cit ).second ;
		}
		else
		{
			++cit ;
		}
	}

	return result ;
}

void
Core::clear_line
(
	std::string & line
)
{
	unsigned int i = 0 ;
	unsigned int length = line.length() ;

	while( i < length )
	{
		if( int( line[ i ] ) == 9 || // tab
			int( line[ i ] ) == 10 || // new line
			int( line[ i ] ) == 32 ) // ' '
		{
			line.erase( line.begin() + i ) ;
			--length ;
		}
		else
		{
			++i ;
		}
	}
}

void
Core::compute_report
(
	CC_Report * report
)
{
	// compute average, variance and divergence
	report->average = 0 ;
	report->variance = 1 ;
	report->divergence = sqrt( report->variance ) ;
}

CC_Folder *
Core::create_branch
(
	QString name_folder
)
{
	CC_File * file = nullptr ;
	CC_Folder * folder = nullptr ;

	QList< CC_File * > list_files ;
	QList< CC_File * >::const_iterator cit_File ;
	QList< CC_Folder * > list_folders ;
	QList< CC_Folder * >::const_iterator cit_Folder ;

	QStringList list_folders_name ;
	QStringList list_files_name ;
	QStringList::const_iterator cit ;

	QDir directory( name_folder ) ;

	// get folders and files name
	list_folders_name =  directory.entryList( QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable | QDir::Executable ) ;
	list_files_name =  directory.entryList( plugin_->get_extensions(), QDir::Files | QDir::Readable ) ;

	// create a CC_File for each file
	for( cit = list_files_name.constBegin() ; cit != list_files_name.constEnd() ; ++cit )
	{
		// new file
		file = new CC_File() ;
		file->name = name_folder + "/" + *cit ;
		file->analyzed = false ;

		// store the file in its folder
		list_files.push_back( file ) ;
	}

	// check each folder in this one
	for( cit = list_folders_name.constBegin() ; cit != list_folders_name.constEnd() ; ++cit )
	{
		folder = create_branch( name_folder + "/" + *cit ) ;

		if( folder != nullptr )
		{
			list_folders.push_back( folder ) ;
		}
	}

	if( ! list_files.empty() || ! list_folders.empty() )
	{
		folder = new CC_Folder() ;
		folder->name = name_folder ;

		for( cit_File = list_files.begin() ; cit_File != list_files.end() ; ++cit_File )
		{
			( *cit_File )->folder = folder ;
		}

		for( cit_Folder = list_folders.begin() ; cit_Folder != list_folders.end() ; ++cit_Folder )
		{
			( *cit_Folder )->parent = folder ;
		}

		folder->list_files = list_files ;
		folder->list_folders = list_folders ;

	}

	return folder ;
}

CC_Folder *
Core::create_tree_view
()
{
	// no chosen plugin or no directory
	if( directory_.isEmpty() ||
		plugin_ == nullptr )
	{
		return nullptr ;
	}

#ifdef Q_OS_UNIX
	if( directory_.at( directory_.size() - 1) == '/' )
	{
		directory_ = directory_.left( directory_.size() - 1 ) ;
	}
#endif

#ifdef Q_OS_WIN
	directory = ;
#endif

	root_ = create_branch( directory_ ) ;
	return root_ ;
}

IUI *
Core::create_UI
()
{
	if( interfaced_ == true )
	{
		std::cout << "Currently, there is no graphical interface available." << std::endl ;
	}
	else
	{
		UI_ = new ConsoleUI( this, welcomed_ ) ;
	}

	return UI_ ;
}

IPlugin *
Core::find_plugin
(
	QString language
)
{
	// loop preparation
	IPlugin * plugin = nullptr ;
	QList< IPlugin * >::const_iterator cit = list_plugins_.constBegin() ;

	// look for a plugin which use this language
	while( cit != list_plugins_.constEnd() && plugin == nullptr )
	{
		if( ( *cit )->get_language() == language )
		{
			plugin = ( *cit ) ;
		}

		++cit ;
	}

	return plugin ;
}

QList< IPlugin * >
Core::list_plugins
()
{
	QList< IPlugin * > list ;
	IPlugin * plugin = nullptr ;

	QString dir = "../plugins" ;
	QDir plugin_directory = QDir( dir ) ;

	// look for every plugin
	foreach( QString file, plugin_directory.entryList( QDir::Files ) )
	{
		// load the plugin
		file = dir + "/" + file ;
		QPluginLoader loader( file ) ;

		// if it's the right plugin format
		if( loader.load() == true )
		{
			plugin = qobject_cast< IPlugin * >( loader.instance() ) ;
			list.push_back( plugin ) ;
		}
	}

	return list ;
}

CC_Report *
Core::make_report
(
	CC_Folder * folder
)
{
	// no chosen plugin or no root folder
	if( root_ == nullptr ||
		plugin_ == nullptr )
	{
		return nullptr ;
	}

	// by default, use the root folder
	if( folder == nullptr )
	{
		folder = root_ ;
	}

	// find if the folder was analyzed before
	CC_Report * tmp = check_reports( folder ) ;

	if( tmp == nullptr )
	{
		// creation of a new report
		report_ = new CC_Report() ;
		report_->folder = folder ;

		// loop preparation
		QList< CC_Folder * > list_folder = folder->list_folders ;
		list_folder.push_front( folder ) ;

		QList< CC_File * >::const_iterator cit_File ;
		QList< CC_Folder * >::const_iterator cit_Folder ;
		CC_Folder * current = nullptr ;

		while( ! list_folder.empty() )
		{
			// extract the current folder
			current = list_folder[ 0 ] ;
			list_folder.erase( list_folder.begin() ) ;

			// add current folder in the list
			for( cit_Folder = current->list_folders.begin() ; cit_Folder != current->list_folders.end() ; ++cit_Folder )
			{
				list_folder.push_front( *cit_Folder ) ;
			}

			// analyze files of the current folder
			for( cit_File = current->list_files.begin() ; cit_File != current->list_files.end() ; ++cit_File )
			{
				// check if the file is analyzed
				if( ( *cit_File )->analyzed == false )
				{
					analyze_file( *cit_File ) ;
				}

				report_->list_files.push_back( *cit_File ) ;

				// compute percentage
				double percent = 0 ;
				report_->percents.push_back( percent ) ;

				// store value
				report_->array += ( *cit_File )->array ;
			}
		}

		// compute average and divergence
		compute_report( report_ ) ;

		// add this report in the list
		map_reports_.push_back( qMakePair( folder, report_ ) ) ;
	}
	else
	{
		// changement of the displayed report
		report_ = tmp ;
	}

	return report_ ;
}

void
Core::release_tree
()
{
	CC_Folder * current = nullptr ;
	QList< CC_Folder * > list ;

	if( root_ != nullptr )
	{
		list = { root_ } ;
	}

	while( !( list.empty() ) )
	{
		current = list.front() ;
		list += current->list_folders ;

		while( !( current->list_files.empty() ) )
		{
			delete current->list_files[ 0 ] ;
			current->list_files.erase( current->list_files.begin() ) ;
		}

		list.erase( list.begin() ) ;
		delete current ;
	}

	root_ = nullptr ;
}