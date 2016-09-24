#ifndef IUI_HPP
#define IUI_HPP

#include "qt.hpp"

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
		IUI( QObject * parent = 0 ) ;

		// --- DESTRUCTORS ---

		///
		/// \brief Destructor
		///
		virtual ~IUI() = 0 ;

	public slots :

		// --- SLOTS ---

		void run() ;

	signals :

		// --- SIGNALS ---

		void finished() ;

	protected :

		// --- MEMBERS ---

		virtual void process() = 0 ;

} ;

#endif // IUI_HPP
