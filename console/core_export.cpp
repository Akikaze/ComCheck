#include "core.hpp"

///
/// \brief Export a CSS file for HTML version of report
/// \param folder Name of the folder
///
void
Core::export_CSS
(
	QString folder
)
{
	std::ofstream ofs( ( folder + "/CC_style.css" ).toStdString() ) ;
	if( ofs )
	{
		std::string indent = "" ;

		// body {
		ofs << indent << "body {" << std::endl ;
		indent += '\t' ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// h1, h1 {
		ofs << indent << "h1, h2 {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "text-align : center ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// h3 {
		ofs << indent << "h3 {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "border-top : 2px solid #000000 ;" << std::endl ;
			ofs << indent << "padding-top : 20px ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// h5 {
		ofs << indent << "h5 {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "margin : 0 ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// input {
		ofs << indent << "input {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "float : right ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// table, th, td {
		ofs << indent << "table, th, td {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "border : 1px solid #000000 ;" << std::endl ;
			ofs << indent << "border-collapse : collapse ;" << std::endl ;
			ofs << indent << "margin-left : auto ;" << std::endl ;
			ofs << indent << "margin-right : auto ;" << std::endl ;
			ofs << indent << "padding : 10px ;" << std::endl ;
			ofs << indent << "text-align : center ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// table.sortable thead {
		ofs << indent << "table.sortable thead {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "background-color	: #333333 ;" << std::endl ;
			ofs << indent << "color : #666666 ;" << std::endl ;
			ofs << indent << "cursor : pointer ;" << std::endl ;
			ofs << indent << "font-weight : bold ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// #tree_view {
		ofs << indent << "#tree_view {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "padding-left : 5px ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// #histogram {
		ofs << indent << "#histogram {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "height : 80% ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// .show_hide {
		ofs << indent << ".show_hide {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "display : none ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// .red {
		ofs << indent << ".red {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "color : #A30000 ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// .green {
		ofs << indent << ".green {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "color : #00A300 ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// .blue {
		ofs << indent << ".blue {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "color : #0000A3 ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;
	}
	else
	{
		std::cout << "Impossible to create the CSS file." << std::endl ;
	}
}

///
/// \brief Export an histogram for HTML version of report
/// \param ofs Stream for the HTML
/// \param report Pointer on the report
///
void
Core::export_histogram
(
	std::ostream & ofs,
	CC_Report * report,
	std::string indent
)
{
	Q_UNUSED( ofs ) ;
	Q_UNUSED( report ) ;
	Q_UNUSED( indent ) ;
}

///
/// \brief Export a CC_Report * in HTML
///
void
Core::export_HTML
()
{
}

///
/// \brief Export a name for HTML tree view
/// \param name Name of the file or the folder
/// \param isFile Check if the name should be colored or not
/// \param level Size of indentation
/// \return Standard string which contain the name and the right prefix
///
std::string
Core::export_name
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
		name = "<span class=\"blue\">" + name + "</span>" ;
	}

	// default prefix
	std::string prefix = "&nbsp;" ;

	if( level != 0 )
	{
		// this case is for align the first line
		prefix += "|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" ;
	}

	for( unsigned int i = 1 ; i < level ; ++i )
	{
		// add as much prefix as the level
		prefix += "|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" ;
	}

	// add the last prefix
	prefix += "|---&nbsp;" ;

	prefix += name.toStdString() ;
	prefix = "<h5>" + prefix + "</h5>" ;

	// bufferize the name with the prefix
	return prefix ;
}

void
Core::export_tree_view
(
	std::ostream & ofs,
	CC_Folder * folder,
	std::string indent,
	unsigned int level
)
{
	// display files in first
	QList< CC_File * > list_files = folder->list_files ;
	for( int i = 0 ; i < list_files.size() ; ++i )
	{
		ofs << indent << export_name( list_files[ i ]->name, true, level ) << std::endl ;
	}

	// and folders after
	QList< CC_Folder * > list_folders = folder->list_folders ;
	for( int i = 0 ; i < list_folders.size() ; ++i )
	{
		// display the name
		ofs << indent << export_name( list_folders[ i ]->name, false, level ) << std::endl ;

		// and be recursive
		export_tree_view( ofs, list_folders[ i ], indent, level + 1 ) ;
	}
}
