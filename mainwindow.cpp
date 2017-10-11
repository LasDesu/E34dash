#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_dumpmodel = new HexViewModel( &m_info );
	ui->tableDump->setModel( m_dumpmodel );
	ui->tableProperties->setModel( &m_info );

	connect( &m_info, SIGNAL(updateDump()), this, SLOT(updateDump()) );

	m_info.parseData();
	update_view();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::update_view()
{
	unsigned miles, engine, maxspeed, maxrpm, geardisplay, country, kzahl, overtemp, insptime;
	unsigned tmp;

	updateDump();

	ui->editVIN->setText( m_info.m_vin );
	ui->spinOdometer->setValue( m_info.m_odometer );
	ui->editPartNum->setText( m_info.m_partnum );
	ui->editCode->setText( m_info.m_code );

	miles = m_info.getData(0x19, 0x01);

	tmp = m_info.getData(0x23);
	switch ( tmp )
	{
		case 0x4F: // M20B25
			engine = 0; break;
		case 0x64:	// M30B30, M30B35
			engine = 1; break;
		case 0x9F: // M70B50
			engine = 7; break;
		case 0x7C: // S38B36
			engine = 8; break;
		case 0x6C: // M50B20
			engine = 3; break;
		case 0x78: // M50B25
			engine = 4; break;
		case 0xC0: // M60B30, M60B40
			engine = 5; break;
		case 0xC3: // S38B39
			engine = 9; break;
		case 0x96: // M50B25 Alpina
			engine = 10; break;
		case 0x9E: // M30B35 Alpina biturbo
			engine = 11; break;
		default:
			engine = -1;
	}

	tmp = m_info.getData(0x19, 0xFE);
	switch ( tmp )
	{
		case 0x63:	// 240 km/h (ECE/Japan), 150 mph (ECE/US)
			if ( miles )
				maxspeed = 4;
			else
				maxspeed = 0;
			break;
		case 0x61:	// 260 km/h (ECE), 160 mph (ECE/US)
			if ( miles )
				maxspeed = 5;
			else
				maxspeed = 1;
			break;
		case 0x60:	// 260 km/h (Japan)
			maxspeed = 2; break;
		case 0x5E:	// 280 km/h (ECE/Japan)
			maxspeed = 3; break;
		case 0x5C:	// 300 km/h (ECE/Japan)
			maxspeed = 4; break;
		case 0x5F:	// 170 mph (ECE/US), 300 km/h (Alpina)
			if ( miles )
				maxspeed = 7;
			else
				maxspeed = 9;
				break;
		case 0x5D:	// 180 mph (ECE/US)
			maxspeed = 8; break;
		case 0x5A:	// 320 km/h (Alpina)
			maxspeed = 10; break;
		default:
			maxspeed = -1;
	}

	tmp = m_info.getData(0x1A, 0xFE);
	switch ( tmp )
	{
		case 0x56:	// 7000 rpm 3 imp/kwu
			maxrpm = 0; break;
		case 0x50:	// 8000 rpm 3 imp/kwu
			maxrpm = 1; break;
		case 0x48:	// 7000 rpm 4 imp/kwu
			maxrpm = 2; break;
		default:
			maxrpm = -1;
	}

	tmp = m_info.getData(0x1D, 0x07);
	switch ( tmp )
	{
		case 0x00:	// No indication
			geardisplay = 0; break;
		case 0x04:	// Gear
			geardisplay = 1; break;
		case 0x05:	// Gear and programm
			geardisplay = 2; break;
		default:
			geardisplay = -1;
	}

	tmp = m_info.getData(0x32, 0x1F);
	switch ( tmp )
	{
		case 0x00: // ECE (Deutsch)
			country = 0; break;
		case 0x01: // US
			country = 1; break;
		case 0x02: // Switzerland (French)
			country = 8; break;
		case 0x03: // Japan
			country = 2; break;
		case 0x04: // km_tacho_und_englisch
			country = 9; break;
		case 0x05: // ECE (France)
			country = 3; break;
		case 0x06: // ECE (Spain)
			country = 4; break;
		case 0x07: // ECE (Italia)
			country = 5; break;
		case 0x08: // Switzerland (German)
			country = 7; break;
		case 0x09: // ECE (RHD)
			country = 6; break;
		default:
			country = -1;
	}

	kzahl = 4000;
	kzahl += m_info.getData(0x15, 0xFF);
	kzahl += m_info.getData(0x14, 0x03) << 8;

	tmp = m_info.getData(0x25, 0xFF);
	switch ( tmp )
	{
		case 0x1A: // 121
			overtemp = 0; break;
		case 0x17: // 125
			overtemp = 1; break;
		case 0x14: // 130
			overtemp = 2; break;
		default:
			kzahl = -1;
	}

	insptime = m_info.getData(0x1C, 0xFF) << 2;
	insptime |= m_info.getData(0x1D, 0xC0);

	ui->comboEngine->setCurrentIndex( engine );
	ui->comboMaxSpeed->setCurrentIndex( maxspeed );
	ui->comboMaxRPM->setCurrentIndex( maxrpm );
	ui->comboTank->setCurrentIndex( m_info.m_tankcurve );
	ui->comboTempCurve->setCurrentIndex( m_info.m_tempcurve );
	ui->comboKVAoil->setCurrentIndex( m_info.getData(0x1B, 0x01) );
	ui->comboSeries->setCurrentIndex( m_info.getData(0x32, 0x20) );
	ui->spinKzahl->setValue( kzahl );
	ui->spinInspectionPeriod->setValue( m_info.getData(0x1F, 0xFF) * 200 );
	ui->spinInspectionTime->setValue( insptime );
	ui->spinOilService->setValue( m_info.getData(0x1D, 0x38) );
	ui->comboOvertemperature->setCurrentIndex( overtemp );

	tmp = m_info.getData(0x22, 0xFF);
	ui->checkSpeedWarning->setChecked( tmp != 0xFF );
	ui->spinSpeedLimit->setVisible( tmp != 0xFF );
	ui->spinSpeedLimit->setValue( (tmp != 0xFF) ? /*tmp * 2*/ tmp + 50 : 120 );

	ui->comboGearDisplay->setCurrentIndex( geardisplay );
	ui->comboCheckControls->setCurrentIndex( m_info.getData(0x27, 0x0F) );
	ui->comboCountry->setCurrentIndex( country );
	ui->comboLanguage->setCurrentIndex( m_info.getData(0x27, 0xF0) );
	ui->comboUnitLength->setCurrentIndex( miles );
	ui->comboUnitConsuption->setCurrentIndex( m_info.getData(0x33, 0xC0) );
	ui->comboUnitTemperature->setCurrentIndex( m_info.getData(0x33, 0x20) );
	ui->comboTimeFormat->setCurrentIndex( m_info.getData(0x33, 0x10) );
}

