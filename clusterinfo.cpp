#include <QTranslator>
#include <QDebug>

#include "clusterinfo.h"

#define TR(s)	QObject::tr(s)

static const unsigned char defaultDump[] =
{
	17,255,91,182,165,116,214,45,229,0,19,255,38,141,79,115,110,43,210,0,74,22,249,233,90,194,173,60,179,16,0,120,0,0,255,120,0,26,255,0,156,100,64,40,16,12,137,193,213,228,32,0,203,96,0,0,255,255,35,69,227,
	33,118,84,48,152,37,39,247,33,243,232,51,81,0,0,84,0,0,0,0,0,8,12,0,77,0,0,0,0,0,51,255,255,255,255,255,71,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,5,89,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,60,255,145,255,145,255,145,255,146,255,235,255,235,255,236,255,236,255,236,255,236,255,236,255,236,130,7,0,0,15,20,15,19,
	15,19,15,19,15,19,15,19,15,19,15,19,15,19,15,19,15,19,15,19,15,19,15,19,15,19,15,19,3,199
};

ClusterInfo::ClusterInfo()
{
	m_size = sizeof(defaultDump);
	memcpy( m_data, defaultDump, m_size );
	parseData();
}

static inline unsigned get_mask_shift( unsigned mask )
{
	int shift;
	for ( shift = 0; shift < 8; shift ++ )
	{
		if ( mask & (1 << shift) )
			break;
	}

	return shift;
}

unsigned ClusterInfo::getData( unsigned addr, unsigned mask ) const
{
	return (m_data[addr] & mask) >> get_mask_shift(mask);
}

void ClusterInfo::setData( unsigned addr, unsigned val, unsigned mask )
{
	if ( m_readonly )
		return;
	m_data[addr] &= ~mask;
	m_data[addr] |= (val << get_mask_shift(mask)) & mask;
}

static const unsigned char tankkennlinie_80[] = { 0x11, 0xFF, 0x5B, 0xB6, 0xA5, 0x74, 0xD6, 0x2D, 0xE5, 0x00 };
static const unsigned char tankkennlinie_81[] = { 0x0D, 0xFF, 0x63, 0xB6, 0xB9, 0x74, 0xD7, 0x2D, 0xE5, 0x00 };
static const unsigned char tankkennlinie_90[] = { 0x0D, 0xFF, 0x7C, 0xA1, 0xBD, 0x67, 0xD6, 0x28, 0xE5, 0x00 };
static const unsigned char tankkennlinie_91[] = { 0x0D, 0xFF, 0x6C, 0xB8, 0xC2, 0x77, 0xE0, 0x32, 0xF3, 0x00 };
static const unsigned char tankkennlinie_102[] = { 0x0D, 0xFF, 0x69, 0xB8, 0xBF, 0x77, 0xDE, 0x32, 0xF3, 0x00 };
static const unsigned char tankkennlinie_91a[] = { 0x11, 0xFF, 0x1D, 0xF3, 0xA8, 0x74, 0xD3, 0x34, 0xE1, 0x00 };
static const info_value_t vals_tankkennlinie[] = {
	{ (uintptr_t)tankkennlinie_80, TR("80 l. plastic") },
	{ (uintptr_t)tankkennlinie_81, TR("81 l. steel") },
	{ (uintptr_t)tankkennlinie_90, TR("90 l. steel") },
	{ (uintptr_t)tankkennlinie_91, TR("91 l. steel") },
	{ (uintptr_t)tankkennlinie_102, TR("102 l. steel") },
	{ (uintptr_t)tankkennlinie_91a, TR("91 l. additional tank (Alpina)") },
	{ 0, NULL }
};

