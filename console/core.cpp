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
				directory_ = ".." ;
				interfaced_ = false ;
				plugin_ = find_plugin( "Test" ) ;
				welcomed_ = false ;
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
	delete root_ ;
	delete UI_ ;

	// release plugins
	while( !( list_plugins_.empty() ) )
	{
		delete list_plugins_[ 0 ] ;
		list_plugins_.erase( list_plugins_.begin() ) ;
	}

	// release reports
	while( !( map_reports_.empty() ) )
	{
		delete map_reports_[ 0 ] ;
		map_reports_.erase( map_reports_.begin() ) ;
	}

	plugin_ = nullptr ;
	report_ = nullptr ;
	root_ = nullptr ;
	UI_ = nullptr ;
}

void
Core::analyse_file
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

		file->analysed = true ;
	}
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
Core::create_tree_view
()
{
	// no chosen plugin or no directory
	if( directory_.isEmpty() ||
		plugin_ == nullptr )
	{
		return nullptr ;
	}

	// root value
	root_ = new CC_Folder() ;
	root_->name = directory_ ;
	root_->parent = nullptr ;

	// loop preparation
	CC_Folder * current = nullptr ;
	CC_Folder * new_folder = nullptr ;
	CC_File * new_file = nullptr ;
	QStringList list_directories ;
	QStringList list_files ;
	QStringList::const_iterator cit ;

	QList< CC_Folder * > list_folders ;
	list_folders.push_back( root_ ) ;

	while( !( list_folders.empty() ) )
	{
		// find the current folder
		current = list_folders[ 0 ] ;
		QDir directory( current->name ) ;

		// get directories and files in the folder
		list_directories =  directory.entryList( QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable | QDir::Executable ) ;
		list_files =  directory.entryList( plugin_->get_extensions(), QDir::Files | QDir::Readable ) ;

		for( cit = list_directories.constBegin() ; cit != list_directories.constEnd() ; ++cit )
		{
			// new folder
			new_folder = new CC_Folder() ;
			new_folder->parent = current ;
			new_folder->name = current->name + "/" + *cit ;

			// store the folder in its parent
			current->list_folders.push_back( new_folder ) ;

			// add the new folder to the list of folder
			list_folders.push_back( new_folder ) ;
		}

		//
		for( cit = list_files.constBegin() ; cit != list_files.constEnd() ; ++cit )
		{
			// new file
			new_file = new CC_File() ;
			new_file->name = current->name + "/" + *cit ;
			new_file->folder = current ;
			new_file->analysed = false ;

			// store the file in its folder
			current->list_files.push_back( new_file ) ;
		}

		// delete the parent folder at the end of its treatment
		list_folders.erase( list_folders.begin() ) ;
	}

	return root_ ;
}

IUI *
Core::create_UI
(
	QObject * parent
)
{
	if( interfaced_ == true )
	{
		std::cout << "Currently, there is no graphical interface available." << std::endl ;
	}
	else
	{
		UI_ = new ConsoleUI( parent, welcomed_ ) ;
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
		loader.load() ;

		// if it's the right plugin format
		if( loader.isLoaded() == true )
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

	// find if the folder was analysed before
	CC_Report * tmp = map_reports_.value( folder, nullptr ) ;

	if( tmp == nullptr )
	{
		// creation of a new report
		report_ = new CC_Report() ;

		// loop preparation
		QList< CC_Folder * > list_folder = folder->list_folders ;
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

			// analyse files of the current folder
			for( cit_File = current->list_files.begin() ; cit_File != current->list_files.end() ; ++cit_File )
			{
				// check if the file is analysed
				if( ( *cit_File )->analysed == false )
				{
					analyse_file( *cit_File ) ;
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
		map_reports_.insert( folder, report_ ) ;
	}
	else
	{
		// changement of the displayed report
		report_ = tmp ;
	}

	return report_ ;
}