void MainWindow::updateDump()
{
	m_dumpmodel->invalidate();
}

void MainWindow::on_chkSwapBytes_toggled(bool checked)
{
	unsigned char tmp;
	unsigned i;

	for ( i = 0; i < m_info.m_size; i += 2 )
	{
		tmp = m_info.getData(i);
		m_info.setData(i, m_info.getData(i + 1));
		m_info.setData(i + 1, tmp);
	}

	ui->butSaveSwap->setText( checked ? tr("Save unswapped") : tr("Save swapped") );

	m_info.parseData();
	update_view();
}

void MainWindow::on_butOpen_clicked()
{
	QString	filename = QFileDialog::getOpenFileName( this, tr("Open file...") );
	if ( filename.isEmpty() )
		return;

	QFile file( filename );
	if ( !file.open( QIODevice::ReadOnly ) )
		return;
	m_info.m_size = file.read( (char *)m_info.getDump(), 256 );

	if ( ui->chkSwapBytes->isChecked() )
	{
		unsigned char tmp;
		unsigned i;
		for ( i = 0; i < m_info.m_size; i += 2 )
		{
			tmp = m_info.getData(i);
			m_info.setData( i, m_info.getData(i + 1) );
			m_info.setData( i + 1, tmp );
		}
	}

	m_info.parseData();
	update_view();
}

void MainWindow::saveFile( bool swap )
{
	QString	filename = QFileDialog::getSaveFileName( this, tr("Save file...") );
	if ( filename.isEmpty() )
		return;

	QFile file( filename );
	if ( !file.open( QIODevice::WriteOnly ) )
		return;

	if ( swap )
	{
		unsigned i;
		for ( i = 0; i < m_info.m_size; i += 2 )
		{
			file.putChar( (char)m_info.getData(i + 1) );
			file.putChar( (char)m_info.getData(i) );
		}
	} else {
		file.write( (const char *)m_info.getDump(), m_info.m_size );
	}
}

