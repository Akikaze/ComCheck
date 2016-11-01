#include "core.hpp"

///
/// \brief Constructor
/// \param argc Counter for parameter
/// \param argv Array of string which are containing parameters
///
Core::Core
(
	int & argc,
	char ** argv
)
: QCoreApplication( argc, argv )
, automatic_( false )
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
			// automatic
			if( QString( argv[ i ] ) == "-a" ||
				QString( argv[ i ] ) == "--automatic" )
			{
				if( !( directory_.isEmpty() ) && plugin_ != nullptr )
				{
					automatic_ = true ;

					create_tree_view() ;
					if( root_ != nullptr )
					{
						make_report() ;
						export_HTML() ;
					}
					else
					{
						std::cout << "The tree view is empty." << std::endl ;
					}
				}
			}

			// directory
			if( QString( argv[ i ] ) == "-d" ||
				QString( argv[ i ] ) == "--directory" )
			{
				if( argc > i + 1 )
				{
					directory_ = argv[ ++i ] ;

					if( directory_[ directory_.length() - 1] == '/' )
					{
						directory_ = directory_.left( directory_.length() - 1 ) ;
					}

					// check if the directory exists
					QDir test( directory_ ) ;
					if( !( test.exists() ) )
					{
						std::cout << directory_.toStdString() << " is not found by the system." << std::endl ;
						directory_ = "" ;
					}
				}
			}

			// language
			if( QString( argv[ i ] ) == "-l" ||
				QString( argv[ i ] ) == "--language" )
			{
				if( argc > i + 1 )
				{
					plugin_ = find_plugin( argv[ ++i ] ) ;
				}

				if( plugin_ == nullptr )
				{
					std::cout << argv[ i ] << " is not found in the list of plugins." << std::endl ;
				}
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
				directory_ = "../console" ;
				interfaced_ = false ;
				plugin_ = find_plugin( "C++" ) ;
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

///
/// \brief Destructor
///
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
	if( !( map_reports_.empty() ) )
	{
		release_reports() ;
	}

	// release tree view
	if( root_ != nullptr )
	{
		release_tree() ;
	}

	// delete the interface
	delete UI_ ;

	// cancel every pointer
	plugin_ = nullptr ;
	report_ = nullptr ;
	UI_ = nullptr ;
}

///
/// \brief analyze a file to fulfill its array
/// \param file File which need to be analyzed
///
void
Core::analyze_file
(
	CC_File * file
)
{
	// open the file
	std::ifstream ifs( file->name.toStdString(), std::ios::in ) ;

	if( ifs )
	{
		double weight_code = 0 ;
		double weight_comment = 0 ;

		std::string line ;

		while( std::getline( ifs, line ) )
		{
			clear_line( line ) ; // remove all useless characters

			if( !( line.empty() ) )
			{
				CC_Line flag = plugin_->get_type( line ) ;// get back information about the line
				file->type[ 0 ] += 1 ; // add a line

				if( flag.type != ERROR )
				{
					file->type[ flag.type ] += 1 ; // add info about the line

					// compute weight

					switch( flag.type )
					{
						case NO_COMMENT :

							weight_code += 1 ;
							break ;

						case ONLY_COMMENT :

							file->description[ flag.description ] += 1 ; // add the description

							if( weight_code != 0 )
							{
								file->weight.push_back( weight_comment * 100 / weight_code ) ;

								weight_code = 0 ;
								weight_comment = 0 ;
							}

							if( flag.description == NORMAL )
							{
								weight_comment += 1 ;
							}
							else if( flag.description == EVOLUTION )
							{
								weight_comment += 0.4 ;
							}
							else if( flag.description == PROBLEM )
							{
								weight_comment += 0.7 ;
							}
							else if( flag.description == TEMPORARY )
							{
								weight_comment += 0.1 ;
							}
							else if( flag.description == DOCUMENTATION )
							{
								weight_comment += 0.2 ;
							}

							break ;

						case MIX_LINE :

							file->description[ flag.description ] += 1 ;
							weight_code += 1 ;

							// the type of comment is not interesting
							if( flag.description > HEADER )
							{
								weight_comment += 1 ;
							}

							break ;

						default :
							break ;
					}
				}
			}
		}
		ifs.close() ;

		if( weight_code != 0 )
		{
			file->weight.push_back( weight_comment * 100 / weight_code ) ;
		}

		// compute last values and signal that this file is already analyzed
		file->percentage = 0 ;

		if( file->type[ 0 ] != 0 )
		{
			file->percentage = ( double )( file->type[ 2 ] + file->type[ 3 ] ) * 100 / ( double )( file->type[ 0 ] ) ;
		}

		file->coverage = CC_Statistics( file->weight ) ;
		file->analyzed = true ;
	}
}

