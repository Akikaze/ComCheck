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
/*
	// js script
	ofs << indent << "<script>" << std::endl ;
	indent += '\t' ;
	ofs << indent << std::endl ;

		double average = report->average ;
		QList< CC_File * >::const_iterator cit_list ;
		QList< double >::const_iterator cit_double ;
		QStringList legend = {} ;
		QStringList::const_iterator cit_files ;

		for( cit_list = report->list_files.constBegin() ;
			 cit_list != report->list_files.constEnd() ;
			 ++cit_list )
		{
			legend.push_back( ( *cit_list )->name ) ;
		}

		ofs << indent << "var average = {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "x: [ " ;
			for( cit_files = legend.constBegin() ; cit_files != legend.constEnd() ; ++cit_files )
			{
				ofs << "'" << ( *cit_files ).toStdString() << "' ," ;
			}
			ofs << " ]," << std::endl ;

			ofs << indent << "y: [ " ;
			for( cit_files = legend.constBegin() ; cit_files != legend.constEnd() ; ++cit_files )
			{
				ofs << average << " ," ;
			}
			ofs << " ]," << std::endl ;
			ofs << indent << "name: 'Average'," << std::endl ;
			ofs << indent << "type: 'scatter'" << std::endl ;

		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "} ;" << std::endl ;
		ofs << indent << std::endl ;

		ofs << indent << "var values = {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "x: [ " ;
			for( cit_files = legend.constBegin() ; cit_files != legend.constEnd() ; ++cit_files )
			{
				ofs << "'" << ( *cit_files ).toStdString() << "' ," ;
			}
			ofs << " ]," << std::endl ;

			ofs << indent << "y: [ " ;
			for( cit_double = report->percents.constBegin() ; cit_double != report->percents.constEnd() ; ++cit_double )
			{
				ofs << ( *cit_double ) << " ," ;
			}
			ofs << " ]," << std::endl ;
			ofs << indent << "marker: { color: [ " ;
			for( cit_double = report->percents.constBegin() ; cit_double != report->percents.constEnd() ; ++cit_double )
			{
				ofs << "'rgba( " ;

				if( ( *cit_double ) > 1.2 * average )
				{
					ofs << "0, 163, 0, 1.0" ;
				}
				else if( ( *cit_double ) < 0.8 * average )
				{
					ofs << "163, 0, 0, 1.0" ;
				}
				else
				{
					ofs << "80, 80, 80, 1.0" ;
				}

				ofs << " )', " ;
			}
			ofs << indent << " ] }," << std::endl ;
			ofs << indent << "name: 'Percentage'," << std::endl ;
			ofs << indent << "type: 'bar'" << std::endl ;

		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "} ;" << std::endl ;
		ofs << indent << std::endl ;

		ofs << indent << "var data = [ values, average ] ;" << std::endl ;
		ofs << indent << std::endl ;

		ofs << indent << "var layout = [ {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "title: 'Comparison between every file by using the percentage of commented line.'," << std::endl ;
			ofs << indent << "xaxis: { title: 'File' }," << std::endl ;
			ofs << indent << "yaxis: { title: 'Percent' }," << std::endl ;

		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "} ] ;" << std::endl ;
		ofs << indent << std::endl ;

		// create histogram before ...
		ofs << indent << "Plotly.newPlot( 'histogram', data, layout ) ;" << std::endl ;
		ofs << indent << std::endl ;

		// ... and hide it after (perfect size with this trick)
		ofs << indent << "$( '#histogram' ).toggleClass( 'show_hide' ) ;" << std::endl ;

	ofs << indent << std::endl ;
	indent = indent.substr( 0, indent.size() - 1 ) ;
	ofs << indent << "</script>" << std::endl ;
*/
}

