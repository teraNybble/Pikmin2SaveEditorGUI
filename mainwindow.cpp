#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "offsets.h"
#include <climits>

#include <iostream>
#include <fstream>
#include <cstdio>

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	QRegularExpression r("doubleSpinBox_[\\S]*");
	//find all double spinboxes
	QList<QDoubleSpinBox*> spinBoxes = ui->tabWidget->findChildren<QDoubleSpinBox*>(r);
	//set all min and max to 0 and uint max respectivly
	for(auto it : spinBoxes){
		it->setMinimum(0);
		it->setMaximum(UINT_MAX);
	}
	r.setPattern("spinBox_[\\S]*");
	QList<QSpinBox*> alsoSpinBoxes = ui->tabWidget->findChildren<QSpinBox*>(r);
	//sets the maximum value an above ground treasure counter can be to ubyte max
	//and the minimum to 0
	for(auto it : alsoSpinBoxes){
		it->setMinimum(0);
		it->setMaximum(0xFF);
	}

	savePath = 	QFileDialog::getOpenFileName(this,"Open Pikmin 2 Save File","","*.gci");
	readData();
}

MainWindow::~MainWindow()
{
	if(saveFile.is_open()){
		saveFile.close();
	}
	delete ui;
}

bool MainWindow::readData()
{
	//set the inital value to be -1 that way if no offset is found i'll know
	for(int i = 0; i < 3; i++) { saves[i] = -1; }

	//qDebug() << savePath;

	saveFile.open(savePath.toStdString(), std::ios::in | std::ios::out | std::ios::binary);

	if(!saveFile.is_open()){
		//qDebug() << "Error: " << strerror(errno);
		QMessageBox msgBox;
		msgBox.setText("Error opening file\n" + QString(strerror(errno)));
		msgBox.exec();
		//return false;
	}

	//TODO: get save offsets
	getSaveOffsets(saveFile,saves);
	//read saveFile Data and insert it into the correct boxes

	for(int i = 0; i < 3; i++){
		if(saves[i] != -1){
			ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_dayDisp_save"+QString::number(i))->setValue(readDispDayCount(saveFile,saves[i]));
			ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_dayReal_save"+QString::number(i))->setValue(readRealDayCount(saveFile,saves[i])+1);//the internal day is zero indexed
			ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_pokoDisp_save"+QString::number(i))->setValue(readDispPokoCount(saveFile,saves[i]));
			ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_pokoReal_save"+QString::number(i))->setValue(readRealPokoCount(saveFile,saves[i]));
			ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_treasure_save"+QString::number(i))->setValue(readTreasureCount(saveFile,saves[i]));
			ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_playTime_save"+QString::number(i))->setValue(readPlayTime(saveFile,saves[i]));
			int piks[21];
			readOnionPiks(saveFile,saves[i],piks);
			for (int j = 0; j < 21; j ++) {
				ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_pik"+pikColours[j/3]+pikTypes[j%3]+"_save"+QString::number(i))->setValue(piks[j]);
			}
			readSkyPiks(saveFile,saves[i],piks);
			for (int j = 0; j < 21; j ++) {
				ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_skyPik"+pikColours[j/3]+pikTypes[j%3]+"_save"+QString::number(i))->setValue(piks[j]);
			}
			for(int j = 0; j < 4; j++) {
				int area = ((areaProgressReadFuncs[j])(saveFile,saves[i]) & 0xFF);
				for(int k = 0; k < 8; k++) {
					ui->tabWidget->findChild<QCheckBox*>("checkBox_"+areaAbbrevations[j]+QString::number(k)+"_save"+QString::number(i))->setChecked(area & (1<<k));
				}
			}
			int val = 0;
			//TODO read in above ground treasures
			val = readValleyOfReposeTreasure(saveFile,saves[i]);
			ui->tabWidget->findChild<QSpinBox*>("spinBox_vorAboveTreasure_save"+QString::number(i))->setValue(val);
			val = readAwakeningWoodTreasure(saveFile,saves[i]);
			ui->tabWidget->findChild<QSpinBox*>("spinBox_awAboveTreasure_save"+QString::number(i))->setValue(val);
			val = readPerplexingPoolTreasure(saveFile,saves[i]);
			ui->tabWidget->findChild<QSpinBox*>("spinBox_ppAboveTreasure_save"+QString::number(i))->setValue(val);
			val = readWistfulWildTreasure(saveFile,saves[i]);
			ui->tabWidget->findChild<QSpinBox*>("spinBox_wwAboveTreasure_save"+QString::number(i))->setValue(val);
			val = readUnlockedPikmin(saveFile,saves[i]);
			for(int j = 0; j < 7; j++) {
				ui->tabWidget->findChild<QCheckBox*>("checkBox_oc"+pikColours[j]+"_save"+QString::number(i))->setChecked(val & (1<<j));
			}
			val = readAwakeOnions(saveFile,saves[i]);
			for(int j = 0; j < 7; j++) {
				ui->tabWidget->findChild<QCheckBox*>("checkBox_oa"+pikColours[j]+"_save"+QString::number(i))->setChecked(val & (1<<j));
			}
			val = readUnlockedPellet(saveFile,saves[i]);
			for(int j = 0; j < 7; j++) {
				ui->tabWidget->findChild<QCheckBox*>("checkBox_pc"+pikColours[j]+"_save"+QString::number(i))->setChecked(val & (1<<j));
			}
			char arr[9 * 0x65];
			readCharArr(saveFile, 0x53, saves[i], 9 * 0x65, arr);
			for (int j = 0; j < 9 * 0x65; j += 9) {
				ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_eny"+enyNames[j/9]+"PikLost_save"+QString::number(i))->setValue(charByteToIntBE(arr, j+4));
				ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_eny"+enyNames[j/9]+"Killed_save"+QString::number(i))->setValue(charByteToIntBE(arr, j));
				if(((int)arr[j+8]) == 3) { arr[j+8]  = (char)(2); }
				ui->tabWidget->findChild<QCheckBox*>("checkBox_eny"+enyNames[j/9]+"_save"+QString::number(i))->setCheckState((Qt::CheckState)((int)arr[j+8]));
					//qDebug() << names[i/9] << "" << charByteToIntBE(arr, j+4) << "\t" << charByteToIntBE(arr, j) << "\t" << (int)arr[j + 8] << "\n";
			}
			ui->tabWidget->findChild<QSpinBox*>("spinBox_mapState_save"+QString::number(i))->setValue(readMapState(saveFile,saves[i]));
			ui->tabWidget->findChild<QComboBox*>("comboBox_shipPos_save"+QString::number(i))->setCurrentIndex(readAreaID(saveFile,saves[i]));
			ui->tabWidget->findChild<QSpinBox*>("spinBox_caveNum_save"+QString::number(i))->setValue(readCaveID(saveFile,saves[i]));
			ui->tabWidget->findChild<QSpinBox*>("spinBox_sublevel_save"+QString::number(i))->setValue(readSublevelFloor(saveFile,saves[i])+1);
			for(int j = 0; j < SIZE_TREASURE_STAT; j++){
				Qt::CheckState state;
				switch(readByte(saveFile,saves[i],TREASURE_STAT+j,1))
				{
				case 0:
					state = Qt::CheckState::Unchecked;
					break;
				case 2:
					state = Qt::CheckState::PartiallyChecked;
					break;
				case 6:
					state = Qt::CheckState::Checked;
					break;
				default:
					state = Qt::CheckState::PartiallyChecked;
					qDebug() << "triggered default case\n";
					break;
				}
				//qDebug() << j << "\t" << treasureNames[j];
				ui->tabWidget->findChild<QCheckBox*>("checkBox_treasure"+ treasureNames[j] +"_save"+QString::number(i))->setCheckState(state);

			}
			for(int j = 0; j < SIZE_EX_KIT_STAT; j++){
				Qt::CheckState state;
				switch(readByte(saveFile,saves[i],EX_KIT_STAT+j,1))
				{
				case 0:
					state = Qt::CheckState::Unchecked;
					break;
				case 2:
					state = Qt::CheckState::PartiallyChecked;
					break;
				case 6:
					state = Qt::CheckState::Checked;
					break;
				default:
					state = Qt::CheckState::PartiallyChecked;
					//qDebug() << "triggered default case\n";
					break;
				}
				//qDebug() << j << "\t" << exNames[j];
				ui->tabWidget->findChild<QCheckBox*>("checkBox_ex"+ exNames[j] +"_save"+QString::number(i))->setCheckState(state);

			}
			//qDebug() << "Almost Done";
			ui->tabWidget->findChild<QCheckBox*>("checkBox_new_save"+QString::number(i))->setChecked(!readNew(saveFile,saves[i]));
			short exkit = readShort(saveFile,EX_KIT,saves[i],SIZE_EX_KIT);
			//qDebug() << exkit << "\n";
			for(int j = 0; j < SIZE_EX_KIT_STAT; j++){
				//qDebug() << j << "\t" << (exkit & (1<<j));
				ui->tabWidget->findChild<QCheckBox*>("checkBox_exkit"+ exNames[j] +"_save"+QString::number(i))->setChecked(exkit & (1<<j));
			}
			ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_spicySpray_save"+QString::number(i))->setValue(readSpicy(saveFile,saves[i]));
			ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_bitterSpray_save"+QString::number(i))->setValue(readBitter(saveFile,saves[i]));
		} else {
			disableButtons(i);
		}
	}

	return true;
}

