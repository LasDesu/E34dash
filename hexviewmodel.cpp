#include "hexviewmodel.h"

HexViewModel::HexViewModel(ClusterInfo *provider)
	: m_provider(provider)
{

}

int HexViewModel::columnCount(const QModelIndex &/*parent*/) const
{
	return 16;
}

int HexViewModel::rowCount(const QModelIndex &/*parent*/) const
{
	return (m_provider->m_size + 15) / 16;
}

QVariant HexViewModel::data(const QModelIndex &index, int role) const
{
	unsigned addr;

	if ( !index.isValid() )
		return QVariant();

	if ( index.column() >= 16 )
		return QVariant();

	addr = index.row() * 16 + index.column();
	if ( addr >= m_provider->m_size )
		return QVariant();

	if (role == Qt::DisplayRole || role == Qt::EditRole)
		return QString("%1").arg( m_provider->getData(addr), 2, 16, QChar('0') ).toUpper();
	else
		return QVariant();
}

QVariant HexViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if ( role != Qt::DisplayRole )
		return QVariant();

	if ( orientation == Qt::Horizontal )
		return "x" + QString::number( section, 16 ).toUpper();
	else if ( orientation == Qt::Vertical )
		return QString("%1").arg( section << 4, 2, 16, QChar('0') ).toUpper();

	return QVariant();
}

void HexViewModel::invalidate()
{
	beginResetModel();
	endResetModel();
}
