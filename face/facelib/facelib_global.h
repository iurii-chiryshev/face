#ifndef FACELIB_GLOBAL_H
#define FACELIB_GLOBAL_H

#include <QtCore/qglobal.h>
#include <memory>


#ifdef FACELIB_LIBRARY
#  define FACELIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FACELIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FACELIB_GLOBAL_H