void MainWindow::on_butSave_clicked()
{
	saveFile( ui->chkSwapBytes->isChecked() );
}

void MainWindow::on_butSaveSwap_clicked()
{
	saveFile( !ui->chkSwapBytes->isChecked() );
}

void MainWindow::on_editVIN_editingFinished()
{
	m_info.setVIN( ui->editVIN->text() );
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_spinOdometer_editingFinished()
{
	m_info.setOdometer( ui->spinOdometer->value() );
	update_view();
}

void MainWindow::on_comboEngine_currentIndexChanged(int index)
{
	unsigned fuel, injection, cylinders;

	fuel = 0;	// gasoline
	cylinders = 6;
	injection = 0x78;	// M50B25 by default
	switch ( index )
	{
		case 0:	// M20B25
			injection = 0x4F;
			break;
		case 1:	// M30B30
			injection = 0x64;
			break;
		case 2: // M30B35
			injection = 0x64;
			break;
		case 3: // M50B20
			injection = 0x6C;
			break;
		case 4: // M50B25
			injection = 0x78;
			break;
		case 5: // M60B30
			injection = 0xC0;
			cylinders = 8;
			break;
		case 6: // M60B40
			injection = 0xC0;
			cylinders = 8;
			break;
		case 7: // M70B50
			injection = 0x9F;
			cylinders = 12;
			break;
		case 8: // S38B36
			injection = 0x7C;
			break;
		case 9: // S38B39
			injection = 0xC3;
			break;
		case 10: // M30B35 Alpina biturbo
			injection = 0x9E;
			break;
		case 11: // M50B25 Alpina
			injection = 0x96;
			break;
	}

	switch ( cylinders )
	{
		case 4:
			m_info.setData(0x14, 1, 0x1C); break;
		case 6:
			m_info.setData(0x14, 2, 0x1C); break;
		case 8:
			m_info.setData(0x14, 3, 0x1C); break;
		case 12:
			m_info.setData(0x14, 2, 0x1C); break;
	}
	m_info.setData(0x35, cylinders, 0xF0);
	m_info.setData(0x16, fuel, 0x04);
	m_info.setData(0x23, injection, 0xFF);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboMaxSpeed_currentIndexChanged(int index)
{
	unsigned speed;

	speed = 0x61;
	switch ( index )
	{
		case 0:	// 240 km/h (ECE/Japan)
			speed = 0x63; break;
		case 1:	// 260 km/h (ECE)
			speed = 0x61; break;
		case 2: // 260 km/h (Japan)
			speed = 0x60; break;
		case 3: // 280 km/h (ECE/Japan)
			speed = 0x5E; break;
		case 4: // 300 km/h (ECE/Japan)
			speed = 0x5C; break;
		case 5: // 150 mph (ECE/US)
			speed = 0x63; break;
		case 6: // 160 mph (ECE/US)
			speed = 0x61; break;
		case 7: // 170 mph (ECE/US)
			speed = 0x5F; break;
		case 8: // 180 mph (ECE/US)
			speed = 0x5D; break;
		case 9: // 300 km/h (Alpina)
			speed = 0x5F; break;
		case 10: // 320 km/h (Alpina)
			speed = 0x5A; break;
	}

	m_info.setData(0x19, speed, 0xFE);
	m_info.setData(0x1B, ~speed, 0xFE);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboMaxRPM_currentIndexChanged(int index)
{
	unsigned rpm = 0x56;

	switch ( index )
	{
		case 0:	// 7000 rpm 3 imp/kwu
			rpm = 0x56; break;
		case 1:	// 8000 rpm 3 imp/kwu
			rpm = 0x50; break;
		case 2: // 7000 rpm 4 imp/kwu
			rpm = 0x48; break;
	}

	m_info.setData(0x1A, rpm, 0xFE);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboSeries_currentIndexChanged(int index)
{
	m_info.setData(0x32, index, 0x20);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboCheckControls_currentIndexChanged(int index)
{
	m_info.setData(0x27, index, 0x0F);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboLanguage_currentIndexChanged(int index)
{
	m_info.setData(0x27, index, 0xF0);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboUnitLength_currentIndexChanged(int index)
{
	m_info.setData(0x19, index, 0x01);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboUnitConsuption_currentIndexChanged(int index)
{
	m_info.setData(0x33, index, 0xC0);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboUnitTemperature_currentIndexChanged(int index)
{
	m_info.setData(0x33, index, 0x20);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboTimeFormat_currentIndexChanged(int index)
{
	m_info.setData(0x33, index, 0x10);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboGearDisplay_currentIndexChanged(int index)
{
	unsigned display = 0;

	switch ( index )
	{
		case 0:	// No indication
			display = 0x00; break;
		case 1:	// Gear
			display = 0x04; break;
		case 2: // Gear and programm
			display = 0x05; break;
	}

	m_info.setData(0x1D, display, 0x07);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboCountry_currentIndexChanged(int index)
{
	unsigned country = 0;

	switch ( index )
	{
		case 0: // ECE (Deutsch)
			country = 0x00; break;
		case 1: // US
			country = 0x01; break;
		case 8: // Switzerland (French)
			country = 0x02; break;
		case 2: // Japan
			country = 0x03; break;
		case 9: // km_tacho_und_englisch
			country = 0x04; break;
		case 3: // ECE (France)
			country = 0x05; break;
		case 4: // ECE (Spain)
			country = 0x06; break;
		case 5: // ECE (Italia)
			country = 0x07; break;
		case 7: // Switzerland (German)
			country = 0x08; break;
		case 6: // ECE (RHD)
			country = 0x09; break;
	}

	m_info.setData(0x32, country, 0x1F);
	m_info.setData(0x33, country, 0xF);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_spinInspectionPeriod_editingFinished()
{
	m_info.setData(0x1F, ui->spinInspectionPeriod->value() / 200, 0xFF);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_spinOilService_editingFinished()
{
	m_info.setData(0x1D, ui->spinInspectionPeriod->value(), 0x38);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_spinKzahl_editingFinished()
{
	unsigned kzahl;

	kzahl = ui->spinKzahl->value();

	kzahl -= 4000;
	m_info.setData(0x14, (kzahl >> 8), 0x03);
	m_info.setData(0x15, kzahl, 0xFF);
	m_info.setData(0x16, ~(kzahl >> 8), 0x03);
	m_info.setData(0x17, ~kzahl, 0xFF);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboOvertemperature_currentIndexChanged(int index)
{
	unsigned temp = 0x1A;

	switch ( index )
	{
		case 0: // 121
			temp = 0x1A; break;
		case 1: // 125
			temp = 0x17; break;
		case 2: // 130
			temp = 0x14; break;
	}

	m_info.setData(0x25, temp, 0xFF);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_spinSpeedLimit_editingFinished()
{
	unsigned speed;

	speed = ui->spinSpeedLimit->value();

	//speed /= 2;
	speed -= 50;
	m_info.setData(0x22, speed, 0xFF);
	m_info.setData(0x24, ~speed, 0xFF);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_checkSpeedWarning_toggled(bool checked)
{
	if ( checked )
	{
		ui->spinSpeedLimit->show();
		on_spinSpeedLimit_editingFinished();
	}
	else
	{
		ui->spinSpeedLimit->hide();
		m_info.setData(0x22, 0xFF, 0xFF);
		m_info.setData(0x24, 0x00, 0xFF);
		m_info.updateProperties();
		updateDump();
	}
}

void MainWindow::on_comboKVAoil_currentIndexChanged(int index)
{
	m_info.setData(0x1B, index, 0x01);
	m_info.updateProperties();
	updateDump();
}

void MainWindow::on_comboTank_currentIndexChanged(int index)
{
	m_info.setTank( index );
}

void MainWindow::on_comboTempCurve_currentIndexChanged(int index)
{
	m_info.setTempSensor( index );
}

void MainWindow::on_editPartNum_editingFinished()
{
	m_info.setPartNum( ui->editPartNum->text().toLong() );
}

void MainWindow::on_editCode_editingFinished()
{
	m_info.setCodeNum( ui->editCode->text().toLong() );
}

void MainWindow::on_spinInspectionTime_editingFinished()
{
	unsigned time;

	time = ui->spinInspectionTime->value();

	m_info.setData(0x1C, time >> 3, 0xFF);
	m_info.setData(0x1D, time, 0xC0);
	m_info.updateProperties();
	updateDump();
}
