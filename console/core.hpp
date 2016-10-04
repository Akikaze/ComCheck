#ifndef CORE_HPP
#define CORE_HPP

#include "console_ui.hpp"
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

		// --- GETTERS / SETTERS ---

		///
		/// \brief Getter of the variable directory_
		/// \return Name of the directory
		///
		inline const QString get_directory() const
			{ return directory_ ; }

		///
		/// \brief Setter of the variable directory_
		/// \param directory Name of the directory
		///
		inline void set_directory( QString directory )
			{ directory_ = directory ; }

		///
		/// \brief Getter of the variable plugin_
		/// \return List of IPlugin
		///
		inline const QList< IPlugin * > get_list_plugins() const
			{ return list_plugins_ ; }

		///
		/// \brief Getter of the variable plugin_
		/// \return Map of reports
		///
		inline const QList< QPair< CC_Folder *, CC_Report * > > get_map_reports() const
			{ return map_reports_ ; }

		///
		/// \brief Getter of the variable plugin_
		/// \return IPlugin
		///
		inline const IPlugin * get_plugin() const
			{ return plugin_ ; }

		///
		/// \brief Setter of the variable plugin_
		/// \param IPlugin
		///
		inline void set_plugin( IPlugin * plugin )
			{ plugin_ = plugin ; }

		///
		/// \brief Getter of the variable report_
		/// \return CC_Report
		///
		inline const CC_Report * get_report() const
			{ return report_ ; }

		///
		/// \brief Getter of the variable root_
		/// \return CC_Folder
		///
		inline const CC_Folder * get_root() const
			{ return root_ ; }

		// --- MEMBERS ---

		///
		/// \brief Create a tree view from directory_
		/// \return Pointer on the project folder
		///
		CC_Folder * create_tree_view() ;

		///
		/// \brief Create a user interface graphical or not
		///	\return A pointer to an class which inherit from IUI
		///
		IUI * create_UI() ;

		///
		/// \brief Find a plugin associated to a language
		/// \param language String searched in the plugins' list
		/// \return Plugin which treats this language
		///
		IPlugin * find_plugin( QString language ) ;

		///
		/// \brief Make report from a specific folder
		/// \param folder Report's folder (root_ by default)
		/// \return A CC_Report of this folder
		///
		CC_Report *	make_report( CC_Folder * folder = nullptr ) ;

		///
		/// \brief Release the tree view
		///
		void release_tree() ;

	private :

		// --- MEMBERS ---

		///
		/// \brief analyze a file to fulfill its array
		/// \param file File which need to be analyzed
		///
		void analyze_file( CC_File * file ) ;

		///
		/// \brief Check if one folder is already before
		/// \param folder Folder
		/// \return A pointer to the report or nullptr
		///
		CC_Report * check_reports( CC_Folder * folder ) ;

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
		/// \brief Create a branch of the view
		/// \return A CC_Folder that represent the branch of the tree view
		///
		CC_Folder * create_branch( QString name_folder ) ;

		///
		/// \brief List all plugins
		/// \return List of plugins found in the folder /plugins
		///
		QList< IPlugin * > list_plugins() ;

		// --- ATTRIBUTES ---

		QString	directory_ ; ///< Address of project's repository
		bool interfaced_ ; ///< Signal for graphical interface
		QList< IPlugin * > list_plugins_ ; ///< List of plugins
		QList< QPair< CC_Folder *, CC_Report * > > map_reports_ ; ///< Map of reports sort by folder
		IPlugin * plugin_ ; ///< Current plugin used
		CC_Report * report_ ; ///< Current report used
		CC_Folder *	root_ ; ///< Root of the tree view
		IUI * UI_ ; ///< User interface
		bool welcomed_ ; ///< Display a nice welcoming message

} ;

#endif // CORE_HPP