///
/// \brief Export a CC_Report * in HTML
///
void
Core::export_HTML
()
{
	if( !( map_reports_.empty() ) )
	{
		bool result = false ;

		// get time
		std::time_t date = std::time( nullptr );
		char string_time[18];
		strftime( string_time, 18, "%d_%m_%y_%H_%M_%S", std::localtime( &date ) );
		string_time[ 17 ] = '\0' ;

		// get folder name
		QString root = "./" ;
		QString name = "export_" ;
		name += string_time ;

		// creation of the folder
		QDir folder( root ) ;
		result = folder.mkdir( name ) ;

		if( result == true )
		{
			export_CSS( root + name ) ;

			QList< QPair< CC_Folder *, CC_Report * > >::const_iterator cit ;
			for( cit = map_reports_.constBegin() ; cit != map_reports_.constEnd() ; ++cit )
			{
				QString file = ( *cit ).first->name ;
				file = file.replace( '/', '_' ) ;
				file += ".html" ;

				file = root + name + '/' + file ;

				std::ofstream ofs( file.toStdString(), std::ios::out ) ;
				if( ofs )
				{
					QList< CC_File * > list_files ;
					QList< CC_File * >::const_iterator cit_list ;
					double average = ( *cit ).second->average ;
					std::string indent = "" ;

					// <html>
					ofs << indent << "<html>" << std::endl ;
					indent += '\t' ;

						// <head>
						ofs << indent << std::endl ;
						ofs << indent << "<head>" << std::endl ;
						indent += '\t' ;

							// title + css + js
							ofs << indent << std::endl ;
							ofs << indent << "<meta charset=\"utf-8\" />" << std::endl ;
							ofs << indent << "<title>ComCheck report: " << ( *cit ).first->name.toStdString() << "</title>" << std::endl ;
							ofs << indent << "<link rel=\"stylesheet\" href=\"CC_style.css\" />" << std::endl ;
							ofs << indent << "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js\"></script>" << std::endl ;
							ofs << indent << "<script src=\"http://www.kryogenix.org/code/browser/sorttable/sorttable.js\"></script>" << std::endl ;
							ofs << indent << "<script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>" << std::endl ;
							ofs << indent << std::endl ;

						// </head>
						indent = indent.substr( 0, indent.size() - 1 ) ;
						ofs << indent << "</head>" << std::endl ;

						ofs << indent << std::endl ;

						// <body>
						ofs << indent << "<body>" << std::endl ;
						indent += '\t' ;

							// h1 + h2
							ofs << indent << std::endl ;
							ofs << indent << "<h1>ComCheck report</h1>" << std::endl ;
							ofs << indent << "<h2>" << ( *cit ).first->name.toStdString() << "</h2>" << std::endl ;

							ofs << indent << std::endl ;

							// <table>
							ofs << indent << "<table>" << std::endl ;
							indent += '\t' ;

								// <tr>
								ofs << indent << "<tr>" << std::endl ;
								indent += '\t' ;

									ofs << indent << "<th rowspan=2>Lines</th>" << std::endl ;
									ofs << indent << "<th>Total number</th>" << std::endl ;
									ofs << indent << "<th>Comment lines</th>" << std::endl ;
									ofs << indent << "<th>Mixed lines</th>" << std::endl ;
									ofs << indent << "<th>Pure code lines</th>" << std::endl ;

								// </tr>
								indent = indent.substr( 0, indent.size() - 1 ) ;
								ofs << indent << "</tr>" << std::endl ;

								// <tr>
								ofs << indent << "<tr>" << std::endl ;
								indent += '\t' ;

									// display array value
									ofs << indent << "<td>" << ( *cit ).second->array[ 0 ] << "</td>" << std::endl ;
									ofs << indent << "<td>" << ( *cit ).second->array[ 1 ] << "</td>" << std::endl ;
									ofs << indent << "<td>" << ( *cit ).second->array[ 2 ] << "</td>" << std::endl ;
									ofs << indent << "<td>" << ( *cit ).second->array[ 3 ] << "</td>" << std::endl ;

								// </tr>
								indent = indent.substr( 0, indent.size() - 1 ) ;
								ofs << indent << "</tr>" << std::endl ;

							// </table>
							indent = indent.substr( 0, indent.size() - 1 ) ;
							ofs << indent << "</table>" << std::endl ;

							// <br/>
							ofs << indent << "<br/>" << std::endl ;

							// <table>
							ofs << indent << "<table>" << std::endl ;
							indent += '\t' ;

								// <tr>
								ofs << indent << "<tr>" << std::endl ;
								indent += '\t' ;

									ofs << indent << "<th rowspan=2>Statistics</th>" << std::endl ;
									ofs << indent << "<th>Average</th>" << std::endl ;
									ofs << indent << "<th>Variance</th>" << std::endl ;
									ofs << indent << "<th>Divergence</th>" << std::endl ;

								// </tr>
								indent = indent.substr( 0, indent.size() - 1 ) ;
								ofs << indent << "</tr>" << std::endl ;

								// <tr>
								ofs << indent << "<tr>" << std::endl ;
								indent += '\t' ;

									// display report compute value
									ofs << indent << "<td>" << ( *cit ).second->average << " %</td>" << std::endl ;
									ofs << indent << "<td>" << ( *cit ).second->variance << "</td>" << std::endl ;
									ofs << indent << "<td>" << ( *cit ).second->divergence << "</td>" << std::endl ;

								// </tr>
								indent = indent.substr( 0, indent.size() - 1 ) ;
								ofs << indent << "</tr>" << std::endl ;

							// </table>
							indent = indent.substr( 0, indent.size() - 1 ) ;
							ofs << indent << "</table>" << std::endl ;

							ofs << indent << "<br/>" << std::endl ;

							// h3
							ofs << indent << "<h3>Long description</h3>" << std::endl ;
							ofs << indent << "<input type=\"button\" onclick=\"$( '#long_description' ).toggleClass( 'show_hide' ) ;\" value=\"show / hide\" />" << std::endl ;
							ofs << indent << "<br/><br/>" << std::endl ;

							// <div id="long_description" class="show_hide">
							ofs << indent << "<div id=\"long_description\" class=\"show_hide\">" << std::endl ;
							indent += '\t' ;

								// Description of what type of comments, doxygen, todo, ...
								ofs << indent << "Nothing here for now. But soon !" << std::endl ;

							// </div
							indent = indent.substr( 0, indent.size() - 1 ) ;
							ofs << indent << "</div>" << std::endl ;

							ofs << indent << std::endl ;

							// h3
							ofs << indent << "<h3>Tree view</h3>" << std::endl ;
							ofs << indent << "<input type=\"button\" onclick=\"$( '#tree_view' ).toggleClass( 'show_hide' ) ;\" value=\"show / hide\" />" << std::endl ;
							ofs << indent << "<br/><br/>" << std::endl ;

							// <div id="tree_view" class="show_hide">
							ofs << indent << "<div id=\"tree_view\" class=\"show_hide\">" << std::endl ;
							indent += '\t' ;

								ofs << indent << "<h5 class=\"blue\">" << ( *cit ).first->name.toStdString() << "</h5>" << std::endl ;
								export_tree_view( ofs, ( *cit ).first, indent ) ;

							// </div>
							indent = indent.substr( 0, indent.size() - 1 ) ;
							ofs << indent << "</div>" << std::endl ;

							ofs << indent << std::endl ;

							// h3
							ofs << indent << "<h3>List files</h3>" << std::endl ;
							ofs << indent << "<input type=\"button\" onclick=\"$( '#list_files' ).toggleClass( 'show_hide' ) ;\" value=\"show / hide\" />" << std::endl ;
							ofs << indent << "<br/><br/>" << std::endl ;

							// <div id="list_files" class="show_hide">
							ofs << indent << "<div id=\"list_files\" class=\"show_hide\">" << std::endl ;
							indent += '\t' ;

								// <table class="sortable">
								ofs << indent << "<table class=\"sortable\">" << std::endl ;
								indent += '\t' ;

									// <tr>
									ofs << indent << "<tr>" << std::endl ;
									indent += '\t' ;

										ofs << indent << "<th>File</th>" << std::endl ;
										ofs << indent << "<th>Total number</th>" << std::endl ;
										ofs << indent << "<th>Comment lines</th>" << std::endl ;
										ofs << indent << "<th>Mixed lines</th>" << std::endl ;
										ofs << indent << "<th>Pure code lines</th>" << std::endl ;
										ofs << indent << "<th>Percentage</th>" << std::endl ;

									// </tr>
									indent = indent.substr( 0, indent.size() - 1 ) ;
									ofs << indent << "</tr>" << std::endl ;

									list_files = ( *cit ).second->list_files ;
									for( cit_list = list_files.constBegin() ; cit_list != list_files.constEnd() ; ++cit_list )
									{

										// <tr>
										ofs << indent << "<tr>" << std::endl ;
										indent += '\t' ;

											// display colored name
											ofs << indent << "<td " ;

											if( ( *cit_list )->com_tot < 0.8 * average )
											{
												ofs << "class=\"red\">" ;
											}
											else if( ( *cit_list )->com_tot > 1.2 * average )
											{
												ofs << "class=\"green\">" ;
											}
											else
											{
												ofs << ">" ;
											}

											ofs << ( *cit_list )->name.toStdString() << "</td>" << std::endl ;

											// display array value
											ofs << indent << "<td>" << ( *cit_list )->array[ 0 ] << "</td>" << std::endl ;
											ofs << indent << "<td>" << ( *cit_list )->array[ 1 ] << "</td>" << std::endl ;
											ofs << indent << "<td>" << ( *cit_list )->array[ 2 ] << "</td>" << std::endl ;
											ofs << indent << "<td>" << ( *cit_list )->array[ 3 ] << "</td>" << std::endl ;
											ofs << indent << "<td>" << ( *cit_list )->com_tot << " %</td>" << std::endl ;

										// </tr>
										indent = indent.substr( 0, indent.size() - 1 ) ;
										ofs << indent << "</tr>" << std::endl ;

									}

								// </table>
								indent = indent.substr( 0, indent.size() - 1 ) ;
								ofs << indent << "</table>" << std::endl ;

							// </div
							indent = indent.substr( 0, indent.size() - 1 ) ;
							ofs << indent << "</div>" << std::endl ;

							ofs << indent << std::endl ;

							// h3
							ofs << indent << "<h3>Histogram</h3>" << std::endl ;
							ofs << indent << "<input type=\"button\" onclick=\"$( '#histogram' ).toggleClass( 'show_hide' ) ;\" value=\"show / hide\" />" << std::endl ;
							ofs << indent << "<br/><br/>" << std::endl ;

							// <div id="histogram">
							ofs << indent << "<div id=\"histogram\">" << std::endl ;
							indent += '\t' ;

								export_histogram( ofs, ( *cit ).second, indent ) ;

							// </div>
							indent = indent.substr( 0, indent.size() - 1 ) ;
							ofs << indent << "</div>" << std::endl ;

							ofs << indent << std::endl ;

							// <h3>
							ofs << indent << "<h3></h3>" << std::endl ;

						// </body>
						indent = indent.substr( 0, indent.size() - 1 ) ;
						ofs << indent << "</body>" << std::endl ;

					// </html>
					indent = indent.substr( 0, indent.size() - 1 ) ;
					ofs << indent << "</html>" ;
				}
				else
				{
					std::cout << "Impossible to create the file: " << file.toStdString() << "." << std::endl ;
				}
			}
		}
		else
		{
			std::cout << "Impossible to create the folder: " << ( root + name ).toStdString() << "." << std::endl ;
		}
	}
	else
	{
		std::cout << "No reports to export." << std::endl ;
	}
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
