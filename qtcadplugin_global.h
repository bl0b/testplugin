#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTCADPLUGIN_LIB)
#  define QTCADPLUGIN_EXPORT Q_DECL_EXPORT
# else
#  define QTCADPLUGIN_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTCADPLUGIN_EXPORT
#endif
