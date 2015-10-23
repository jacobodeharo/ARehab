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

#include "ARehab_vPaciente.h"

namespace ARehabGUIPlayer
{

	ARehabMainWindow::ARehabMainWindow(QWidget *parent)
		: QMainWindow(parent),
		state(PREVIEW_INITIAL),
		guistatewidget(new QGUIStateWidget()),
		jointSelectorWidget(new QJointSelectorWidget()),
		glWidgetPreview(new QGLViewer(this)),
		glWidgetPlayer(new QGLViewer(this)),
		fileReaderController(0),
//		fileWriterController(0),
		timerPlayer(0),
		timerPreview(0),
		playControlWidget(new QARehabFileControlWidget()),
		previewControlWidget(new QARehabFileControlWidget()),
		dirFiles("./ARehabFiles"),
		fileDialog(NULL),
		maximized(false)
	{
		this->fileDialog = new QFileDialog(this, tr("Seleccione la ruta del fichero"), dirFiles.absolutePath(), tr("ARehab Files (*.arehab)"));

		this->fileReaderController = new ARehabFileReaderController();
//		this->fileWriterController = new ARehabFileWriterController();
		this->timerPlayer = new QTimer(this);
		this->timerPreview = new QTimer(this);
		this->timerPlayer->setTimerType(Qt::PreciseTimer);
		this->timerPreview->setTimerType(Qt::PreciseTimer);
		ui.setupUi(this);
		this->initGUI();
		this->connectWidgets();
		this->showMaximized();
	}

