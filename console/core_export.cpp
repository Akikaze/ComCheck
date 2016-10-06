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

		// h1 {
		ofs << indent << "h1 {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "text-align : center ;" << std::endl ;

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
			ofs << indent << "margin_right : auto ;" << std::endl ;
			ofs << indent << "padding : 10px ;" << std::endl ;
			ofs << indent << "text-align : center ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// .red {
		ofs << indent << ".red {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "color : #FF0000 ;" << std::endl ;

		// }
		indent = indent.substr( 0, indent.size() - 1 ) ;
		ofs << indent << "}" << std::endl ;

		ofs << indent << std::endl ;

		// .green {
		ofs << indent << ".green {" << std::endl ;
		indent += '\t' ;

			ofs << indent << "color : #00FF00 ;" << std::endl ;

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
			export_JS( root + name ) ;

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
							ofs << indent << "<title>ComCheck report: " << ( *cit ).first->name.toStdString() << "</title>" << std::endl ;
							ofs << indent << "<link rel=\"stylesheet\" href=\"CC_style.css\" />" << std::endl ;
							ofs << indent << "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jquery/2.1.3/jquery.min.js\"></script>" << std::endl ;
							ofs << indent << "<script src=\"CC_script.js\"></script>" << std::endl ;
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

							// <div id="short_description">
							ofs << indent << "<div id=\"short_description\">" << std::endl ;
							indent += '\t' ;

								// h3
								ofs << indent << "<h3>Short description</h3>" << std::endl ;

								// <table>
								ofs << indent << "<table>" << std::endl ;
								indent += '\t' ;

									// <tr>
									ofs << indent << "<tr>" << std::endl ;
									indent += '\t' ;

										ofs << indent << "<th>Total number of lines</th>" << std::endl ;
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

								// <table>
								ofs << indent << "<table>" << std::endl ;
								indent += '\t' ;

									// <tr>
									ofs << indent << "<tr>" << std::endl ;
									indent += '\t' ;

										ofs << indent << "<th>Average</th>" << std::endl ;
										ofs << indent << "<th>Divergence</th>" << std::endl ;
										ofs << indent << "<th>Variance</th>" << std::endl ;

									// </tr>
									indent = indent.substr( 0, indent.size() - 1 ) ;
									ofs << indent << "</tr>" << std::endl ;

									// <tr>
									ofs << indent << "<tr>" << std::endl ;
									indent += '\t' ;

										// display report compute value
										ofs << indent << "<td>" << ( *cit ).second->average << "</td>" << std::endl ;
										ofs << indent << "<td>" << ( *cit ).second->divergence << "</td>" << std::endl ;
										ofs << indent << "<td>" << ( *cit ).second->variance << "</td>" << std::endl ;

									// </tr>
									indent = indent.substr( 0, indent.size() - 1 ) ;
									ofs << indent << "</tr>" << std::endl ;

								// </table>
								indent = indent.substr( 0, indent.size() - 1 ) ;
								ofs << indent << "</table>" << std::endl ;

							// </div
							indent = indent.substr( 0, indent.size() - 1 ) ;
							ofs << indent << "</div>" << std::endl ;

							ofs << indent << std::endl ;

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
/// \brief Export a JS file for HTML version of report
/// \param folder Name of the folder
///
void
Core::export_JS
(
	QString folder
)
{
	std::ofstream ofs( ( folder + "/CC_script.js" ).toStdString() ) ;
	if( ofs )
	{

	}
	else
	{
		std::cout << "Impossible to create the JS file." << std::endl ;
	}
}