void MainWindow::disableButtons(int i)
{
	//find all the input boxes and disable them
	QRegularExpression r("doubleSpinBox_[\\S]*_save"+QString::number(i));
	QList<QDoubleSpinBox*> spinBoxes = ui->tabWidget->findChildren<QDoubleSpinBox*>(r);
	for(auto it : spinBoxes){
		it->setEnabled(false);
	}
	r.setPattern("checkBox_[\\S]*_save"+QString::number(i));
	QList<QCheckBox*> checkBoxes = ui->tabWidget->findChildren<QCheckBox*>(r);
	for(auto it : checkBoxes){
		it->setEnabled(false);
	}
	r.setPattern("spinBox_[\\S]*_save"+QString::number(i));
	QList<QSpinBox*> treasureSpinBox = ui->tabWidget->findChildren<QSpinBox*>(r);
	for(auto it : treasureSpinBox){
		it->setEnabled(false);
	}
	r.setPattern("pushButton_[\\S]*_save"+QString::number(i));
	QList<QPushButton*> buttons = ui->tabWidget->findChildren<QPushButton*>(r);
	for(auto it : buttons){
		it->setEnabled(false);
	}
	//comboBox_shipPos_save0
	r.setPattern("comboBox_[\\S]*_save"+QString::number(i));
	QList<QComboBox*> box = ui->tabWidget->findChildren<QComboBox*>(r);
	for(auto it : box){
		it->setEnabled(false);
	}
}

