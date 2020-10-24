/*
This file is part of ARehab.

ARehab is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3 of the License.

ARehab is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.If not, see <http://www.gnu.org/licenses/>.

Copyright 2015 Jacobo de Haro
jacobodeharo@gmail.com
@jacobodeharo

*/

#include "ARehab_vTerapeuta.h"

namespace ARehabGUIDesigner
{

	ARehabMainWindow::ARehabMainWindow(QWidget *parent)
		: QMainWindow(parent),
		state(KINECT_INITIAL),
		guistatewidget(new QGUIStateWidget()),
		jointSelectorWidget(new QJointSelectorWidget()),
		glwidget(new QGLViewer(this)),
		glwidget2(new QGLViewer(this)),
		kinectController(0),
		fileReaderController(0),
		fileWriterController(0),		
		timerFrames(0),
		timerFrames2(0),
		kinectControlWidget(new QKinectControlWidget()),
		arehabfileControlWidget(new QARehabFileControlWidget()),
		dirFiles("./ARehabFiles"),
		fileDialog(NULL),
		maximized(false)
	{

		this->fileDialog = new QFileDialog(this, tr("Select the file path"), dirFiles.absolutePath(), tr("ARehab Files (*.arehab)"));
		this->kinectController = new KinectController();
		this->fileReaderController = new ARehabFileReaderController();
		this->fileWriterController = new ARehabFileWriterController();
		this->timerFrames = new QTimer(this);
		this->timerFrames2 = new QTimer(this);
		this->timerFrames->setTimerType(Qt::PreciseTimer);
		this->timerFrames2->setTimerType(Qt::PreciseTimer);
		ui.setupUi(this);
		this->initGUI();
		this->connectWidgets();

		if (!dirFiles.exists())
		{
			dirFiles.mkdir("./ARehabFiles");
		}

		this->showMaximized();	
	}

	void ARehabMainWindow::slot_update(void)
	{
		ARehabFrame * frameKinect = NULL,
			*frameFile = NULL;
		unsigned long long int leftFrame = 0, rightFrame = 0;
		switch (this->state)
		{
		case KINECT_PLAY:
			this->glwidget2->slot_pauseAnimation();
			frameKinect = this->kinectController->GetNextARehabFrame();
			if (frameKinect)
				emit newARehabKinectFrame(frameKinect);
			break;

		case KINECT_RECORDING:
			this->glwidget2->slot_pauseAnimation();
			frameKinect = this->kinectController->GetNextARehabFrame();
			if (frameKinect)
			{
				frameKinect->refCount++;
				emit newARehabKinectFrame(frameKinect);
				fileWriterController->StoreARehabFrame(frameKinect);
				frameKinect->freeMem();
			}
			break;

		case CUTTING_INITIAL:
			this->glwidget->slot_pauseAnimation();
			this->timerFrames->stop();
			leftFrame = arehabfileControlWidget->GetLeftFrameIndex();
			rightFrame = arehabfileControlWidget->GetRightFrameIndex();
			this->fileReaderController->ResetCurrentFrame(leftFrame);
			frameFile = fileReaderController->GetiARehabFrame(leftFrame);
			if (frameFile && glwidget2->isVisible()) {
				emit newARehabFileFrame(frameFile);
				this->glwidget2->slot_startAnimation();
			}
			break;

		case CUTTING_MOVLEFT:
			this->glwidget->slot_pauseAnimation();
			frameFile = fileReaderController->GetiARehabFrame(this->arehabfileControlWidget->GetLeftFrameIndex(), true);
			if (frameFile && glwidget2->isVisible()) {
				emit newARehabFileFrame(frameFile);
			}
			this->fileReaderController->ResetCurrentFrame(arehabfileControlWidget->GetLeftFrameIndex());
			this->glwidget2->slot_startAnimation();
			break;

		case CUTTING_MOVRIGHT:
			this->glwidget->slot_pauseAnimation();
			frameFile = fileReaderController->GetiARehabFrame(this->arehabfileControlWidget->GetRightFrameIndex());			
			if (frameFile && glwidget2->isVisible()) {
				emit newARehabFileFrame(frameFile);
			}
			this->glwidget2->slot_startAnimation();
			break;

		case CUTTING_PLAYING:
			this->glwidget->slot_pauseAnimation();
			leftFrame = arehabfileControlWidget->GetLeftFrameIndex();
			rightFrame = arehabfileControlWidget->GetRightFrameIndex();
			frameFile = fileReaderController->GetNextIntervalARehabFrame(leftFrame, rightFrame);
			if (frameFile && glwidget2->isVisible())
			{				
				emit newARehabFileFrame(frameFile);
				unsigned long long int interval = qMax(fileReaderController->currentTimeSpan, fileReaderController->previousTimeSpan) - qMin(fileReaderController->currentTimeSpan, fileReaderController->previousTimeSpan);			
				this->timerFrames2->start(interval);
			}
			this->glwidget2->slot_startAnimation();
			break;
		}
	}

