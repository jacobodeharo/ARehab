#ifndef AREHAB_H
#define AREHAB_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QSurfaceFormat>
#include <QGridLayout>
#include <QMessageBox>
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
#include <QBitArray>
#include <QDebug>
//#include <QImage>

#include "ui_ARehab_vPaciente.h"

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

namespace ARehabGUIPlayer
{
	class ARehabMainWindow : public QMainWindow
	{
		Q_OBJECT

		public:
			enum { INITIAL, PREVIEW_INITIAL, PREVIEW_PLAY, PLAYING_INITIAL, PLAYING_EXERCISE } AppStates;
			ARehabMainWindow(QWidget *parent = 0);
			virtual void showEvent(QShowEvent * event) Q_DECL_OVERRIDE;
			virtual void closeEvent(QCloseEvent * event) Q_DECL_OVERRIDE;
			virtual void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
			virtual ~ARehabMainWindow(void);

		public slots:
			void slot_cleanAll(void);
			void slot_Maximize(bool on);
			void slot_guiStateSelected(unsigned int state0, unsigned int state1);

			void slot_PreviewWidgetPlayPause(bool);
			void slot_PreviewWidgetStop(void);

			void slot_PlayerWidgetPlayPause(bool);
			void slot_PlayerWidgetStop(void);
			
			void showGUIInitial(void);
			void showGUIState1(void);
			void showGUIState2(void);
			void showGUIState3(void);

			void slot_LoadFile(void);
			void slot_update(void);

		protected:
			virtual void resizeEvent(QResizeEvent * event);

		signals:
			void newBodyFrame(BodyFrame *);
			void newColorFrame(ColorFrame *);			
			void newARehabFileFrame(ARehabFrame *);
	
		private:
			ARehabFileMetadata arehabFileMetadata;
			QString arehabFileURL;
			unsigned int state; // Application state
			Ui::ARehabClass ui;
		
			QTimer * timerPreview;
			QTimer * timerPlayer;
								
			ARehabFileReaderController * fileReaderController;
			QGLViewer * glWidgetPreview;
			QGLViewer * glWidgetPlayer;

			/* Initial stacked widget, Main menu in the initial window */
			QGraphicsPixmapItem * pixmapItemInitial;
			QPushButton * btLoad;
			QGraphicsProxyWidget * proxyBtLoad;

			/* Custom Widgets */
			QGUIStateWidget * guistatewidget;
			QJointSelectorWidget * jointSelectorWidget;
			QARehabFileControlWidget * previewControlWidget;
			QARehabFileControlWidget * playControlWidget;						

			/* QFile dialog */
			QDir dirFiles;
			QFileDialog * fileDialog;

			bool maximized;

			void initGUI(void);
			void connectWidgets(void);

			void loadGuiInitial(void);
			void loadGuiState1(void);
			void loadGuiState2(void);
			void loadGuiState3(void);

			void resetGuiState1(void);
			void resetGuiState2(void);
			void resetGuiState3(void);
			void resetAll(void);

		};

};

#endif // AREHAB_H

