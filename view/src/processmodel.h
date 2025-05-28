#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <qqml.h>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT // Registers the type for QML

public:
    explicit TableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return 200;
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent);
        return 200;
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (!index.isValid())
            return QVariant();
        switch (role) {
            case Qt::DisplayRole:
                return QString("%1, %2").arg(index.column()).arg(index.row());
            default:
                return QVariant();
        }
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return {{Qt::DisplayRole, "display"}};
    }
};

#endif // TABLEMODEL_H