void MainWindow::enableButtons(int i)
{
	//find all the input boxes and disable them
	QRegularExpression r("doubleSpinBox_[\\S]*_save"+QString::number(i));
	QList<QDoubleSpinBox*> spinBoxes = ui->tabWidget->findChildren<QDoubleSpinBox*>(r);
	for(auto it : spinBoxes){
		it->setEnabled(true);
	}
	r.setPattern("checkBox_[\\S]*_save"+QString::number(i));
	QList<QCheckBox*> checkBoxes = ui->tabWidget->findChildren<QCheckBox*>(r);
	for(auto it : checkBoxes){
		it->setEnabled(true);
	}
	r.setPattern("spinBox_[\\S]*_save"+QString::number(i));
	QList<QSpinBox*> treasureSpinBox = ui->tabWidget->findChildren<QSpinBox*>(r);
	for(auto it : treasureSpinBox){
		it->setEnabled(true);
	}
	r.setPattern("pushButton_[\\S]*_save"+QString::number(i));
	QList<QPushButton*> buttons = ui->tabWidget->findChildren<QPushButton*>(r);
	for(auto it : buttons){
		it->setEnabled(true);
	}
	r.setPattern("comboBox_[\\S]*_save"+QString::number(i));
	QList<QComboBox*> box = ui->tabWidget->findChildren<QComboBox*>(r);
	for(auto it : box){
		it->setEnabled(true);
	}
}

