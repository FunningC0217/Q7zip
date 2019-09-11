#ifndef Q7ZIP_GLOBAL_H
#define Q7ZIP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(Q7ZIP_LIBRARY)
#  define Q7ZIPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define Q7ZIPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // Q7ZIP_GLOBAL_H