static const unsigned char tankkennlinie_bc_80[] = { 0x9C, 0x64, 0x40, 0x28, 0x10, 0x0C, 0x89, 0xC1, 0xD5, 0xE4 };
static const unsigned char tankkennlinie_bc_81[] = { 0xA2, 0x64, 0x50, 0x28, 0x11, 0x0C, 0x9D, 0xC2, 0xDE, 0xE9 };
static const unsigned char tankkennlinie_bc_90[] = { 0xB4, 0x78, 0x50, 0x28, 0x0E, 0x0C, 0x89, 0xC7, 0xDE, 0xE9 };
static const unsigned char tankkennlinie_bc_91[] = { 0xB6, 0x8C, 0x64, 0x28, 0x10, 0x0C, 0x72, 0xC5, 0xE9, 0xF4 };
static const unsigned char tankkennlinie_bc_102[] = { 0xCC, 0x80, 0x64, 0x28, 0x0E, 0x0C, 0xA6, 0xCD, 0xEB, 0xF5 };
static const unsigned char tankkennlinie_bc_91a[] = { 0xDC, 0xA0, 0x50, 0x28, 0x14, 0x11, 0x1D, 0xA8, 0xD3, 0xE1 };
static const info_value_t vals_tankkennlinie_bc[] = {
	{ (uintptr_t)tankkennlinie_bc_80, TR("80 l. plastic") },
	{ (uintptr_t)tankkennlinie_bc_81, TR("81 l. steel") },
	{ (uintptr_t)tankkennlinie_bc_90, TR("90 l. steel") },
	{ (uintptr_t)tankkennlinie_bc_91, TR("91 l. steel") },
	{ (uintptr_t)tankkennlinie_bc_102, TR("102 l. steel") },
	{ (uintptr_t)tankkennlinie_bc_91a, TR("91 l. additional tank (Alpina)") },
	{ 0, NULL }
};

static const unsigned char tempkennlinie_1[] = { 0x13, 0xFF, 0x1E, 0x8D, 0x4F, 0x73, 0x6E, 0x2B, 0xD2, 0x00 };
static const unsigned char tempkennlinie_2[] = { 0x13, 0xFF, 0x26, 0x8D, 0x4F, 0x73, 0x6E, 0x2B, 0xD2, 0x00 };
static const unsigned char tempkennlinie_3[] = { 0x12, 0xFF, 0x1E, 0x81, 0x4E, 0x81, 0x6B, 0x2B, 0xD2, 0x00 };
static const info_value_t vals_tempkennlinie[] = {
	{ (uintptr_t)tempkennlinie_1, TR("1") },
	{ (uintptr_t)tempkennlinie_2, TR("2") },
	{ (uintptr_t)tempkennlinie_3, TR("3") },
	{ 0, NULL }
};

static const info_value_t vals_zylinderzahl[] = {
	{ 0x01, TR("4 cylinders") },
	{ 0x02, TR("6 cylinders") },
	{ 0x03, TR("8 cylinders") },
	{ 0x02, TR("12 cylinders") },
	{ 0, NULL }
};

static const info_value_t vals_motorart[] = {
	{ 0x00, TR("Gasoline") },
	{ 0, NULL }
};

static const info_value_t vals_kzahl_hi[] = {
	{ 0x02, "4534" },
	{ 0x01, "4500" },
	{ 0, NULL }
};

static const info_value_t vals_kzahl_lo[] = {
	{ 0x16, "4534" },
	{ 0xF4, "4500" },
	{ 0, NULL }
};

static const info_value_t vals_siagrenzdreh[] = {
	{ 0x2D, TR("4500 rpm") },
	{ 0, NULL }
};

static const info_value_t vals_wegstrecke[] = {
	{ 0x00, TR("km") },
	{ 0x01, TR("miles") },
	{ 0, NULL }
};

static const info_value_t vals_tachoskala[] = {
	{ 0x63, TR("240 km/h (ECE/Japan)") },
	{ 0x61, TR("260 km/h (ECE)") },
	{ 0x60, TR("260 km/h (Japan)") },
	{ 0x5E, TR("280 km/h (ECE/Japan)") },
	{ 0x5C, TR("300 km/h (ECE/Japan)") },
	{ 0x63, TR("150 mph (ECE/US)") },
	{ 0x61, TR("160 mph (ECE/US)") },
	{ 0x5F, TR("170 mph (ECE/US)") },
	{ 0x5D, TR("180 mph (ECE/US)") },
	{ 0x5F, TR("300 km/h (Alpina)") },
	{ 0x5A, TR("320 km/h (Alpina)") },
	{ 0, NULL }
};

