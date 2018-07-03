#ifndef CLUSTERINFO_H
#define CLUSTERINFO_H

#include <QAbstractTableModel>
#include <QObject>
#include <QByteArray>
#include <QString>
//#include <stdint.h> //un comment this line if you building in Ubuntu with qmake

typedef struct rec_value
{
	uintptr_t value;
	const QString name;
} info_value_t;

typedef struct property {
	const char *prop;
	const QString desc;
	unsigned addr;
	unsigned size;
	unsigned mask;
	const struct rec_value *values;
	bool complementary;
} info_property_t;

class ClusterInfo : public QAbstractTableModel
{
	Q_OBJECT

public:
	ClusterInfo();

	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	void parseData();
	void updateProperties();
	void updateSimple();

	unsigned char *getDump( unsigned offset = 0 ) { return m_data + offset; }
	unsigned getData( unsigned addr, unsigned mask = 0xFF ) const;
	void setData( unsigned addr, unsigned val, unsigned mask = 0xFF );

	void setReadOnly( bool val = true ) { m_readonly = val; }

	int setVIN( const QString &vin );
	void setOdometer( unsigned milage );
	void setTank( int index );
	void setTempSensor( int index );
	void setPartNum( unsigned num );
	void setCodeNum( unsigned num );

	unsigned m_size;
	bool m_readonly;

	QString m_vin;
	QString m_partnum;
	QString m_code;
	long m_odometer;
	unsigned m_tankcurve;
	unsigned m_tempcurve;

private:
	unsigned char m_data[256];

signals:
	void updateDump();
};

#endif // CLUSTERINFO_H
