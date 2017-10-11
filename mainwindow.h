#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clusterinfo.h"
#include "hexviewmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void update_view();

private:
	void saveFile( bool swap );

private slots:
	void on_chkSwapBytes_toggled(bool checked);

	void on_butOpen_clicked();
	void on_butSave_clicked();
	void on_butSaveSwap_clicked();

	void on_editVIN_editingFinished();
	void on_spinOdometer_editingFinished();

private:
	Ui::MainWindow *ui;
	ClusterInfo m_info;
	HexViewModel *m_dumpmodel;

private slots:
	void updateDump();
	void on_comboEngine_currentIndexChanged(int index);
	void on_comboMaxSpeed_currentIndexChanged(int index);
	void on_comboMaxRPM_currentIndexChanged(int index);
	void on_comboSeries_currentIndexChanged(int index);
	void on_comboCheckControls_currentIndexChanged(int index);
	void on_comboLanguage_currentIndexChanged(int index);
	void on_comboUnitLength_currentIndexChanged(int index);
	void on_comboUnitConsuption_currentIndexChanged(int index);
	void on_comboUnitTemperature_currentIndexChanged(int index);
	void on_comboTimeFormat_currentIndexChanged(int index);
	void on_comboGearDisplay_currentIndexChanged(int index);
	void on_comboCountry_currentIndexChanged(int index);
	void on_spinInspectionPeriod_editingFinished();
	void on_spinOilService_editingFinished();
	void on_comboOvertemperature_currentIndexChanged(int index);
	void on_spinKzahl_editingFinished();
	void on_spinSpeedLimit_editingFinished();
	void on_checkSpeedWarning_toggled(bool checked);
	void on_comboKVAoil_currentIndexChanged(int index);
	void on_comboTank_currentIndexChanged(int index);
	void on_comboTempCurve_currentIndexChanged(int index);
	void on_editPartNum_editingFinished();
	void on_editCode_editingFinished();
	void on_spinInspectionTime_editingFinished();
};

#endif // MAINWINDOW_H