static const info_value_t vals_dzmskala[] = {
	{ 0x56, TR("7000 rpm 3_imp/kwu") },
	{ 0x50, TR("8000 rpm 3_imp/kwu") },
	{ 0x48, TR("7000 rpm 4_imp/kwu") },
	{ 0, NULL }
};

static const info_value_t vals_kvmoel[] = {
	{ 0x00, TR("Consumption") },
	{ 0x01, TR("Oil temperature") },
	{ 0, NULL }
};

static const info_value_t vals_getriebe[] = {
	{ 0x00, TR("No indication") },
	{ 0x04, TR("Gear") },
	{ 0x05, TR("Gear and programm") },
	{ 0, NULL }
};

static const info_value_t vals_siazeit[] = {
	{ 0x53, TR("335 days") },
	{ 0xB3, TR("716 days") },
	{ 0, NULL }
};

static const info_value_t vals_siagrenzeinspekt[] = {
	{ 0xA1, TR("32.200 km") },
	{ 0x96, TR("30.000 km") },
	{ 0x78, TR("24.000 km") },
	{ 0, NULL }
};

static const info_value_t vals_warnunggesch[] = {
	{ 0x46, TR("120 km/h") },
	{ 0xFF, TR("Not active") },
	{ 0, NULL }
};

static const info_value_t vals_steigung[] = {
	{ 0x4F, "M20B25" },
	{ 0x64, "M30B30" },
	{ 0x64, "M30B35" },
	{ 0x9F, "M70B50" },
	{ 0x7C, "S38B36" },
	{ 0x6C, "M50B20" },
	{ 0x78, "M50B25" },
	{ 0xC0, "M60B30" },
	{ 0xC0, "M60B40" },
	{ 0xC3, "S38B39" },
	{ 0x96, "M50B25 Alpina" },
	{ 0x9E, "M30B35 Alpina biturbo" },
	{ 0, NULL }
};

static const info_value_t vals_warnungtemp[] = {
	{ 0x1A, TR("121°C") },
	{ 0x17, TR("125°C") },
	{ 0x14, TR("130°C") },
	{ 0, NULL }
};

static const info_value_t vals_checkcontrol[] = {
	{ 0x00, TR("ECE") },
	{ 0x01, TR("US") },
	{ 0x02, TR("Belt warning") },
	{ 0x03, TR("Japan") },
	{ 0x04, TR("Japan + belt warning") },
	{ 0, NULL }
};

static const info_value_t vals_sprache[] = {
	{ 0x00, TR("Deutsch") },
	{ 0x01, TR("English US") },
	{ 0x02, TR("English UK") },
	{ 0x03, TR("French") },
	{ 0x04, TR("Italian") },
	{ 0x05, TR("Spanish") },
	{ 0, NULL }
};

static const info_value_t vals_laender[] = {
	{ 0x00, TR("ECE (Deutsch)") },
	{ 0x01, TR("US") },
	{ 0x02, TR("Switzerland (French)") },
	{ 0x03, TR("Japan") },
	{ 0x04, TR("km_tacho_und_englisch") },
	{ 0x05, TR("ECE (France)") },
	{ 0x06, TR("ECE (Spain)") },
	{ 0x07, TR("ECE (Italia)") },
	{ 0x08, TR("Switzerland (German)") },
	{ 0x09, TR("ECE (RHD)") },	// righ- hand drive?
	{ 0, NULL }
};

static const info_value_t vals_baureihe[] = {
	{ 0x00, "E32" },
	{ 0x01, "E34" },
	{ 0, NULL }
};

static const info_value_t vals_zeiteinheit[] = {
	{ 0x00, TR("24 hours") },
	{ 0x01, TR("12 hours") },
	{ 0, NULL }
};

static const info_value_t vals_tempeinheit[] = {
	{ 0x00, TR("Celsius") },
	{ 0x01, TR("Fahrenheit") },
	{ 0, NULL }
};

static const info_value_t vals_verbraucheinheit[] = {
	{ 0x00, TR("l/100") },
	{ 0x01, TR("km/l") },
	{ 0x02, TR("mpg US") },
	{ 0x03, TR("mpg UK") },
	{ 0, NULL }
};

