#ifndef CORE_HPP
#define CORE_HPP

#include "iplugin.hpp"

class Core
: public QCoreApplication
{
	public :

		// --- CONSTRUCTORS ---

		///
		/// \brief Constructor
		/// \param argc Counter for parameter
		/// \param argv Array of string which are containing parameters
		///
		Core( int & argc, char ** argv ) ;

		// --- DESTRUCTORS ---

		///
		/// \brief Destructor
		///
		~Core() ;

		// --- MEMBERS ---

		///
		/// \brief Create a tree view from directory_
		/// \return A CC_Folder that represent the root of the tree view
		///
		CC_Folder *	create_tree_view() ;


		///
		/// \brief Make report from a specific folder
		/// \param folder Report's folder (root_ by default)
		/// \return A CC_Report of this folder
		///
		CC_Report *	make_report( CC_Folder * folder = nullptr ) ;

	private :

		// --- MEMBERS ---

		///
		/// \brief Analyse a file to fulfill its array
		/// \param file File which need to be analysed
		///
		void analyse_file( CC_File * file ) ;

		///
		/// \brief Clear all spaces in a line
		/// \param line Reference on the line
		///
		void clear_line( std::string & line ) ;

		///
		/// \brief Compute statistical value of a report
		/// \param report Report without statistical value
		///
		void compute_report( CC_Report * report ) ;

		///
		/// \brief Find a plugin associated to a language
		/// \param language String searched in the plugins' list
		/// \return Plugin which treats this language
		///
		IPlugin * find_plugin( QString language ) ;

		///
		/// \brief List all plugins
		/// \return List of plugins found in the folder /plugins
		///
		QList< IPlugin * > list_plugins() ;

		// --- ATTRIBUTES ---

		QString	directory_ ; ///< Address of project's repository
		bool interfaced_ ; ///< If signal are need
		QList< IPlugin * > list_plugins_ ; ///< List of plugins
		QMap< CC_Folder *, CC_Report * > map_reports_ ; ///< Map of reports sort by folder
		IPlugin * plugin_ ; ///< Current plugin used
		CC_Report * report_ ; ///< Current report used
		CC_Folder *	root_ ; ///< Root of the tree view

} ;

#endif // CORE_HPP
