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
//#include <QImage>

#include "ui_ARehab.h"
#include "QGLViewer.h"
#include "QGuiStateWidget.h"
#include "QJointSelectorWidget.h"
#include "QKinectControlWidget.h"
#include "QARehabFileControlWidget.h"
#include "FrameWriter.h"

namespace ARehabControl {
	class ColorFrame;
	class BodyFrame;
	class ARehabFrame;
	class KinectController;
	class ARehabFileWriterController;
	class ARehabFileReaderController;
	//class ARehabFileCutter;
};
using namespace ARehabControl;

namespace ARehabUI
{
	//class ARehabController : public QTimer
	//{
	//	public:
	//		ARehabController(QObject * parent = 0) :
	//			QTimer(parent),
	//			lastTime(0)
	//		{

	//		}

	//		~ARehabController(void)
	//		{
	//		}
	//					
	//		quint64 lastTime;
	//};

	class ARehabFileMetadata
	{
	public:
		QString exerciseName;
		QString metadataFileURL;
		QString arehabFileURL;
		QString description;
		QBitArray jointsInvolved;
		bool loaded;

		ARehabFileMetadata(void) :
			loaded(false),
			jointsInvolved(25, false)
		{
		}

		virtual ~ARehabFileMetadata(void)
		{
		}

		bool isLoaded(void)
		{
			return loaded;
		}

		bool load(const QString &jsonFileURL)
		{
			QFile metadataFile(jsonFileURL);
			if (!metadataFile.open(QIODevice::ReadOnly)) {
				qWarning() << "Error abriendo el fichero para guardar." << endl;
				return false;
			}
			QByteArray saveData = metadataFile.readAll();
			QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
			QJsonObject jsonObj = loadDoc.object();

			exerciseName = jsonObj.take("ExerciseName").toString();
			metadataFileURL = jsonObj["MetadataFileURL"].toString();
			arehabFileURL = jsonObj["ARehabFileURL"].toString();
			description = jsonObj["Descripcion"].toString();
			QJsonObject jointsInv = jsonObj["JointsInvolved"].toObject();
			for (unsigned int i = 0; i < jointsInvolved.size(); ++i)
			{
				jointsInvolved[i] = jointsInv[QString::number(i)].toBool();
			}
			loaded = true;
			return loaded;
		}

		bool save(const QString &jsonFileURL)
		{
			QFile metadataFile(jsonFileURL);
			if (!metadataFile.open(QIODevice::WriteOnly)) {
				qWarning() << "Error abriendo el fichero para guardar." << endl;
				return false;
			}
			QJsonObject arehabJsonObject;
			arehabJsonObject.insert("ExerciseName", exerciseName);
			arehabJsonObject.insert("MetadataFileURL", metadataFileURL);
			arehabJsonObject.insert("ARehabFileURL", arehabFileURL);
			arehabJsonObject.insert("Descripcion", description);

			QJsonObject jointsInv;
			for (unsigned int i = 0; i < jointsInvolved.size(); ++i)
			{
				jointsInv.insert(QString::number(i), QJsonValue(jointsInvolved[i]));
			}
			arehabJsonObject.insert("JointsInvolved", jointsInv);
			QJsonDocument jsonDocument(arehabJsonObject);
			metadataFile.write(jsonDocument.toJson());

			return true;
		}

		void clear(void)
		{
			this->arehabFileURL.clear();
			this->description.clear();
			this->exerciseName.clear();
			this->jointsInvolved = QBitArray(this->jointsInvolved.size(), false);
			this->metadataFileURL.clear();
			loaded = false;
		}
	};

	class ARehabMainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		enum { INITIAL, KINECT_INITIAL, KINECT_PLAY, KINECT_RECORDING, CUTTING_INITIAL, CUTTING_MOVLEFT, CUTTING_MOVRIGHT, CUTTING_PLAYING } AppStates;
		ARehabMainWindow(QApplication * app, QWidget *parent = 0);
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
		void slot_validateGuiState2(void);
		void slot_cleanAll(void);
		void slot_KinectOnOff(bool);
		void slot_KinectRecord(bool);
		void slot_Maximize(bool on);
		void slot_guiStateSelected(unsigned int state0, unsigned int state1);

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
		QApplication * app;
		
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

		void loadGuiInitial(void);
		void loadGuiState1(void);
		void loadGuiState2(void);
		void loadGuiState3(void);

		bool validateExerciseName(void);
		bool validateARehabFileURL(void);
		bool validateDescription(void);
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