const static info_property_t records[] = {
	{ "TANK_KENNLINIE", TR("Tank curve"),
	  0x00, 0xA, 0xFF, vals_tankkennlinie, false },
	{ "TEMPERATUR_KENNLINIE", TR("Temperature curve"),
	  0x0A, 0xA, 0xFF, vals_tempkennlinie, false },
	{ "ZYLINDER_ZAHL", TR("Cylinder number"),
	  0x14, 0x1, 0x1C, vals_zylinderzahl, false },
	{ "K_ZAHL", TR("K-Zahl (hi)"),
	  0x14, 0x1, 0x03, vals_kzahl_hi, false },
	{ "K_ZAHL", TR("K-Zahl (lo)"),
	  0x15, 0x1, 0xFF, vals_kzahl_lo, false },
	{ "MOTOR_ART", TR("Motor type"),
	  0x16, 0x1, 0x04, vals_motorart, false },
	{ "K_ZAHL_KOMPL", TR("K-Zahl (hi, complementary)"),
	  0x16, 0x1, 0x03, vals_kzahl_hi, true },
	{ "K_ZAHL_KOMPL", TR("K-Zahl (lo, complementary)"),
	  0x17, 0x1, 0xFF, vals_kzahl_lo, true },
	{ "SIA_GRENZDREHZAHL", TR("SIA speed limit"),
	  0x18, 0x1, 0xFE, vals_siagrenzdreh, false },
	{ "WEGSTRECKE_EINHEIT", TR("Length unit"),
	  0x19, 0x1, 0x01, vals_wegstrecke, false },
	{ "tacho_skala_endwert", TR("Speedometer scale limit"),
	  0x19, 0x1, 0xFE, vals_tachoskala, false },
	{ "DZM_SKALA_ENDWERT", TR("RPM scale limit"),
	  0x1A, 0x1, 0xFE, vals_dzmskala, false },
	{ "ANZEIGE_KVA/OEL", TR("Consumption/Oil display"),
	  0x1B, 0x1, 0x01, vals_kvmoel, false },
	{ "tacho_skala_endwert_kompl", TR("Speedometer scale limit (complementary)"),
	  0x1B, 0x1, 0xFE, vals_tachoskala, true },
	{ "SIA_GRENZE_ZEIT", TR("SIA days limit (hi)"),
	  0x1C, 0x1, 0xFF, vals_siazeit, false },
	{ "SIA_GRENZE_ZEIT", TR("SIA days limit (lo)"),
	  0x1D, 0x1, 0xC0, NULL, false },
	{ "GETRIEBE_ART", TR("Transmission display"),
	  0x1D, 0x1, 0x07, vals_getriebe, false },
	{ "OELSERVICE_ANZAHL", TR("Oil service number"),
	  0x1D, 0x1, 0x38, NULL, false },
	{ "SIA_GRENZE_INSPEKTION", TR("SIA inspection limit"),
	  0x1F, 0x1, 0xFF, vals_siagrenzeinspekt, false },	// *200 km
	{ "WARNUNG_GESCHW_LIMIT", TR("Warning speed limit"),
	  0x22, 0x1, 0xFF, vals_warnunggesch, false },
	{ "STEIGUNG_EINSPRITZ_KENNL", TR("Injection pitch"),
	  0x23, 0x1, 0xFF, vals_steigung, false },
	{ "WARNUNG_GESCHW_LIM_KOMPL", TR("Warning speed limit (complementary)"),
	  0x24, 0x1, 0xFF, vals_warnunggesch, true },
	{ "WARNUNG_UEBERTEMPERATUR", TR("Overtemperature warning"),
	  0x25, 0x1, 0xFF, vals_warnungtemp, false },
	{ "check_control_version", TR("Check control version"),
	  0x27, 0x1, 0x0F, vals_checkcontrol, false },
	{ "sprache", TR("Language"),
	  0x27, 0x1, 0xF0, vals_sprache, false },
	{ "tank_kennlinie_bc", TR("Tank curve BC"),
	  0x28, 0xA, 0xFF, vals_tankkennlinie_bc, false },
	{ "laender_einheiten", TR("Country"),
	  0x32, 0x1, 0x1F, vals_laender, false },
	{ "baureihe", TR("Series"),
	  0x32, 0x1, 0x20, vals_baureihe, false },
	{ "zeit_einheit", TR("Time format"),
	  0x33, 0x1, 0x10, vals_zeiteinheit, false },
	{ "temperatur_einheit", TR("Temperature units"),
	  0x33, 0x1, 0x20, vals_tempeinheit, false },
	{ "verbrauch_einheit", TR("Consumption units"),
	  0x33, 0x1, 0xC0, vals_verbraucheinheit, false },
	{ "k_zahl_bc", TR("K-Zahl BC"),
	  0x34, 0x1, 0xFF, NULL, false },
	{ "ZYLINDER_ZAHL", TR("Cylinder number"),
	  0x35, 0x1, 0xF0, NULL, false },
	{ "CODE_NR_5", TR("Code num[5]"),
	  0x35, 0x1, 0x0F, NULL, false },
	{ "CODE_NR_1", TR("Code num[1]"),
	  0x36, 0x1, 0xF0, NULL, false },
	{ "CODE_NR_2", TR("Code num[2]"),
	  0x36, 0x1, 0x0F, NULL, false },
	{ "CODE_NR_3", TR("Code num[3]"),
	  0x37, 0x1, 0xF0, NULL, false },
	{ "CODE_NR_4", TR("Code num[4]"),
	  0x37, 0x1, 0x0F, NULL, false },
	{ "FAHRGESTELL_NR[14]", TR("Chassis num[14]"),
	  0x3A, 0x1, 0xF0, NULL, false },
	{ "FAHRGESTELL_NR[15]", TR("Chassis num[15]"),
	  0x3A, 0x1, 0x0F, NULL, false },
	{ "FAHRGESTELL_NR[16]", TR("Chassis num[16]"),
	  0x3B, 0x1, 0xF0, NULL, false },
	{ "FAHRGESTELL_NR[17]", TR("Chassis num[17]"),
	  0x3B, 0x1, 0x0F, NULL, false },
	{ "FAHRGESTELL_NR[11]", TR("Chassis num[11]"),
	  0x3C, 0x1, 0x3E, NULL, false },
	{ "FAHRGESTELL_NR[12]", TR("Chassis num[12]"),
	  0x3C, 0x1, 0x01, NULL, false },
	{ "FAHRGESTELL_NR[12]", TR("Chassis num[12]"),
	  0x3D, 0x1, 0xF0, NULL, false },
	{ "FAHRGESTELL_NR[13]", TR("Chassis num[13]"),
	  0x3D, 0x1, 0x0F, NULL, false },
	{ "TEILENUMMER[4]", TR("Part num[4]"),
	  0x3E, 0x1, 0xF0, NULL, false },
	{ "TEILENUMMER[5]", TR("Part num[5]"),
	  0x3E, 0x1, 0x0F, NULL, false },
	{ "TEILENUMMER[6]", TR("Part num[6]"),
	  0x3F, 0x1, 0xF0, NULL, false },
	{ "TEILENUMMER[7]", TR("Part num[7]"),
	  0x3F, 0x1, 0x0F, NULL, false },
	{ "DATA_VALID", TR("Data valid"),
	  0x40, 0x1, 0xC0, NULL, false },
	{ "VDO_TEST", TR("VDO test"),
	  0x40, 0x1, 0x30, NULL, false },
	{ "TEILENUMMER[1]", TR("Part num[1]"),
	  0x40, 0x1, 0x0F, NULL, false },
	{ "TEILENUMMER[2]", TR("Part num[2]"),
	  0x41, 0x1, 0xF0, NULL, false },
	{ "TEILENUMMER[3]", TR("Part num[3]"),
	  0x41, 0x1, 0x0F, NULL, false },
};

