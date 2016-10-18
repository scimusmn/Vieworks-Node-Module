#ifndef VWGIGE_GLOBAL_H
#define VWGIGE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VWGIGE_LIBRARY)
#  define VWGIGESHARED_EXPORT Q_DECL_EXPORT
#else
#  define VWGIGESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // VWGIGE_GLOBAL_H
