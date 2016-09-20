#ifndef IPLUGIN_HPP
#define IPLUGIN_HPP

#include <QtPlugin>

#include "structures.hpp"

class IPlugin
{
	public :

		///
		/// \brief Destructor
		///
		virtual ~IPlugin() {}

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
		/// \return CC_Flag which describe the line
		///
		virtual CC_Flag get_type( const std::string & line ) = 0 ;

	protected :

		QStringList extensions_ ; ///< Extension analysed by the plugin
		QString	language_ ; ///< Language of the plugin
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