static inline char vinchar( unsigned n )
{
	if ( n & 0x10 )
		return (n & 0xF) + 'A' - 1;
	return n + '0';
}

static inline char vinnum( unsigned n )
{
	return n + '0';
}

void ClusterInfo::parseData()
{
	updateProperties();
	updateSimple();
}

void ClusterInfo::updateProperties()
{
	dataChanged( index(1, 0), index(1, rowCount()) );
}

int ClusterInfo::columnCount(const QModelIndex &/*parent*/) const
{
	return 3;
}

int ClusterInfo::rowCount(const QModelIndex &/*parent*/) const
{
	return sizeof(records) / sizeof(records[0]);
}

QVariant ClusterInfo::headerData(int section, Qt::Orientation orientation, int role) const
{
	if ( role != Qt::DisplayRole )
		return QVariant();

	if ( orientation == Qt::Horizontal )
		switch ( section )
		{
			case 0:
				return QString( tr("Property") );
			case 1:
				return QString( tr("Value") );
			case 2:
				return QString( tr("Description") );
		}

	return QVariant();
}

QVariant ClusterInfo::data(const QModelIndex &index, int role) const
{
	if ( !index.isValid() )
		return QVariant();

	if ( index.column() >= 3 )
		return QVariant();

	if ( index.row() >= rowCount() )
		return QVariant();

	if (role == Qt::DisplayRole /*|| role == Qt::EditRole*/)
	{
		const info_property_t *r = records + index.row();
		switch ( index.column() )
		{
			case 0:
				return QString(r->prop);
			case 2:
				return QString(r->desc);
			case 1:
			default:
			{
				QString value;
				unsigned val = getData( r->addr, r->mask );
				unsigned shift = get_mask_shift( r->mask );
				unsigned i;

				if ( r->size > 1 )
				{
					for ( i = 0; i < r->size; i ++ )
						value += QString("%1 ").arg( getData(r->addr + i), 2, 16, QChar('0') ).toUpper();
				}
				else
					value = QString::number(val, 16).toUpper() + "h ";

				if ( !r->values )
					return value;

				const info_value_t *v;
				for ( v = r->values; !v->name.isEmpty(); v ++ )
				{
					if ( r->complementary )
					{
						if ( v->value == ((~val) & (r->mask >> shift)) )
							break;
					}
					else if ( r->size > 1 )
					{
						if ( memcmp( (void *)v->value, m_data + r->addr, r->size) == 0 )
							break;
					}
					else if ( v->value == val )
						break;
				}
				if ( v->name.isEmpty() )
					return value;

				return value + QString("'%1'").arg(v->name);
			}
		}
	}

	return QVariant();
}