void MainWindow::writeToSave(int i)
{
	//int i = 0;
	writeDispDayCount(saveFile,saves[i],(int)ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_dayDisp_save"+QString::number(i))->value());
	writeRealDayCount(saveFile,saves[i],(int)ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_dayReal_save"+QString::number(i))->value()-1);//the internal day is zero indexed
	writeDispPokoCount(saveFile,saves[i],ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_pokoDisp_save"+QString::number(i))->value());
	writeRealPokoCount(saveFile,saves[i],ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_pokoReal_save"+QString::number(i))->value());
	writeTreasureCount(saveFile,saves[i],ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_treasure_save"+QString::number(i))->value());
	writePlayTime(saveFile,saves[i],ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_playTime_save"+QString::number(i))->value());

	for (int j = 0; j < 21; j++) {
		//qDebug() << pikColours[j/3] << " " << pikTypes[j%3] << " " << ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_skyPik"+pikColours[j/3]+pikTypes[j%3]+"_save"+QString::number(i))->value();
		//qDebug() << "\n";
		writeOnionPiks(saveFile,saves[i],j*4,ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_pik"+pikColours[j/3]+pikTypes[j%3]+"_save"+QString::number(i))->value());
	}
	for (int j = 0; j < 21; j++) {
		//qDebug() << pikColours[j/3] << " " << pikTypes[j%3] << " " << ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_skyPik"+pikColours[j/3]+pikTypes[j%3]+"_save"+QString::number(i))->value();
		writeSkyPiks(saveFile,saves[i],j*4,ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_skyPik"+pikColours[j/3]+pikTypes[j%3]+"_save"+QString::number(i))->value());
	}
	for(int j = 0; j < 4; j++) {
		int area = 0;
		for(int k = 0; k < 8; k++) {
			area = area | (ui->tabWidget->findChild<QCheckBox*>("checkBox_"+areaAbbrevations[j]+QString::number(k)+"_save"+QString::number(i))->isChecked() << k);
		}
		(areaProgressWriteFuncs[j])(saveFile,saves[i],area);
	}

	writeValleyOfReposeTreasure(saveFile,saves[i],ui->tabWidget->findChild<QSpinBox*>("spinBox_vorAboveTreasure_save"+QString::number(i))->value());
	writeAwakeningWoodTreasure(saveFile,saves[i],ui->tabWidget->findChild<QSpinBox*>("spinBox_awAboveTreasure_save"+QString::number(i))->value());
	writePerplexingPoolTreasure(saveFile,saves[i],ui->tabWidget->findChild<QSpinBox*>("spinBox_ppAboveTreasure_save"+QString::number(i))->value());
	writeWistfulWildTreasure(saveFile,saves[i],ui->tabWidget->findChild<QSpinBox*>("spinBox_wwAboveTreasure_save"+QString::number(i))->value());

	int val = 0;
	for(int j = 0; j < 7; j++) {
		val = val | (ui->tabWidget->findChild<QCheckBox*>("checkBox_oc"+pikColours[j]+"_save"+QString::number(i))->isChecked() << j);
	}
	writeUnlockedPikmin(saveFile,saves[i],val);

	val = 0;
	for(int j = 0; j < 7; j++) {
		val = val | (ui->tabWidget->findChild<QCheckBox*>("checkBox_oa"+pikColours[j]+"_save"+QString::number(i))->isChecked() << j);
	}
	writeAwakeOnions(saveFile,saves[i],val);

	val = 0;
	for(int j = 0; j < 7; j++) {
		val = val | (ui->tabWidget->findChild<QCheckBox*>("checkBox_pc"+pikColours[j]+"_save"+QString::number(i))->isChecked() << j);
	}
	writeUnlockedPellet(saveFile,saves[i],val);

	char arr[9 * 0x65];
	for (int j = 0; j < 9 * 0x65; j += 9) {
		intToCharByteBE(ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_eny"+enyNames[j/9]+"PikLost_save"+QString::number(i))->value(),arr+j+4);
		intToCharByteBE(ui->tabWidget->findChild<QDoubleSpinBox*>("doubleSpinBox_eny"+enyNames[j/9]+"Killed_save"+QString::number(i))->value(), arr+j);
		//if(((int)arr[j+8]) == 3) { arr[j+8]  = (char)(2); }
		//ui->tabWidget->findChild<QCheckBox*>("checkBox_eny"+enyNames[j/9]+"_save"+QString::number(i))->setCheckState((Qt::CheckState)((int)arr[j+8]));
		switch (ui->tabWidget->findChild<QCheckBox*>("checkBox_eny"+enyNames[j/9]+"_save"+QString::number(i))->checkState()) {
			case Qt::CheckState::Unchecked:
			arr[j+8] = (char)0;
			break;
		case Qt::CheckState::PartiallyChecked:
			arr[j+8] = (char)1;
			break;
		case Qt::CheckState::Checked:
			arr[j+8] = (char)3;
			break;
		}
		//writeInt(saveFile,0x53,saves[0],)
			//qDebug() << names[j/9] << "" << charByteToIntBE(arr, j+4) << "\t" << charByteToIntBE(arr, j) << "\t" << (int)arr[j + 8] << "\n";
	}
	for(int j = 0; j < 9 * 0x65; j++){
		writeByte(saveFile,0x53+j,saves[i],arr[j]);
	}
	writeMapState(saveFile,saves[i],ui->tabWidget->findChild<QSpinBox*>("spinBox_mapState_save"+QString::number(i))->value());
	writeAreaID(saveFile,saves[i],ui->tabWidget->findChild<QComboBox*>("comboBox_shipPos_save"+QString::number(i))->currentIndex());
	writeCaveID(saveFile,saves[i],ui->tabWidget->findChild<QSpinBox*>("spinBox_caveNum_save"+QString::number(i))->value());
	writeSublevelFloor(saveFile,saves[i],ui->tabWidget->findChild<QSpinBox*>("spinBox_sublevel_save"+QString::number(i))->value()-1);
	switch(ui->tabWidget->findChild<QComboBox*>("comboBox_shipPos_save"+QString::number(i))->currentIndex())
	{
	case 0:
		writeAreaLetter(saveFile,saves[i],0x75);
		break;
	case 1:
		writeAreaLetter(saveFile,saves[i],0x66);
		break;
	case 2:
		writeAreaLetter(saveFile,saves[i],0x79);
		break;
	case 3:
		writeAreaLetter(saveFile,saves[i],0x6C);
		break;
	}

	for(int j = 0; j < SIZE_TREASURE_STAT; j++){
		unsigned int val = 0;
		switch(ui->tabWidget->findChild<QCheckBox*>("checkBox_treasure"+ treasureNames[j] +"_save"+QString::number(i))->checkState())
		{
		case Qt::CheckState::Unchecked:
			val = 0;
			break;
		case Qt::CheckState::PartiallyChecked:
			val = 2;
			break;
		case Qt::CheckState::Checked:
			val = 6;
			break;
		default:
			val = 2;
			break;
		}
		writeByte(saveFile,saves[i],TREASURE_STAT+j,val);
		//qDebug() << j << "\t" << treasureNames[j];

	}

	for(int j = 0; j < SIZE_EX_KIT_STAT; j++){
		unsigned int val = 0;
		switch(ui->tabWidget->findChild<QCheckBox*>("checkBox_ex"+ exNames[j] +"_save"+QString::number(i))->checkState())
		{
		case Qt::CheckState::Unchecked:
			val = 0;
			break;
		case Qt::CheckState::PartiallyChecked:
			val = 2;
			break;
		case Qt::CheckState::Checked:
			val = 6;
			break;
		default:
			val = 2;
			break;
		}
		//qDebug() << j << "\t" << exNames[j];
		writeByte(saveFile,saves[i],EX_KIT_STAT+j,val);
		//ui->tabWidget->findChild<QCheckBox*>("checkBox_ex"+ exNames[j] +"_save"+QString::number(i))->setCheckState(state);

	}
	writeNew(saveFile,saves[i],!(ui->tabWidget->findChild<QCheckBox*>("checkBox_new_save"+QString::number(i))->isChecked()));

	short exkit = 0;
	//qDebug() << exkit << "\n";
	for(int j = 0; j < SIZE_EX_KIT_STAT; j++){
		//qDebug() << j << "\t" << (exkit & (1<<j));
		exkit |= (ui->tabWidget->findChild<QCheckBox*>("checkBox_exkit"+ exNames[j] +"_save"+QString::number(i))->isChecked() << j);
	}
	writeShort(saveFile,EX_KIT,saves[i],exkit);

	//checksum
	int checkSum = calculateCheckSum(saveFile, saves[i]);
	saveFile.seekp(PIK_SAV_CHKSUM_SRT+saves[i]);
	char* tempCheckSum = new char[4];
	intToCharByteBE(checkSum, tempCheckSum);
	saveFile.write(tempCheckSum, 4);
}

// save0
void MainWindow::on_checkBox_linkPoko_save0_stateChanged(int arg1)
{
	ui->doubleSpinBox_pokoDisp_save0->setEnabled(!arg1);

	if(!ui->doubleSpinBox_pokoDisp_save0->isEnabled()){
		ui->doubleSpinBox_pokoDisp_save0->setValue(ui->doubleSpinBox_pokoReal_save0->value());
	}
}

void MainWindow::on_doubleSpinBox_dayReal_save0_valueChanged(double arg1)
{
	if(!ui->doubleSpinBox_dayDisp_save0->isEnabled()){
		ui->doubleSpinBox_dayDisp_save0->setValue(arg1);
	}
}

void MainWindow::on_doubleSpinBox_pokoReal_save0_valueChanged(double arg1)
{
	if(!ui->doubleSpinBox_pokoDisp_save0->isEnabled()){
		ui->doubleSpinBox_pokoDisp_save0->setValue(arg1);
	}
}

void MainWindow::on_checkBox_linkDay_save0_stateChanged(int arg1)
{
	ui->doubleSpinBox_dayDisp_save0->setEnabled(!arg1);

	if(!ui->doubleSpinBox_dayDisp_save0->isEnabled()){
		ui->doubleSpinBox_dayDisp_save0->setValue(ui->doubleSpinBox_dayReal_save0->value());
	}
}

void MainWindow::on_pushButton_reRead_save0_clicked()
{
	if(saveFile.is_open()){
		saveFile.close();
	}
	readData();
	enableButtons(0);
	ui->doubleSpinBox_dayDisp_save0->setEnabled(!(ui->checkBox_linkDay_save0->isChecked()));
	ui->doubleSpinBox_pokoDisp_save0->setEnabled(!(ui->checkBox_linkPoko_save0->isChecked()));
	//ui->pushButton_write_save0->setEnabled(true);
}

void MainWindow::on_pushButton_write_save0_clicked()
{
	writeToSave(0);
	QMessageBox msgBox;
	msgBox.setText("Sucsesfully written to save");
	msgBox.exec();
}

void MainWindow::on_pushButton_close_save0_clicked()
{
	if(saveFile.is_open()){
		saveFile.close();
	}
	disableButtons(0);
	ui->pushButton_reRead_save0->setEnabled(true);
}

void MainWindow::on_checkBox_treasureAll_save0_stateChanged(int arg1)
{
	QRegularExpression r("checkBox_treasure[\\S]*_save0");
	//r.setPattern("checkBox_treasure[\\S]*_save0"+QString::number(i));
	QList<QCheckBox*> checkBoxes = ui->tabWidget->findChildren<QCheckBox*>(r);
	for(auto it : checkBoxes){
		it->setCheckState((Qt::CheckState)arg1);
	}
	r.setPattern("checkBox_ex[\\S]*_save0");
	//r.setPattern("checkBox_treasure[\\S]*_save0"+QString::number(i));
	QList<QCheckBox*> exCheckBoxes = ui->tabWidget->findChildren<QCheckBox*>(r);
	for(auto it : exCheckBoxes){
		it->setCheckState((Qt::CheckState)arg1);
	}
}

//save 1
void MainWindow::on_checkBox_linkPoko_save1_stateChanged(int arg1)
{
	ui->doubleSpinBox_pokoDisp_save1->setEnabled(!arg1);

	if(!ui->doubleSpinBox_pokoDisp_save1->isEnabled()){
		ui->doubleSpinBox_pokoDisp_save1->setValue(ui->doubleSpinBox_pokoReal_save1->value());
	}
}

void MainWindow::on_doubleSpinBox_dayReal_save1_valueChanged(double arg1)
{
	if(!ui->doubleSpinBox_dayDisp_save1->isEnabled()){
		ui->doubleSpinBox_dayDisp_save1->setValue(arg1);
	}
}

void MainWindow::on_doubleSpinBox_pokoReal_save1_valueChanged(double arg1)
{
	if(!ui->doubleSpinBox_pokoDisp_save1->isEnabled()){
		ui->doubleSpinBox_pokoDisp_save1->setValue(arg1);
	}
}

void MainWindow::on_checkBox_linkDay_save1_stateChanged(int arg1)
{
	ui->doubleSpinBox_dayDisp_save1->setEnabled(!arg1);

	if(!ui->doubleSpinBox_dayDisp_save1->isEnabled()){
		ui->doubleSpinBox_dayDisp_save1->setValue(ui->doubleSpinBox_dayReal_save1->value());
	}
}

void MainWindow::on_pushButton_reRead_save1_clicked()
{
	if(saveFile.is_open()){
		saveFile.close();
	}
	readData();
	enableButtons(1);
	ui->doubleSpinBox_dayDisp_save1->setEnabled(!(ui->checkBox_linkDay_save1->isChecked()));
	ui->doubleSpinBox_pokoDisp_save1->setEnabled(!(ui->checkBox_linkPoko_save1->isChecked()));
	//ui->pushButton_write_save1->setEnabled(true);
}

void MainWindow::on_pushButton_write_save1_clicked()
{
	writeToSave(1);
	QMessageBox msgBox;
	msgBox.setText("Sucsesfully written to save");
	msgBox.exec();
}

void MainWindow::on_pushButton_close_save1_clicked()
{
	if(saveFile.is_open()){
		saveFile.close();
	}
	disableButtons(1);
	ui->pushButton_reRead_save1->setEnabled(true);
}

void MainWindow::on_checkBox_treasureAll_save1_stateChanged(int arg1)
{
	QRegularExpression r("checkBox_treasure[\\S]*_save1");
	//r.setPattern("checkBox_treasure[\\S]*_save0"+QString::number(i));
	QList<QCheckBox*> checkBoxes = ui->tabWidget->findChildren<QCheckBox*>(r);
	for(auto it : checkBoxes){
		it->setCheckState((Qt::CheckState)arg1);
	}
	r.setPattern("checkBox_ex[\\S]*_save1");
	//r.setPattern("checkBox_treasure[\\S]*_save0"+QString::number(i));
	QList<QCheckBox*> exCheckBoxes = ui->tabWidget->findChildren<QCheckBox*>(r);
	for(auto it : exCheckBoxes){
		it->setCheckState((Qt::CheckState)arg1);
	}
}

//save 2
void MainWindow::on_checkBox_linkPoko_save2_stateChanged(int arg1)
{
	ui->doubleSpinBox_pokoDisp_save2->setEnabled(!arg1);

	if(!ui->doubleSpinBox_pokoDisp_save2->isEnabled()){
		ui->doubleSpinBox_pokoDisp_save2->setValue(ui->doubleSpinBox_pokoReal_save2->value());
	}
}

void MainWindow::on_doubleSpinBox_dayReal_save2_valueChanged(double arg1)
{
	if(!ui->doubleSpinBox_dayDisp_save2->isEnabled()){
		ui->doubleSpinBox_dayDisp_save2->setValue(arg1);
	}
}

void MainWindow::on_doubleSpinBox_pokoReal_save2_valueChanged(double arg1)
{
	if(!ui->doubleSpinBox_pokoDisp_save2->isEnabled()){
		ui->doubleSpinBox_pokoDisp_save2->setValue(arg1);
	}
}

void MainWindow::on_checkBox_linkDay_save2_stateChanged(int arg1)
{
	ui->doubleSpinBox_dayDisp_save2->setEnabled(!arg1);

	if(!ui->doubleSpinBox_dayDisp_save2->isEnabled()){
		ui->doubleSpinBox_dayDisp_save2->setValue(ui->doubleSpinBox_dayReal_save2->value());
	}
}

void MainWindow::on_pushButton_reRead_save2_clicked()
{
	if(saveFile.is_open()){
		saveFile.close();
	}
	readData();
	enableButtons(2);
	ui->doubleSpinBox_dayDisp_save2->setEnabled(!(ui->checkBox_linkDay_save2->isChecked()));
	ui->doubleSpinBox_pokoDisp_save2->setEnabled(!(ui->checkBox_linkPoko_save2->isChecked()));
	//ui->pushButton_write_save2->setEnabled(true);
}

void MainWindow::on_pushButton_write_save2_clicked()
{
	writeToSave(2);
	QMessageBox msgBox;
	msgBox.setText("Sucsesfully written to save");
	msgBox.exec();
}

void MainWindow::on_pushButton_close_save2_clicked()
{
	if(saveFile.is_open()){
		saveFile.close();
	}
	disableButtons(2);
	ui->pushButton_reRead_save2->setEnabled(true);
}

void MainWindow::on_checkBox_treasureAll_save2_stateChanged(int arg1)
{
	QRegularExpression r("checkBox_treasure[\\S]*_save2");
	//r.setPattern("checkBox_treasure[\\S]*_save0"+QString::number(i));
	QList<QCheckBox*> checkBoxes = ui->tabWidget->findChildren<QCheckBox*>(r);
	for(auto it : checkBoxes){
		it->setCheckState((Qt::CheckState)arg1);
	}
	r.setPattern("checkBox_ex[\\S]*_save2");
	//r.setPattern("checkBox_treasure[\\S]*_save0"+QString::number(i));
	QList<QCheckBox*> exCheckBoxes = ui->tabWidget->findChildren<QCheckBox*>(r);
	for(auto it : exCheckBoxes){
		it->setCheckState((Qt::CheckState)arg1);
	}
}
