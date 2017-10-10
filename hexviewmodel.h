#ifndef HEXVIEWMODEL_H
#define HEXVIEWMODEL_H

#include <QAbstractTableModel>
#include "clusterinfo.h"

class HexViewModel : public QAbstractTableModel
{
public:
	HexViewModel(ClusterInfo *provider);

	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	void invalidate();

private:
	ClusterInfo *m_provider;
};

#endif // HEXVIEWMODEL_H