#define ODOMETER_OFFSET 34

void ClusterInfo::updateSimple()
{
	const info_value_t *v;
	unsigned i;

	m_vin = "";
	m_vin += vinchar( getData(0x3C, 0x3E) );
	m_vin += vinchar( (getData(0x3C, 1) << 4) | getData( 0x3D, 0xF0) );
	m_vin += vinnum( getData(0x3D, 0x0F) );
	m_vin += vinnum( getData(0x3A, 0xF0) );
	m_vin += vinnum( getData(0x3A, 0x0F) );
	m_vin += vinnum( getData(0x3B, 0xF0) );
	m_vin += vinnum( getData(0x3B, 0x0F) );
	//qDebug() << "VIN:" << m_vin;

	m_odometer = 0;
	for ( i = m_size - 34; i < m_size - 2; i += 2 )
	{
		m_odometer += m_data[i + 0] << 8;
		m_odometer += m_data[i + 1];
	}
	m_odometer *= 2;
	m_odometer -= ODOMETER_OFFSET;
	//qDebug() << "Milage:" << m_odometer << "km";

	m_partnum = getData(0x40, 0xF) + '0';
	m_partnum += QString("%1").arg( getData(0x41), 2, 16, QChar('0') );
	m_partnum += QString("%1").arg( getData(0x3E), 2, 16, QChar('0') );
	m_partnum += QString("%1").arg( getData(0x3F), 2, 16, QChar('0') );
	m_partnum = m_partnum.toUpper();

	m_code = QString("%1").arg( getData(0x36), 2, 16, QChar('0') );
	m_code += QString("%1").arg( getData(0x37), 2, 16, QChar('0') );
	m_code += getData(0x35, 0xF) + '0';
	m_code = m_code.toUpper();

	m_tankcurve = -1;
	for ( i = 0, v = vals_tankkennlinie; !v->name.isEmpty(); i ++, v ++ )
	{
		if ( memcmp( (void *)v->value, m_data + 0x00, 0xA ) == 0 )
			break;
	}
	if ( !v->name.isEmpty() )
		m_tankcurve = i;

	m_tempcurve = -1;
	for ( i = 0, v = vals_tempkennlinie; !v->name.isEmpty(); i ++, v ++ )
	{
		if ( memcmp( (void *)v->value, m_data + 0x0A, 0xA ) == 0 )
			break;
	}
	if ( !v->name.isEmpty() )
		m_tempcurve = i;
}

