#ifndef IPLUGIN_HPP
#define IPLUGIN_HPP

#include <QtPlugin>

#include "structures.hpp"

class IPlugin
{
	public :

		// --- DESTRUCTORS ---

		///
		/// \brief Destructor
		///
		virtual ~IPlugin() {}

		// --- GETTERS ---

		///
		/// \brief Getter on extensions
		/// \return Extensions understood by the plugin
		///
		inline const QStringList get_extensions() const
			{ return extensions_ ; }

		///
		/// \brief Getter on the language
		/// \return The language of the plugin
		///
		inline const QString get_language() const
			{ return language_ ; }

		///
		/// \brief Getter on the rank
		/// \return The rank of the plugin
		///
		inline unsigned short get_rank() const
			{ return rank_ ; }

		///
		/// \brief Get the type of a line
		/// \param Line of the file
		/// \return CC_Line which describe the line
		///
		virtual CC_Line get_type( const std::string & line ) = 0 ;


	protected :

		// --- GETTERS ---

		///
		/// \brief Get the description of a comment
		/// \param Comment in a line
		/// \return CC_Desc
		///
		virtual CC_Desc get_description( const std::string & comment ) = 0 ;

		// --- SETTERS ---

		///
		/// \brief Set prefix in order to describe a comment
		///
		inline void set_prefix()
		{
			/*
			 * In a normal case, I will never put code in a header file.
			 * But this case is not normal, I cannot use a .cpp in the interface
			 * of a plugin. This is why I put the whole function here.
			 */

			std::ifstream ifs( "prefix.txt" ) ;

			if( ifs )
			{
				std::string line ;
				QStringList list ;

				std::getline( ifs, line ) ; // comment line
				std::getline( ifs, line ) ; // empty line

				/*
				 * The process is always the same :
				 * 1 - divide the line into words
				 * 2 - erase the first one because it's only a description for the user
				 * 3 - store every object in the prefix list associated to the right description
				 */

				// --- USELESS ---

				std::getline( ifs, line ) ; // empty line
				list = QString( line.c_str() ).split( ' ' ) ; // 1
				list.erase( list.begin() ) ; // 2

				while( !( list.empty() ) ) // 3
				{
					prefix_.push_back( qMakePair( list.first(), USELESS ) ) ;
					list.erase( list.begin() ) ;
				}

				// --- HEADER ---

				std::getline( ifs, line ) ; // empty line
				list = QString( line.c_str() ).split( ' ' ) ; // 1
				list.erase( list.begin() ) ; // 2

				while( !( list.empty() ) ) // 3
				{
					prefix_.push_back( qMakePair( list.first(), HEADER ) ) ;
					list.erase( list.begin() ) ;
				}

				// --- PROBLEM ---

				std::getline( ifs, line ) ; // empty line
				list = QString( line.c_str() ).split( ' ' ) ; // 1
				list.erase( list.begin() ) ; // 2

				while( !( list.empty() ) ) // 3
				{
					prefix_.push_back( qMakePair( list.first(), BUGS ) ) ;
					list.erase( list.begin() ) ;
				}

				// --- TEMPORARY ---

				std::getline( ifs, line ) ; // empty line
				list = QString( line.c_str() ).split( ' ' ) ; // 1
				list.erase( list.begin() ) ; // 2

				while( !( list.empty() ) ) // 3
				{
					prefix_.push_back( qMakePair( list.first(), TEMPORARY ) ) ;
					list.erase( list.begin() ) ;
				}

				// --- EVOLUTION ---

				std::getline( ifs, line ) ; // empty line
				list = QString( line.c_str() ).split( ' ' ) ; // 1
				list.erase( list.begin() ) ; // 2

				while( !( list.empty() ) ) // 3
				{
					prefix_.push_back( qMakePair( list.first(), EVOLUTION ) ) ;
					list.erase( list.begin() ) ;
				}

				// close the file
				ifs.close() ;
			}
			else
			{
				std::cout << "Impossible to load prefix from the config file." << std::endl ;
				std::cin.get() ;
			}
		}

		// --- ATTRIBUTES ---

		QStringList extensions_ ; ///< Extension analysed by the plugin
		QString	language_ ; ///< Language of the plugin
		QList< QPair< QString, CC_Desc > > prefix_ ; ///< List of prefix associated to description
		unsigned short rank_ ; ///< Rank of the plugin (by default 65534)
} ;

Q_DECLARE_INTERFACE( IPlugin, "ComCheck.IPlugin" )

class Comp_IPlugin
{
	bool operator()
	(
		const IPlugin * a,
		const IPlugin * b
	)
	const
	{
		if( a->get_rank() == b ->get_rank() )
		{
			return a->get_language() < b->get_language() ;
		}
		else
		{
			return a->get_rank() < b->get_rank() ;
		}
	}
} ;

#endif // IPLUGIN_HPP