	void ARehabMainWindow::resizeEvent(QResizeEvent * event)
	{
		QGraphicsScene * scene = ui.graphicsInicial->scene();
		scene->setSceneRect(0, 0, ui.graphicsInicial->width(), ui.graphicsInicial->height());
		float wImage = this->pixmapItemInitial->boundingRect().width();
		float hImage = this->pixmapItemInitial->boundingRect().height();
		float wGraphicsView = ui.graphicsInicial->width();
		float hGraphicsView = ui.graphicsInicial->height();
		float scaleH = hGraphicsView / hImage;
		pixmapItemInitial->setScale(scaleH);
		pixmapItemInitial->setPos((wGraphicsView / 2.0f) - (scaleH*wImage / 2.0f), 0);

		QRadialGradient gradient(wGraphicsView / 2.0f, hGraphicsView / 2.0f, hGraphicsView);
		gradient.setColorAt(0.95, QColor(200, 200, 200));
		gradient.setColorAt(0.5, QColor(255, 255, 255));
		gradient.setColorAt(0, QColor(255, 255, 255));
		scene->setBackgroundBrush(QBrush(gradient));

		proxyBtNuevo->setPos((wGraphicsView / 2.0f) - (btNew->width() / 2.0f), hGraphicsView / 2.0f);
		proxyBtLoad->setPos((wGraphicsView / 2.0f) - (btLoad->width() / 2.0f), 80 + (hGraphicsView / 2.0f));
		proxyBtLoadResults->setPos((wGraphicsView / 2.0f) - (btLoadResults->width() / 2.0f), 160 + (hGraphicsView / 2.0f));

		ui.graphicsInicial->update();

		QMainWindow::resizeEvent(event);
	}


	void ARehabMainWindow::initGUI(void)
	{
		this->loadGuiInitial();
		this->loadGuiState1();
		this->loadGuiState2();
		this->loadGuiState3();
		ui.stackedWidget->setCurrentIndex(0);
	}

	void ARehabMainWindow::connectWidgets(void)
	{
		QObject::connect(ui.btFileChooser, &QPushButton::clicked, this, &ARehabMainWindow::slot_actionRutaFichero);
		QObject::connect(ui.txNombreEjercicio, &QLineEdit::textChanged, this, &ARehabMainWindow::slot_validateTXName);
		QObject::connect(ui.txARehabFile, &QLineEdit::textChanged, this, &ARehabMainWindow::slot_validateFileURL);
		QObject::connect(ui.txDescription, &QPlainTextEdit::textChanged, this, &ARehabMainWindow::slot_validateDescription);
		QObject::connect(ui.btGroupRepeats, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonReleased), this, &ARehabMainWindow::slot_btGroupRepToggled);
		QObject::connect(this->jointSelectorWidget, &QJointSelectorWidget::sig_jointsStateChanged, this, &ARehabMainWindow::slot_validateJoints);

		QObject::connect(btNew, &QPushButton::clicked, this, &ARehabMainWindow::slot_NewFile);
		QObject::connect(btLoad, &QPushButton::clicked, this, &ARehabMainWindow::slot_LoadFile);
		QObject::connect(this->guistatewidget, &QGUIStateWidget::sig_changeState, this, &ARehabMainWindow::slot_guiStateSelected);
		QObject::connect(this->guistatewidget, &QGUIStateWidget::sig_Save, this, &ARehabMainWindow::slot_ARehabSaveFile);