///
/// \brief Check if one folder is already before
/// \param folder Folder
/// \return A pointer to the report or nullptr
///
CC_Report *
Core::check_reports
(
	CC_Folder * folder
)
{
	CC_Report * result = nullptr ;
	QList< QPair< CC_Folder *, CC_Report * > >::const_iterator cit = map_reports_.constBegin() ;

	// go through every report
	while( cit != map_reports_.constEnd() && result == nullptr )
	{
		// check if this folder was analyzed previously
		if( ( *cit ).first == folder )
		{
			// avoid a second useless analyze
			result = ( *cit ).second ;
		}
		else
		{
			++cit ;
		}
	}

	return result ;
}

///
/// \brief Clear all spaces in a line
/// \param line Reference on the line
///
void
Core::clear_line
(
	std::string & line
)
{
	unsigned int i = 0 ;
	unsigned int length = line.length() ;

	// erase any thing useless
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

///
/// \brief Compute statistical value of a report
/// \param report Report without statistical value
///
void
Core::compute_report
(
	CC_Report * report
)
{
	report->coverage = CC_Statistics( report->coverages ) ;
	report->percentage = CC_Statistics( report->percentages ) ;
}

///
/// \brief Create a branch of the view
/// \return A CC_Folder that represent the branch of the tree view
///
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
		// create a branch recursively
		folder = create_branch( name_folder + "/" + *cit ) ;

		// useless folders must not appears in the tree view
		if( folder != nullptr )
		{
			list_folders.push_back( folder ) ;
		}
	}

	// if there is something interesting in this folder
	if( ! list_files.empty() || ! list_folders.empty() )
	{
		// store the folder
		folder = new CC_Folder() ;
		folder->name = name_folder ;

		// signal the folder of each file interesting in this folder
		for( cit_File = list_files.begin() ; cit_File != list_files.end() ; ++cit_File )
		{
			( *cit_File )->folder = folder ;
		}

		// signal the parent of each folder interesting in this one
		for( cit_Folder = list_folders.begin() ; cit_Folder != list_folders.end() ; ++cit_Folder )
		{
			( *cit_Folder )->parent = folder ;
		}

		// store its files
		folder->list_files = list_files ;
		// store its interesting folders
		folder->list_folders = list_folders ;

	}

	return folder ;
}

///
/// \brief Create a tree view from directory_
/// \return Pointer on the project folder
///
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
	// directory = ;
#endif

	// create the tree view from the root
	root_ = create_branch( directory_ ) ;

	return root_ ;
}

///
/// \brief Create a user interface graphical or not
///	\return A pointer to an class which inherit from IUI
///
IUI *
Core::create_UI
()
{
	// check if it's an automatic execution
	if( automatic_ == false )
	{
		// check if the graphical interface is installed
		if( interfaced_ == true )
		{
			std::cout << "Currently, there is no graphical interface available." << std::endl ;
		}
		else
		{
			// use the console interface
			UI_ = new ConsoleUI( this, welcomed_ ) ;
		}
	}

	return UI_ ;
}

///
/// \brief Find a plugin associated to a language
/// \param language String searched in the plugins' list
/// \return Plugin which treats this language
///
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

///
/// \brief List all plugins
/// \return List of plugins found in the folder /plugins
///
QList< IPlugin * >
Core::list_plugins
()
{
	QList< IPlugin * > list ;
	IPlugin * plugin = nullptr ;

	// check in the right directory directly
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
			// create a plugin thanks to the loader instance
			plugin = qobject_cast< IPlugin * >( loader.instance() ) ;
			list.push_back( plugin ) ;
		}
	}

	return list ;
}

///
/// \brief Make report from a specific folder
/// \param folder Report's folder (root_ by default)
/// \return A CC_Report of this folder
///
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
		QList< CC_Folder * > list_folder ;
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

				// store file information in the report
				report_->list_files.push_back( *cit_File ) ;
				report_->percentages.push_back( ( *cit_File )->percentage ) ;
				report_->coverages.push_back( ( *cit_File )->coverage.average ) ;

				report_->type += ( *cit_File )->type ;
				report_->description += ( *cit_File )->description ;
			}

		}

		// compute average and deviation
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

///
/// \brief Release every report
///
void
Core::release_reports
()
{
	while( !( map_reports_.empty() ) )
	{
		delete ( map_reports_[ 0 ].second ) ;
		map_reports_.erase( map_reports_.begin() ) ;
	}

	report_ = nullptr ;
}

///
/// \brief Release the tree view
///
void
Core::release_tree
()
{
	CC_Folder * current = nullptr ;
	QList< CC_Folder * > list ;

	// if there is a tree view
	if( root_ != nullptr )
	{
		list = { root_ } ;
	}

	while( !( list.empty() ) )
	{
		// for a specific folder
		current = list.front() ;

		// add its folders in the list
		list += current->list_folders ;

		// erase every file in this folder
		while( !( current->list_files.empty() ) )
		{
			delete current->list_files[ 0 ] ;
			current->list_files.erase( current->list_files.begin() ) ;
		}

		list.erase( list.begin() ) ;

		// erase the folder
		delete current ;
	}

	root_ = nullptr ;
}
