#ifndef QKINECTCONTROLWIDGET_H
#define QKINECTCONTROLWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QtSvg/QGraphicsSvgItem>
#include <QGraphicsColorizeEffect>
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>
#include <QLinearGradient>
#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>

#include "QGraphicsItemToolkit.h"
#include "QSVGInteractiveItem.h"

namespace ARehabUI
{

	class QKinectControlWidget : public QGraphicsView
	{
		Q_OBJECT

	public:
		explicit QKinectControlWidget(QWidget * parent = 0);
		virtual ~QKinectControlWidget(void);
		void doLayout(void);
		void resetWidget(void);
		void resetMaximizeButton(void);

		public slots:
		void slot_KinectOn(bool on);
		void slot_KinectRecord(bool on);
		void slot_Maximize(bool on);

	signals:
		void sig_kinectOn(bool on);
		void sig_kinectRecord(bool on);
		void sig_kinectMaximize(bool on);

	protected:
		virtual void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;

	private:
		QGraphicsRectItem * containerRect;
		QGraphicsItem * itemHovered;
		QSVGInteractiveItem * svgKinectOnOff;
		QSVGInteractiveItem * svgKinectRecord;
		QSVGInteractiveItem * svgKinectMaximize;
		QGraphicsPolygonItem * itemBtContainer;

		bool onOff;
		bool recording;

		void connectItems(void);
		void init(void);
		void loadSVGFiles(void);
	};

};
#endif // QKINECTCONTROLWIDGET_H