int ClusterInfo::setVIN( const QString &vin )
{
	if ( vin.length() != 7 )
		return -1;

	if ( !vin[0].isLetter() )
		return -2;
	if ( !vin[1].isLetter() )
		return -2;
	if ( !vin[2].isDigit() )
		return -2;
	if ( !vin[3].isDigit() )
		return -2;
	if ( !vin[4].isDigit() )
		return -2;
	if ( !vin[5].isDigit() )
		return -2;
	if ( !vin[6].isDigit() )
		return -2;

	m_vin = vin.toUpper();

	setData(0x3C, 3, 0xC0);
	setData(0x3C, (m_vin[0].toLatin1() - 'A' + 1) | 0x10, 0x3E);
	setData(0x3C, 1, 0x1);
	setData(0x3D, m_vin[1].toLatin1() - 'A' + 1, 0xF0);
	setData(0x3D, m_vin[2].digitValue(), 0x0F);
	setData(0x3A, m_vin[3].digitValue(), 0xF0);
	setData(0x3A, m_vin[4].digitValue(), 0x0F);
	setData(0x3B, m_vin[5].digitValue(), 0xF0);
	setData(0x3B, m_vin[6].digitValue(), 0x0F);

	updateProperties();
	emit updateDump();

	return 0;
}

void ClusterInfo::setOdometer( unsigned odometer )
{
	unsigned km_hi, km_lo;
	unsigned i;

	odometer /= 2;
	odometer += ODOMETER_OFFSET / 2;
	km_hi = odometer / 16;
	km_lo = odometer % 16;
	odometer -= ODOMETER_OFFSET / 2;

	for ( i = 0; i < 16; i ++ )
	{
		 if ( i >= km_lo )
		{
			setData( m_size - 34 + i * 2, km_hi >> 8 );
			setData( m_size - 33 + i * 2, km_hi );
		} else {
			setData( m_size - 34 + i * 2, (km_hi + 1) >> 8 );
			setData( m_size - 33 + i * 2, km_hi + 1 );
		}
	}

	m_odometer = odometer * 2;

	emit updateDump();
}

void ClusterInfo::setTank( int index )
{
	if ( index < 0 )
		return;
	if ( index >= 6 )
		return;

	memcpy( m_data + 0x00, (void *)vals_tankkennlinie[index].value, 0xA );
	memcpy( m_data + 0x28, (void *)vals_tankkennlinie_bc[index].value, 0xA );

	updateProperties();
	emit updateDump();
}

void ClusterInfo::setTempSensor( int index )
{
	if ( index < 0 )
		return;
	if ( index >= 3 )
		return;

	memcpy( m_data + 0x0A, (void *)vals_tempkennlinie[index].value, 0xA );

	updateProperties();
	emit updateDump();
}

void ClusterInfo::setPartNum( unsigned num )
{
	setData(0x40, (num / 1000000) % 10, 0x0F);
	setData(0x41, (num / 100000) % 10, 0xF0);
	setData(0x41, (num / 10000) % 10, 0x0F);
	setData(0x3E, (num / 1000) % 10, 0xF0);
	setData(0x3E, (num / 100) % 10, 0x0F);
	setData(0x3F, (num / 10) % 10, 0xF0);
	setData(0x3F, num % 10, 0x0F);

	updateProperties();
	emit updateDump();
}

void ClusterInfo::setCodeNum( unsigned num )
{
	setData(0x36, (num / 10000) % 10, 0xF0);
	setData(0x36, (num / 1000) % 10, 0x0F);
	setData(0x37, (num / 100) % 10, 0xF0);
	setData(0x37, (num / 10) % 10, 0x0F);
	setData(0x35, num % 10, 0x0F);

	updateProperties();
	emit updateDump();
}
