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

#ifndef AREHAB_H
#define AREHAB_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QSurfaceFormat>
#include <QGridLayout>
#include <QMessageBox>
#include <QButtonGroup>
#include <QAction>
#include <QTimer>
#include <QShowEvent>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QActionGroup>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QScrollBar>
#include <QGraphicsProxyWidget>
#include <QRadialGradient>
#include <QDialogButtonBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
//#include <QImage>

#include "ui_ARehab_vTerapeuta.h"

#include "KinectController.h"
#include "ARehabFileReaderController.h"
#include "ARehabFileWriterController.h"

#include "ARehabFileMetadata.h"
#include "QGLViewer.h"
#include "QGuiStateWidget.h"
#include "QJointSelectorWidget.h"
#include "QKinectControlWidget.h"
#include "QARehabFileControlWidget.h"

using namespace ARehabControl;
using namespace ARehabUI;

namespace ARehabGUIDesigner
{

	class ARehabMainWindow : public QMainWindow
	{
		Q_OBJECT

		public:
			enum { INITIAL, KINECT_INITIAL, KINECT_PLAY, KINECT_RECORDING, CUTTING_INITIAL, CUTTING_MOVLEFT, CUTTING_MOVRIGHT, CUTTING_PLAYING } AppStates;
			ARehabMainWindow(QWidget *parent = 0);
			virtual void showEvent(QShowEvent * event) Q_DECL_OVERRIDE;
			virtual void closeEvent(QCloseEvent * event) Q_DECL_OVERRIDE;
			virtual void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
			virtual ~ARehabMainWindow(void);

		public slots:
			void slot_actionRutaFichero(void);
			void slot_validateTXName(void);
			void slot_validateFileURL(void);
			void slot_validateDescription(void);
			void slot_validateJoints(void);
			void slot_validateNumExercises(void);
			void slot_validateGuiState2(void);
			void slot_cleanAll(void);
			void slot_KinectOnOff(bool);
			void slot_KinectRecord(bool);
			void slot_Maximize(bool on);
			void slot_guiStateSelected(unsigned int state0, unsigned int state1);
			void slot_btGroupRepToggled(int id);

			void slot_ArehabFileStop(void);
			void slot_ArehabFilePlayPause(bool);
			void slot_ArehabFileCuttingLeftMoving(unsigned long long int);
			void slot_ArehabFileCuttingRightMoving(unsigned long long int);
			void slot_ARehabSaveFile(void);

			void showGUIInitial(void);
			void showGUIState1(void);
			void showGUIState2(void);
			void showGUIState3(void);

			void slot_NewFile(void);
			void slot_LoadFile(void);
			void slot_update(void);

		protected:
			virtual void resizeEvent(QResizeEvent * event);

		signals:
			void newBodyFrame(BodyFrame *);
			void newColorFrame(ColorFrame *);
			void newARehabKinectFrame(ARehabFrame *);
			void newARehabFileFrame(ARehabFrame *);

			void sig_kinectOnOff(bool);
			void sig_kinectRecord(bool);
			void sig_kinectRecordFinished(void);

		
		private:
			ARehabFileMetadata arehabFileMetadata;
			QString arehabFileURL;
			unsigned int state; // Application state
			Ui::ARehabClass ui;
		
			QTimer * timerFrames; //Timer for retrieve kinect frames to glwidget
			QTimer * timerFrames2; //Timer for retrieve ARehabfile frames to glwidget2
			unsigned long long int timeOut; //last TimeOut for sync

			KinectController * kinectController;
			ARehabFileReaderController * fileReaderController;
			ARehabFileWriterController * fileWriterController;
			QGLViewer * glwidget;
			QGLViewer * glwidget2;

			/* Initial stacked widget, Main menu in the initial window */
			QGraphicsPixmapItem * pixmapItemInitial;
			QPushButton * btNew;
			QGraphicsProxyWidget * proxyBtNuevo;
			QPushButton * btLoad;
			QGraphicsProxyWidget * proxyBtLoad;
			QPushButton * btLoadResults;
			QGraphicsProxyWidget * proxyBtLoadResults;

			/* Custom Widgets */
			QGUIStateWidget * guistatewidget;
			QJointSelectorWidget * jointSelectorWidget;
			QKinectControlWidget * kinectControlWidget;
			QARehabFileControlWidget * arehabfileControlWidget;

			/* QFile dialog */
			QDir dirFiles;
			QFileDialog * fileDialog;

			bool maximized;

			void initGUI(void);
			void connectWidgets(void);
			void doLayout(void);

			void loadGuiInitial(void);
			void loadGuiState1(void);
			void loadGuiState2(void);
			void loadGuiState3(void);

			bool validateExerciseName(void);
			bool validateARehabFileURL(void);
			bool validateDescription(void);
			bool validateNumExercises(void);
			bool validateJointSelection(void);

			bool validateGuiState1(void);
			bool validateGuiState2(void);
			bool validateGuiState3(void);

			void resetGuiState1(void);
			void resetGuiState2(void);
			void resetGuiState3(void);
			void resetAll(void);

	};

};

#endif // AREHAB_H