		QObject::connect(this->kinectControlWidget, &QKinectControlWidget::sig_kinectOn, this, &ARehabMainWindow::slot_KinectOnOff);
		QObject::connect(this->kinectControlWidget, &QKinectControlWidget::sig_kinectRecord, this, &ARehabMainWindow::slot_KinectRecord);
		QObject::connect(this, &ARehabMainWindow::sig_kinectRecord, this, &ARehabMainWindow::slot_validateGuiState2);
		QObject::connect(this->kinectControlWidget, &QKinectControlWidget::sig_kinectMaximize, this, &ARehabMainWindow::slot_Maximize);

		QObject::connect(this->arehabfileControlWidget, &QARehabFileControlWidget::sig_PlayPause, this, &ARehabMainWindow::slot_ArehabFilePlayPause);
		QObject::connect(this->arehabfileControlWidget, &QARehabFileControlWidget::sig_Restart, this, &ARehabMainWindow::slot_ArehabFileStop);
		QObject::connect(this->arehabfileControlWidget, &QARehabFileControlWidget::sig_Maximize, this, &ARehabMainWindow::slot_Maximize);

		QObject::connect(this->arehabfileControlWidget, &QARehabFileControlWidget::sig_LeftChanged, this, &ARehabMainWindow::slot_ArehabFileCuttingLeftMoving);
		QObject::connect(this->arehabfileControlWidget, &QARehabFileControlWidget::sig_RightChanged, this, &ARehabMainWindow::slot_ArehabFileCuttingRightMoving);


		/* GLViewports and related */
		QObject::connect(this, &ARehabMainWindow::newARehabKinectFrame, this->glwidget, &QGLViewer::slot_newARehabFrame);
		QObject::connect(this, &ARehabMainWindow::newARehabFileFrame, this->glwidget2, &QGLViewer::slot_newARehabFrame);

		QObject::connect(this->jointSelectorWidget, &QJointSelectorWidget::sig_jointsStateChanged, this->glwidget, &QGLViewer::slot_updateTrackedJoints);
		QObject::connect(this->jointSelectorWidget, &QJointSelectorWidget::sig_jointsStateChanged, this->glwidget2, &QGLViewer::slot_updateTrackedJoints);

