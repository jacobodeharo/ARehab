#include "qkinectcontrolwidget.h"

namespace ARehabUI
{

	QKinectControlWidget::QKinectControlWidget(QWidget * parent) :
		QGraphicsView(new QGraphicsScene(), parent),
		containerRect(0),
		itemHovered(0),
		svgKinectOnOff(0),
		svgKinectRecord(0),
		svgKinectMaximize(0),
		itemBtContainer(0)
	{
		this->init();
	}

	QKinectControlWidget::~QKinectControlWidget(void)
	{
	}

	void QKinectControlWidget::loadSVGFiles(void)
	{
		QGraphicsScene * scene = this->scene();
		scene->setSceneRect(0, 0, this->width(), this->height());
		scene->setBackgroundBrush(QBrush(QColor(100, 100, 100, 255)));

		containerRect = new QGraphicsRectItem(0, 0, this->width(), this->height());
		containerRect->setPen(Qt::NoPen);
		containerRect->setBrush(QBrush(QColor(200, 200, 200, 50), Qt::CrossPattern));

		QPolygonF polygon;
		polygon.append(QPointF(80, 0));
		polygon.append(QPointF(0, 80));
		polygon.append(QPointF(400, 80));
		polygon.append(QPointF(320, 0));
		itemBtContainer = new QGraphicsPolygonItem(polygon, containerRect);
		itemBtContainer->setPen(Qt::NoPen);
		itemBtContainer->setBrush(QBrush(QColor(200, 200, 200, 100)));
		svgKinectOnOff = new QSVGInteractiveItem(":/svg/power.svg", itemBtContainer);
		svgKinectOnOff->setCheckable(true);
		svgKinectRecord = new QSVGInteractiveItem(":/svg/record.svg", itemBtContainer);

		svgKinectMaximize = new QSVGInteractiveItem(":/svg/maximize.svg", itemBtContainer);
		svgKinectMaximize->setCheckable(true);

		svgKinectRecord->setCheckable(true);
		svgKinectRecord->setEnabled(false);

		scene->addItem(containerRect);
	}

	void QKinectControlWidget::doLayout(void)
	{
		QGraphicsItemToolkit::moveTopLeft(svgKinectOnOff, 90);
		QGraphicsItemToolkit::centerItemVertically(svgKinectOnOff);

		QGraphicsItemToolkit::stackRight(svgKinectRecord, svgKinectOnOff, 50);
		QGraphicsItemToolkit::centerItemVertically(svgKinectRecord);

		QGraphicsItemToolkit::stackRight(svgKinectMaximize, svgKinectRecord, 50);
		QGraphicsItemToolkit::centerItemVertically(svgKinectMaximize);

		itemBtContainer->setTransformOriginPoint(itemBtContainer->boundingRect().center().x(), itemBtContainer->boundingRect().height());
		itemBtContainer->setScale(0.7);
		QGraphicsItemToolkit::centerItemHorizontally(itemBtContainer);
		QGraphicsItemToolkit::moveToBottom(itemBtContainer);

		QGraphicsItemToolkit::centerItem(containerRect);
	}

	void QKinectControlWidget::resetWidget(void)
	{
		this->svgKinectOnOff->reset();
		this->svgKinectRecord->reset(false); //Only the Kinect Record button depends from Kinect On/Off state...
		this->svgKinectMaximize->reset();
	}

	void QKinectControlWidget::resetMaximizeButton(void)
	{
		emit sig_kinectMaximize(false);
		this->svgKinectMaximize->reset();
	}

	void QKinectControlWidget::slot_KinectOn(bool on)
	{
		this->svgKinectRecord->setEnabled(on);
		emit sig_kinectOn(on);
	}

	void QKinectControlWidget::slot_KinectRecord(bool on)
	{
		emit sig_kinectRecord(on);
	}

	void QKinectControlWidget::slot_Maximize(bool on)
	{
		emit sig_kinectMaximize(on);
	}

	void QKinectControlWidget::connectItems(void)
	{
		QObject::connect(this->svgKinectOnOff, &QSVGInteractiveItem::stateChanged, this, &QKinectControlWidget::slot_KinectOn);
		QObject::connect(this->svgKinectRecord, &QSVGInteractiveItem::stateChanged, this, &QKinectControlWidget::slot_KinectRecord);
		QObject::connect(this->svgKinectMaximize, &QSVGInteractiveItem::stateChanged, this, &QKinectControlWidget::slot_Maximize);
	}

	void QKinectControlWidget::init(void)
	{
		this->setMinimumWidth(1000);
		this->setMinimumHeight(130);
		this->setMouseTracking(true);
		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
		this->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
		this->setStyleSheet("border: none; background: transparent;");

		loadSVGFiles();
		connectItems();
		doLayout();
	}

	void QKinectControlWidget::resizeEvent(QResizeEvent * event)
	{
		Q_UNUSED(event);
		QGraphicsScene * scene = this->scene();
		scene->setSceneRect(0, 0, width(), height());
		containerRect->setRect(0, 0, this->width(), this->height());
		containerRect->setPos((this->width() / 2.0f) - (containerRect->boundingRect().width() / 2.0f), (this->height() / 2.0f) - (containerRect->boundingRect().height() / 2.0f));

		doLayout();
	}

};
