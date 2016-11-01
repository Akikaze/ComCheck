#ifndef QT_HPP
#define QT_HPP

/*
Include every Qt object that is required by ComCheck.
It could be not optimised but it's way simpler to use.

But this method increase the time for maintainance because we need to check
at every moment if the modification of a code requires a new include or if
we could withdraw an include without creating errors.
*/

#include <QCoreApplication>
#include <QDir>
#include <QList>
#include <QPair>
#include <QPluginLoader>
#include <QString>
#include <QStringList>
#include <QTimer>

#endif // QT_HPP