		QObject::connect(this->timerFrames, &QTimer::timeout, this, &ARehabMainWindow::slot_update);
		QObject::connect(this->timerFrames2, &QTimer::timeout, this, &ARehabMainWindow::slot_update);

	}

	void ARehabMainWindow::doLayout(void)
	{
		ui.graphicsInicial->update();
	}

	void ARehabMainWindow::slot_KinectOnOff(bool on)
	{
		if (on) {
			this->kinectController->Initialize();
			if (this->timerFrames)
			{
				this->glwidget->slot_startAnimation();
				this->state = KINECT_PLAY;
				if (!this->timerFrames->isActive())
					this->timerFrames->start(33);
			}
		}
		else {
			if (this->timerFrames)
			{
				this->glwidget->slot_pauseAnimation();
				this->state = KINECT_INITIAL;
				this->timerFrames->stop();
				this->glwidget->update();
			}
			this->kinectController->Stop();
		}
	}

	void ARehabMainWindow::slot_KinectRecord(bool on)
	{
		if (on)
		{
			this->fileWriterController->OpenOutputFile(this->arehabFileMetadata.arehabFileURL.toStdString());
			this->state = KINECT_RECORDING;
		}
		else {
			this->state = KINECT_INITIAL;
			this->fileWriterController->CloseOutputFile();
		}
	}

	void ARehabMainWindow::slot_Maximize(bool on)
	{
		if (on){
			ui.topFrame->hide();
			ui.bottomFrame->hide();
			ui.mainGridLayout->setRowStretch(0, 0);
			ui.mainGridLayout->setRowStretch(1, 1);
			ui.mainGridLayout->setRowStretch(2, 0);			
		}
		else{
			ui.topFrame->show();
			ui.bottomFrame->show();
			ui.mainGridLayout->setRowStretch(0, 1);
			ui.mainGridLayout->setRowStretch(1, 6);
			ui.mainGridLayout->setRowStretch(2, 1);			
		}
		maximized = on;
		this->update();
	}

	void ARehabMainWindow::slot_ArehabFilePlayPause(bool on)
	{
		if (this->timerFrames2)
		{
			if (on) {
			
				this->state = CUTTING_PLAYING;
				this->glwidget2->slot_startAnimation();
				QTimer::singleShot(0, this, &ARehabMainWindow::slot_update);
			}
			else {			
					this->state = CUTTING_INITIAL;
					if (this->timerFrames2->isActive())
						this->timerFrames2->stop();
					this->fileReaderController->ResetCurrentFrame(arehabfileControlWidget->GetLeftFrameIndex());
			}
		}
	}

	void ARehabMainWindow::slot_ArehabFileStop(void)
	{
		this->arehabfileControlWidget->resetPlayButton();
		this->fileReaderController->ResetCurrentFrame(arehabfileControlWidget->GetLeftFrameIndex());
		this->state = CUTTING_INITIAL;
		this->timerFrames2->stop();
		QTimer::singleShot(5, this, &ARehabMainWindow::slot_update);
	}

	void ARehabMainWindow::slot_ArehabFileCuttingLeftMoving(unsigned long long int frameid)
	{
		this->arehabfileControlWidget->resetPlayButton();		
		this->state = CUTTING_MOVLEFT;
		QTimer::singleShot(5, this, &ARehabMainWindow::slot_update);
	}

	void ARehabMainWindow::slot_ArehabFileCuttingRightMoving(unsigned long long int frameid)
	{
		this->arehabfileControlWidget->resetPlayButton();

		this->state = CUTTING_MOVRIGHT;
		QTimer::singleShot(5, this, &ARehabMainWindow::slot_update);
	}

	void ARehabMainWindow::slot_ARehabSaveFile(void)
	{
		QMessageBox msgBox;
		msgBox.setText(QLatin1String("The file will be cut and this operation is irreversible..."));
		msgBox.setInformativeText(QLatin1String("Are you sure?"));
		msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		int ret = msgBox.exec();
		unsigned long long int left = 0, right = 0;
		switch (ret)
		{
			case QMessageBox::Save:	left = arehabfileControlWidget->GetLeftFrameIndex();
									right = arehabfileControlWidget->GetRightFrameIndex();
									if (this->fileReaderController->CutIntervalToFile(left, right))
									{
										QMessageBox msgBoxCutOk;
										msgBoxCutOk.setText(QLatin1String("File cut"));
										msgBoxCutOk.setInformativeText(QLatin1String("The ARehab file has been cut successfuly."));
										msgBoxCutOk.setStandardButtons(QMessageBox::Ok);
										msgBoxCutOk.setDefaultButton(QMessageBox::Ok);
										msgBoxCutOk.exec();

										this->resetAll();
										this->showGUIInitial();
									}
									break;
			case QMessageBox::Cancel: break;
		}
	}

	void ARehabMainWindow::slot_actionRutaFichero(void)
	{
		if (!dirFiles.exists())
		{
			dirFiles.mkdir("./ARehabFiles");
		}
		if (fileDialog)
		{
			fileDialog->setFileMode(QFileDialog::AnyFile);
			fileDialog->setDefaultSuffix(".arehab");
			if (fileDialog->exec())
			{
				QString arehabFileMetadataURL = fileDialog->selectedFiles().first();
				if (arehabFileMetadataURL != "")
				{
					QFile metadataFile(arehabFileMetadataURL);
					if (!metadataFile.open(QIODevice::WriteOnly)) {
						qWarning() << "Error opening file to save." << endl;
						return;
					}
					this->arehabFileMetadata.arehabFileURL = QUrl::fromLocalFile(arehabFileMetadataURL).toLocalFile();
					ui.txARehabFile->setText(QUrl::fromLocalFile(arehabFileMetadataURL).toLocalFile());

					this->validateARehabFileURL();
				}
			}
		}
	}

	void ARehabMainWindow::slot_validateTXName(void)
	{
		this->validateExerciseName();
	}

	void ARehabMainWindow::slot_validateFileURL(void)
	{
		this->validateARehabFileURL();
	}

	void ARehabMainWindow::slot_validateDescription(void)
	{
		this->validateDescription();
	}

	void ARehabMainWindow::slot_validateJoints(void)
	{
		this->validateJointSelection();
	}

	void ARehabMainWindow::slot_validateNumExercises(void)
	{
		this->validateNumExercises();
	}

	void ARehabMainWindow::slot_validateGuiState2(void)
	{
		this->validateGuiState2();
	}

	void ARehabMainWindow::slot_cleanAll(void)
	{
		ui.txARehabFile->setText("");
		ui.txDescription->setPlainText("");
		ui.txNombreEjercicio->setText("");		
		ui.lbValidationDescription->clear();
		ui.lbValidationFile->clear();
		ui.lbValidationName->clear();
		ui.lbValidationJoints->clear();
	}

	void ARehabMainWindow::slot_guiStateSelected(unsigned int state0, unsigned int state1)
	{
		bool validated = false;
		switch (state0)
		{
			case QGUIStateWidget::Initial:
				break;

			case QGUIStateWidget::Definition:
				validated = this->validateGuiState1();
				break;

			case QGUIStateWidget::Preview:
				this->resetGuiState2();
				validated = this->validateGuiState2();
				break;

			case QGUIStateWidget::Cutting:
				this->arehabfileControlWidget->resetPlayButton();
				validated = this->validateGuiState3();
				break;
		}

		switch (state1)
		{
			case QGUIStateWidget::Initial:
				this->showGUIInitial();
				break;

			case QGUIStateWidget::Definition:
				this->showGUIState1();
				break;
			case QGUIStateWidget::Recording:
				this->showGUIState2();
				break;
			case QGUIStateWidget::Cutting:
				this->showGUIState3();
				break;
			default: break;
		}
		ui.stackedWidget->setCurrentIndex(state1);
	}

	void ARehabMainWindow::slot_btGroupRepToggled(int id)
	{
		this->validateNumExercises();
	}

	void ARehabMainWindow::showEvent(QShowEvent * event)
	{
		this->doLayout();		
	}

	void ARehabMainWindow::closeEvent(QCloseEvent * event)
	{
		event->ignore();

		if (this->glwidget)
			this->glwidget->enableOpenGLDebuging(false);

		if (this->glwidget2)
			this->glwidget2->enableOpenGLDebuging(false);

		event->accept();
	}

	void ARehabMainWindow::keyPressEvent(QKeyEvent * event)
	{
		switch (event->key())
		{
			case Qt::Key_F5:
				if (maximized)
				{				
					if (this->state >= KINECT_INITIAL && state <= KINECT_RECORDING )
						this->kinectControlWidget->resetMaximizeButton();
					else if (this->state >= CUTTING_INITIAL && state <= CUTTING_PLAYING)												
						this->arehabfileControlWidget->resetMaximizeButton();					
				}
				break;
		}
	}

	ARehabMainWindow::~ARehabMainWindow(void)
	{
		if (this->fileDialog)
		{
			this->fileDialog->deleteLater();
		}
		if (this->timerFrames)
		{
			this->timerFrames->deleteLater();
		}
		if (this->timerFrames2)
		{
			this->timerFrames2->deleteLater();
		}
		if (this->glwidget)
		{
			this->glwidget->deleteLater();
		}
		if (this->glwidget2)
		{
			this->glwidget2->deleteLater();
		}
		if (this->fileWriterController)
		{
			delete this->fileWriterController;
			this->fileWriterController = 0;
		}
		if (this->fileReaderController)
		{
			delete this->fileReaderController;
			this->fileReaderController = 0;
		}

		this->kinectController->Stop();
	}

	void ARehabMainWindow::loadGuiInitial(void)
	{
		float wGraphicsView = ui.graphicsInicial->width();
		float hGraphicsView = ui.graphicsInicial->height();

		ui.btHelp->setIcon(QPixmap(QString::fromUtf8(":/svg/help.svg")));
		ui.btAbout->setIcon(QPixmap(QString::fromUtf8(":/svg/about.svg")));

		ui.graphicsInicial->verticalScrollBar()->blockSignals(true);
		ui.graphicsInicial->horizontalScrollBar()->blockSignals(true);
		ui.graphicsInicial->setScene(new QGraphicsScene);
		QGraphicsScene * scene = ui.graphicsInicial->scene();
		scene->setSceneRect(0, 0, ui.graphicsInicial->width(), ui.graphicsInicial->height());

		QRadialGradient gradient(wGraphicsView / 2.0f, hGraphicsView / 2.0f, hGraphicsView);
		gradient.setColorAt(0.95, QColor(200, 200, 200));
		gradient.setColorAt(0.5, QColor(255, 255, 255));
		gradient.setColorAt(0, QColor(255, 255, 255));
		scene->setBackgroundBrush(QBrush(gradient));

		QPixmap pix1(":/images/banner.png");
		this->pixmapItemInitial = scene->addPixmap(pix1);

		float wImage = this->pixmapItemInitial->boundingRect().width();
		float hImage = this->pixmapItemInitial->boundingRect().height();
		float scaleH = hGraphicsView / hImage;

		pixmapItemInitial->setTransformationMode(Qt::SmoothTransformation);
		pixmapItemInitial->setScale(scaleH);
		pixmapItemInitial->setPos((wGraphicsView / 2.0f) - (scaleH*wImage / 2.0f), 0);

		QFont btFont("Calibri", 16, QFont::Light);

		btNew = new QPushButton("Create Exersise");
		btNew->setMinimumSize(360, 60);
		btNew->setFont(btFont);
		proxyBtNuevo = scene->addWidget(btNew);

		btLoad = new QPushButton("Load Exersise");
		btLoad->setMinimumSize(360, 60);
		btLoad->setFont(btFont);
		proxyBtLoad = scene->addWidget(btLoad);

		QGridLayout * layoutBottomFrame = reinterpret_cast<QGridLayout*>(ui.bottomFrame->layout());
		if (layoutBottomFrame)
		{
			layoutBottomFrame->addWidget(this->guistatewidget, 0, 1);
			this->guistatewidget->hide();
		}
	}

	void ARehabMainWindow::loadGuiState1(void)
	{
		ui.layout4JointWidget->addWidget(this->jointSelectorWidget, 2, 0);

		ui.btGroupRepeats->setId(ui.bt5rep, 0);
		ui.btGroupRepeats->setId(ui.bt10rep, 1);
		ui.btGroupRepeats->setId(ui.bt15rep, 2);
		ui.btGroupRepeats->setId(ui.bt20rep, 3);
		ui.btGroupRepeats->setId(ui.bt25rep, 4);
		ui.btGroupRepeats->setId(ui.bt30rep, 5);
		ui.btGroupRepeats->setId(ui.bt35rep, 6);
		ui.btGroupRepeats->setId(ui.bt40rep, 7);
	}

	void ARehabMainWindow::loadGuiState2(void)
	{
		ui.layoutRecording->addWidget(this->glwidget, 0);
		ui.layoutRecording->addWidget(this->kinectControlWidget, 1);
		ui.layoutRecording->setStretch(0, 12);
		ui.layoutRecording->setStretch(1, 1);
	}

	void ARehabMainWindow::loadGuiState3(void)
	{
		ui.layoutCutting->addWidget(this->glwidget2, 0);
		ui.layoutCutting->addWidget(this->arehabfileControlWidget, 1);
		ui.layoutCutting->setStretch(0, 12);
		ui.layoutCutting->setStretch(1, 1);
	}

	bool ARehabMainWindow::validateExerciseName(void)
	{
		bool validate = ui.txNombreEjercicio->text().length();
		if (!validate)
		{
			ui.lbValidationName->setPixmap(QPixmap(QString::fromUtf8(":/svg/bad.svg")));
		}
		else
		{
			this->arehabFileMetadata.exerciseName = ui.txNombreEjercicio->text();
			ui.lbValidationName->setPixmap(QPixmap(QString::fromUtf8(":/svg/checked.svg")));
		}
		return validate;
	}

	bool ARehabMainWindow::validateARehabFileURL(void)
	{
		bool validate = true;
		QString filePath = ui.txARehabFile->text();
		QFileInfo checkFile(filePath);
		if (!checkFile.exists() || !checkFile.isFile()) {
			validate = false;
			ui.lbValidationFile->setPixmap(QPixmap(QString::fromUtf8(":/svg/bad.svg")));
		}
		else
		{
			ui.lbValidationFile->setPixmap(QPixmap(QString::fromUtf8(":/svg/checked.svg")));
		}
		return validate;
	}

	bool ARehabMainWindow::validateDescription(void)
	{
		bool validate = true;
		QString text = ui.txDescription->toPlainText();
		if (!text.length()) {
			validate = false;
			ui.lbValidationDescription->setPixmap(QPixmap(QString::fromUtf8(":/svg/bad.svg")));
		}
		else
		{
			this->arehabFileMetadata.description = text;
			ui.lbValidationDescription->setPixmap(QPixmap(QString::fromUtf8(":/svg/checked.svg")));
		}
		return validate;
	}

	bool ARehabMainWindow::validateNumExercises(void)
	{
		bool validated = false;
		if (ui.btGroupRepeats->checkedId() >= 0)
		{
			validated = true;
			ui.lbValidationNumRep->setPixmap(QPixmap(QString::fromUtf8(":/svg/checked.svg")));		
		}
		else
		{			
			ui.lbValidationNumRep->setPixmap(QPixmap(QString::fromUtf8(":/svg/bad.svg")));			
		}
		return validated;
	}

	bool ARehabMainWindow::validateJointSelection(void)
	{
		bool validate = true, someBitActive = false;
		QBitArray bitArray = jointSelectorWidget->getJointSelectorModel();

		for (unsigned int i = 0; i < bitArray.size(); ++i)
		{
			someBitActive |= jointSelectorWidget->getJointSelectorModel()[i];
		}

		if (!someBitActive)
		{			
			ui.lbValidationJoints->setPixmap(QPixmap(QString::fromUtf8(":/svg/bad.svg")));
			validate = false;
		}
		else
		{
			this->arehabFileMetadata.jointsInvolved = this->jointSelectorWidget->getJointSelectorModel();
			ui.lbValidationJoints->setPixmap(QPixmap(QString::fromUtf8(":/svg/checked.svg")));
		}
		return validate;
	}

	bool ARehabMainWindow::validateGuiState1(void)
	{
		bool validate = (validateARehabFileURL() && validateExerciseName() && validateDescription() && validateJointSelection() && validateNumExercises());

		if (validate)
		{
			this->arehabFileMetadata.exerciseName = ui.txNombreEjercicio->text();
			this->arehabFileMetadata.metadataFileURL = ui.txARehabFile->text();
			this->arehabFileMetadata.arehabFileURL = ui.txARehabFile->text() + ".data";
			this->arehabFileMetadata.description = ui.txDescription->toPlainText();
			this->arehabFileMetadata.jointsInvolved = this->jointSelectorWidget->getJointSelectorModel();

			QPushButton * btChecked = reinterpret_cast<QPushButton *>(ui.btGroupRepeats->checkedButton());
			if (btChecked)
				this->arehabFileMetadata.numRepetitions = btChecked->text().toUInt();
			this->arehabFileMetadata.save(this->arehabFileMetadata.metadataFileURL);

			this->guistatewidget->setStateValidation(QGUIStateWidget::Definition, QSVGTristateItem::ItemState::Valid);
		}
		else{
			this->arehabFileMetadata.clear();
			this->guistatewidget->setStateValidation(QGUIStateWidget::Definition, QSVGTristateItem::ItemState::NotValid);
		}
		return validate;
	}

	bool ARehabMainWindow::validateGuiState2(void)
	{
		bool validate = true;

		if (!this->validateGuiState1())
			validate = false;

		if (validate)
		{
			QString arehabFileURL = this->arehabFileMetadata.arehabFileURL;

			QFile arehabFile(arehabFileURL);
			if (!arehabFile.exists() || arehabFile.size() <= 0)
			{
				validate = false;
			}
		}

		if (validate)
		{
			this->guistatewidget->setStateValidation(QGUIStateWidget::Recording, QSVGTristateItem::ItemState::Valid);
			this->fileReaderController->OpenImputFile(arehabFileMetadata.arehabFileURL.toStdString());			
			this->arehabfileControlWidget->setModel(QARehabFileControlModel(this->fileReaderController->minKeyFrame, this->fileReaderController->maxKeyFrame));
		}
		else{
			this->arehabfileControlWidget->setModel(QARehabFileControlModel(0, 1));
			this->guistatewidget->setStateValidation(QGUIStateWidget::Recording, QSVGTristateItem::ItemState::NotValid);
		}
		return validate;
	}

	bool ARehabMainWindow::validateGuiState3(void)
	{
		bool validate = true;

		if (!this->validateGuiState2())
			validate = false;

		if (validate)
		{
			this->guistatewidget->setStateValidation(QGUIStateWidget::Cutting, QSVGTristateItem::ItemState::Valid);
		}
		else{
			this->guistatewidget->setStateValidation(QGUIStateWidget::Cutting, QSVGTristateItem::ItemState::NotValid);
		}
		return validate;
	}

	void ARehabMainWindow::resetGuiState1(void)
	{
		ui.txARehabFile->clear();		
		ui.txNombreEjercicio->clear();
		ui.txDescription->clear();
		this->jointSelectorWidget->resetWidget();
				
		ui.lbValidationFile->clear();
		ui.lbValidationName->clear();		
		ui.lbValidationDescription->clear();
		ui.lbValidationJoints->clear();
	}

	void ARehabMainWindow::resetGuiState2(void)
	{
		this->glwidget->update();
		this->kinectControlWidget->resetWidget();
		this->state = KINECT_INITIAL;
		QTimer::singleShot(5, this, &ARehabMainWindow::slot_update);
	}

	void ARehabMainWindow::resetGuiState3(void)
	{
		if (this->timerFrames2)
		{
			this->glwidget2->slot_pauseAnimation();
			this->timerFrames2->stop();
			this->glwidget2->update();			
		}
		this->arehabfileControlWidget->resetWidget();
	}

	void ARehabMainWindow::resetAll(void)
	{
		this->timerFrames->stop();
		this->timerFrames2->stop();

		this->state = INITIAL;

		this->kinectController->Stop();
		this->fileReaderController->CloseImputFile();
		this->fileWriterController->CloseOutputFile();

		this->resetGuiState1();
		this->resetGuiState2();
		this->resetGuiState3();
		this->guistatewidget->resetWidget();
	}

	void ARehabMainWindow::showGUIInitial(void)
	{
		this->guistatewidget->currentState = INITIAL;
		this->resetAll();
		this->guistatewidget->hide();
		ui.stackedWidget->setCurrentIndex(QGUIStateWidget::Initial);
	}

	void  ARehabMainWindow::slot_NewFile(void)
	{
		this->arehabFileMetadata.clear();
		this->resetAll();
		this->showGUIState1();
	}

	void ARehabMainWindow::slot_LoadFile(void)
	{
		if (!dirFiles.exists())
		{
			dirFiles.mkdir("./ARehabFiles");
		}
		this->arehabFileMetadata.clear();
		if (fileDialog)
		{
			fileDialog->setFileMode(QFileDialog::ExistingFile);
			fileDialog->setDefaultSuffix(".arehab");
			if (fileDialog->exec())
			{
				QString arehabFileURL = fileDialog->selectedFiles().first();
				if (arehabFileURL != "")
				{
					this->resetGuiState1();
					this->resetGuiState2();
					this->resetGuiState3();

					this->arehabFileMetadata.arehabFileURL = QUrl::fromLocalFile(arehabFileURL).toLocalFile();
					this->arehabFileMetadata.load(this->arehabFileMetadata.arehabFileURL);

					ui.txARehabFile->setText(this->arehabFileMetadata.metadataFileURL);
					ui.txNombreEjercicio->setText(this->arehabFileMetadata.exerciseName);
					ui.txDescription->setPlainText(this->arehabFileMetadata.description);
					this->jointSelectorWidget->setJointSelectorModel(this->arehabFileMetadata.jointsInvolved);
					if (arehabFileMetadata.numRepetitions > 0)
					{
						unsigned int buttonID = (arehabFileMetadata.numRepetitions / 5) - 1;
						QPushButton * bt = reinterpret_cast<QPushButton*>(ui.btGroupRepeats->button(buttonID));
						if (bt) {
							bt->setChecked(true);
						}
					}
					this->validateGuiState1();
					this->showGUIState1();
				}
			}
		}
	}

	void ARehabMainWindow::showGUIState1(void)
	{
		this->guistatewidget->currentState = 1;
		this->guistatewidget->show();
		this->guistatewidget->showNextButton(true);
		ui.stackedWidget->setCurrentIndex(this->guistatewidget->currentState);
	}

	void ARehabMainWindow::showGUIState2(void)
	{
		this->guistatewidget->showNextButton(true);
	}

	void ARehabMainWindow::showGUIState3(void)
	{
		bool validated = this->validateGuiState2();
		if (!validated)
		{
			this->resetGuiState3();
			this->arehabfileControlWidget->SetEditable(false);
		}
		else
		{
			this->arehabfileControlWidget->SetEditable(true);
		}
		this->timerFrames->stop();
		this->fileWriterController->CloseOutputFile();
		this->kinectController->Stop();
		this->arehabfileControlWidget->setEnabled(validated);
		this->guistatewidget->setButtonSaveEnabled(validated);		
		this->guistatewidget->showNextButton(false);
		this->arehabfileControlWidget->doLayout();
	}

};