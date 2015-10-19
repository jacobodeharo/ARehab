//#ifndef AREHABWORKER_H
//#define AREHABWORKER_H
//
//#include <qstring.h>
//#include <qobject.h>
//#include <qthread.h>
//
//#include "KinectController.h"
//#include "ARehabFrame.h"
//
//using namespace ARehabKinect;
//
//class ARehabReaderWorker : public QObject
//{
//	Q_OBJECT
//
//	public:
//		ARehabReaderWorker(QObject *parent);
//		virtual ~ARehabReaderWorker(void);
//
//	public slots:		
//		void read(void)
//		{
//			while (true)
//			{
//				ARehabFrame * frame = NULL;
//				//Read the frame...
//				emit frameReady(frame);
//			}
//		}
//
//	signals:
//		void frameReady(ARehabFrame *);
//
//	private:
//		IARehabFrameReader * frameReader;
//		bool exit;
//};
//
//class ARehabWriterWorker : public QObject
//{
//	Q_OBJECT
//
//	public:
//		ARehabWriterWorker(QObject *parent);
//		virtual ~ARehabWriterWorker(void);
//
//		public slots:
//		void write(const ARehabFrame * frame)
//		{
//			
//		}
//
//	signals:
//		void frameReady(ARehabFrame *);
//
//	private:
//		IARehabFrameWriter * frameWriter;
//		bool exit;
//};
//
//class ARehabController : public QObject
//{
//	Q_OBJECT
//	
//	private:
//		QThread workerKinectThread;
//		QThread workerFileReaderThread;
//		QThread workerFileWriterThread;
//
//	public:
//		ARehabController(void);
//		virtual ~ARehabController(void);
//
//		void connectItems(void);
//
//		void startWorkKinect(void);
//		void startWorkFileRead(QString fileURL);		
//		void startWorkFileWrite(QString fileURL);
//
//	public slots:
//		void slot_ResultReady();
//
//};
//
//#endif // AREHABWORKER_H
