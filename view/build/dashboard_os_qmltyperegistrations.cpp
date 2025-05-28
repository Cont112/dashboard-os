/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#if __has_include(<processmodel.h>)
#  include <processmodel.h>
#endif


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_DashboardOS()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    QMetaType::fromType<QAbstractTableModel *>().id();
    qmlRegisterTypesAndRevisions<TableModel>("DashboardOS", 1);
    qmlRegisterAnonymousType<QAbstractItemModel, 254>("DashboardOS", 1);
    QT_WARNING_POP
    qmlRegisterModule("DashboardOS", 1, 0);
}

static const QQmlModuleRegistration dashboardOSRegistration("DashboardOS", qml_register_types_DashboardOS);