	void ARehabMainWindow::slot_update(void)
	{
		ARehabFrame * frameFile = NULL;
		unsigned long long int leftFrame = 0, rightFrame = 0;
		switch (this->state)
		{
			case PREVIEW_INITIAL:
				this->glWidgetPlayer->slot_pauseAnimation();				
				//leftFrame = previewControlWidget->GetLeftFrameIndex();
				//rightFrame = previewControlWidget->GetRightFrameIndex();
				this->fileReaderController->ResetCurrentFrame(0);
				frameFile = fileReaderController->GetNextARehabFrame();
				if (frameFile && glWidgetPreview->isVisible()) 
				{					
					emit newARehabFileFrame(frameFile);
					this->glWidgetPreview->slot_startAnimation();
				}
				break;

			case PREVIEW_PLAY:
				this->glWidgetPlayer->slot_pauseAnimation();
				frameFile = fileReaderController->GetNextARehabFrame();
				if (frameFile && glWidgetPreview->isVisible())
				{
					unsigned long long int interval = qMax(fileReaderController->currentTimeSpan, fileReaderController->previousTimeSpan) - qMin(fileReaderController->currentTimeSpan, fileReaderController->previousTimeSpan);
					this->timerPreview->start(interval);
					emit newARehabFileFrame(frameFile);
				}
				this->glWidgetPreview->slot_startAnimation();
				break;
			
			case PLAYING_INITIAL:
				break;

			case PLAYING_EXERCISE:
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

		proxyBtLoad->setPos((wGraphicsView / 2.0f) - (btLoad->width() / 2.0f), 80 + (hGraphicsView / 2.0f));
	}

	void ARehabMainWindow::initGUI(void)
	{
		if (!dirFiles.exists())
		{
			dirFiles.mkdir("./ARehabFiles");
		}

		this->loadGuiInitial();
		this->loadGuiState1();
		this->loadGuiState2();
		this->loadGuiState3();
		ui.stackedWidget->setCurrentIndex(0);
	}

	void ARehabMainWindow::connectWidgets(void)
	{
		QObject::connect(btLoad, &QPushButton::clicked, this, &ARehabMainWindow::slot_LoadFile);
		QObject::connect(this->guistatewidget, &QGUIStateWidget::sig_changeState, this, &ARehabMainWindow::slot_guiStateSelected);

		QObject::connect(this->previewControlWidget, &QARehabFileControlWidget::sig_PlayPause, this, &ARehabMainWindow::slot_PreviewWidgetPlayPause);
		QObject::connect(this->previewControlWidget, &QARehabFileControlWidget::sig_Restart, this, &ARehabMainWindow::slot_PreviewWidgetStop);
		QObject::connect(this->previewControlWidget, &QARehabFileControlWidget::sig_Maximize, this, &ARehabMainWindow::slot_Maximize);

		QObject::connect(this->playControlWidget, &QARehabFileControlWidget::sig_PlayPause, this, &ARehabMainWindow::slot_PlayerWidgetPlayPause);
		QObject::connect(this->playControlWidget, &QARehabFileControlWidget::sig_Restart, this, &ARehabMainWindow::slot_PlayerWidgetStop);
		QObject::connect(this->playControlWidget, &QARehabFileControlWidget::sig_Maximize, this, &ARehabMainWindow::slot_Maximize);

		/* GLViewports and related */
		QObject::connect(this, &ARehabMainWindow::newARehabFileFrame, this->glWidgetPreview, &QGLViewer::slot_newARehabFrame);
		QObject::connect(this, &ARehabMainWindow::newARehabFileFrame, this->glWidgetPlayer, &QGLViewer::slot_newARehabFrame);

		QObject::connect(this->jointSelectorWidget, &QJointSelectorWidget::sig_jointsStateChanged, this->glWidgetPreview, &QGLViewer::slot_updateTrackedJoints);
		QObject::connect(this->jointSelectorWidget, &QJointSelectorWidget::sig_jointsStateChanged, this->glWidgetPlayer, &QGLViewer::slot_updateTrackedJoints);

		QObject::connect(this->timerPlayer, &QTimer::timeout, this, &ARehabMainWindow::slot_update);
		QObject::connect(this->timerPreview, &QTimer::timeout, this, &ARehabMainWindow::slot_update);
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

	void ARehabMainWindow::slot_PreviewWidgetPlayPause(bool on)
	{
		if (this->timerPreview)
		{
			if (on) {			
				this->state = PREVIEW_PLAY;
				this->glWidgetPreview->slot_startAnimation();
				QTimer::singleShot(0, this, &ARehabMainWindow::slot_update);
			}
			else {	
				this->state = PREVIEW_INITIAL;
				if (this->timerPreview->isActive())
					this->timerPreview->stop();
			}
		}
	}

	void ARehabMainWindow::slot_PreviewWidgetStop(void)
	{	
		this->previewControlWidget->resetPlayButton();
		this->fileReaderController->ResetCurrentFrame(0);
		this->state = PREVIEW_INITIAL;
		this->timerPreview->stop();
		QTimer::singleShot(5, this, &ARehabMainWindow::slot_update);
	}

	void ARehabMainWindow::slot_PlayerWidgetPlayPause(bool on)
	{
		qDebug() << "PLAY: " << on << endl;
	}

	void ARehabMainWindow::slot_PlayerWidgetStop(void)
	{
		qDebug() << "STOP" << endl;
	}

	void ARehabMainWindow::slot_cleanAll(void)
	{
		ui.txARehabFile->setText("");
		ui.txDescription->setPlainText("");
		ui.txNombreEjercicio->setText("");
	}

	void ARehabMainWindow::slot_guiStateSelected(unsigned int state0, unsigned int state1)
	{
		switch (state0)
		{
			case QGUIStateWidget::Initial:
				break;

			case QGUIStateWidget::Definition:
				break;

			case QGUIStateWidget::Preview:
				
				break;

			case QGUIStateWidget::Cutting:

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
			case QGUIStateWidget::Preview:
				this->previewControlWidget->resetPlayButton();
				this->showGUIState2();
				break;
			case QGUIStateWidget::Cutting:
				this->showGUIState3();
				break;
			default: break;
		}
		ui.stackedWidget->setCurrentIndex(state1);
	}

	void ARehabMainWindow::closeEvent(QCloseEvent * event)
	{
		event->ignore();

		if (this->glWidgetPreview)
			this->glWidgetPreview->enableOpenGLDebuging(false);

		if (this->glWidgetPlayer)
			this->glWidgetPlayer->enableOpenGLDebuging(false);

		event->accept();
	}

	void ARehabMainWindow::keyPressEvent(QKeyEvent * event)
	{
		switch (event->key())
		{
			case Qt::Key_F5:
				if (maximized)
				{				
					if (this->state >= PREVIEW_INITIAL && state <= PREVIEW_PLAY)
					{
						this->previewControlWidget->resetMaximizeButton();				
					}
					if (this->state >= PLAYING_INITIAL && state <= PLAYING_EXERCISE)
					{
						this->playControlWidget->resetMaximizeButton();
					}
				}
				break;
		}
	}

	void ARehabMainWindow::showEvent(QShowEvent * event)
	{

	}

	ARehabMainWindow::~ARehabMainWindow(void)
	{
		if (this->fileDialog)
		{
			this->fileDialog->deleteLater();
		}
		if (this->timerPlayer)
		{
			this->timerPlayer->deleteLater();
		}
		if (this->timerPreview)
		{
			this->timerPreview->deleteLater();
		}
		if (this->glWidgetPreview)
		{
			this->glWidgetPreview->deleteLater();
		}
		if (this->glWidgetPlayer)
		{
			this->glWidgetPlayer->deleteLater();
		}
		if (this->fileReaderController)
		{
			delete this->fileReaderController;
			this->fileReaderController = 0;
		}
	}

	void ARehabMainWindow::loadGuiInitial(void)
	{
		//Load main window icons
		ui.btHelp->setIcon(QPixmap(QString::fromUtf8(":/svg/help.svg")));
		ui.btAbout->setIcon(QPixmap(QString::fromUtf8(":/svg/about.svg")));

		this->guistatewidget->setStateName(0, QLatin1String("Definición"));
		this->guistatewidget->setStateName(1, QLatin1String("Previsualización"));
		this->guistatewidget->setStateName(2, QLatin1String("Realización"));

		float wGraphicsView = ui.graphicsInicial->width();
		float hGraphicsView = ui.graphicsInicial->height();

		ui.graphicsInicial->verticalScrollBar()->blockSignals(true);
		ui.graphicsInicial->horizontalScrollBar()->blockSignals(true);
		ui.graphicsInicial->setScene(new QGraphicsScene);
		QGraphicsScene * scene = ui.graphicsInicial->scene();

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

		btLoad = new QPushButton("Cargar Ejercicio");
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
		ui.layout4JointWidget->addWidget(this->jointSelectorWidget, 1, 0);
		this->jointSelectorWidget->setInteractive(false);
		
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
		ui.layoutRecording->addWidget(this->glWidgetPreview, 0);
		ui.layoutRecording->addWidget(this->previewControlWidget, 1);
		ui.layoutRecording->setStretch(0, 12);
		ui.layoutRecording->setStretch(1, 1);		
	}

	void ARehabMainWindow::loadGuiState3(void)
	{
		ui.layoutCutting->addWidget(this->glWidgetPlayer, 0);
		ui.layoutCutting->addWidget(this->playControlWidget, 1);
		ui.layoutCutting->setStretch(0, 12);
		ui.layoutCutting->setStretch(1, 1);
	}

	void ARehabMainWindow::resetGuiState1(void)
	{
		ui.txARehabFile->clear();		
		ui.txNombreEjercicio->clear();
		ui.txDescription->clear();
		this->jointSelectorWidget->resetWidget();
	}

	void ARehabMainWindow::resetGuiState2(void)
	{
		this->glWidgetPreview->update();
		//this->kinectControlWidget->resetWidget();
		this->state = PREVIEW_INITIAL;
		QTimer::singleShot(5, this, &ARehabMainWindow::slot_update);
	}

	void ARehabMainWindow::resetGuiState3(void)
	{
		if (this->timerPreview)
		{
			this->glWidgetPlayer->slot_pauseAnimation();
			this->timerPreview->stop();
			this->glWidgetPlayer->update();			
		}
		this->playControlWidget->resetWidget();
	}

	void ARehabMainWindow::resetAll(void)
	{
		this->timerPlayer->stop();
		this->timerPreview->stop();

		this->state = INITIAL;
		
		this->fileReaderController->CloseImputFile();
		//this->fileWriterController->CloseOutputFile();

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
						//ui.btGroupRepeats->buttonToggled(buttonID, true);
						for (unsigned int i = 0; i < ui.btGroupRepeats->buttons().size(); ++i)
						{
							QPushButton * bt = reinterpret_cast<QPushButton*>(ui.btGroupRepeats->button(i));
							if (bt)
							{
								if (i == buttonID){
									bt->show();
									bt->setChecked(true);
								}
								else {
									bt->hide();
								}
							}
						}
					}
					this->fileReaderController->OpenImputFile(arehabFileMetadata.arehabFileURL.toStdString());
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
		//this->previewControlWidget->SetEditable(false);
	}

	void ARehabMainWindow::showGUIState3(void)
	{
		this->resetGuiState3();		
		this->timerPlayer->stop();
//		this->fileWriterController->CloseOutputFile();

		this->playControlWidget->setEnabled(true);
		
		this->guistatewidget->showNextButton(false);
		this->playControlWidget->doLayout();
	}

};