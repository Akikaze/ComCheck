#ifndef IUI_HPP
#define IUI_HPP

#include "structures.hpp"

class Core ;

class IUI
: public QObject
{
	Q_OBJECT

	public :

		// --- CONSTRUCTORS ---

		///
		/// \brief Constructor
		/// \param parent Parent for this QObject
		///
		IUI( QObject * core ) ;

		// --- DESTRUCTORS ---

		///
		/// \brief Destructor
		///
		~IUI() ;

	public slots :

		// --- SLOTS ---

		void run() ;

	signals :

		// --- SIGNALS ---

		void finished() ;

	protected :

		// --- MEMBERS ---

		virtual void process() = 0 ;

		// --- ATTRIBUTES ---

		Core * core_ ; ///< Pointer to the core

} ;

#endif // IUI_HPP
