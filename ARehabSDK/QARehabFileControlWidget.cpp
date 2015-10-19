#include "qarehabfilecontrolwidget.h"

namespace ARehabUI
{



	int QARehabFileControlWidget::leftPadding = 100;
	int QARehabFileControlWidget::rightPadding = 100;

	QARehabFileControlWidget::QARehabFileControlWidget(QWidget * parent) :
		QGraphicsView(new QGraphicsScene(), parent),
		model(0, 1000),
		containerRect(0),
		itemHovered(0),
		svgRestart(0),
		svgPlayPause(0),
		svgKinectMaximize(0),
		svgLeftSelector(0),
		svgRightSelector(0),
		itemTimeline(0),
		itemCuttingInterval(0),
		itemBtContainer(0),
		selectorMoving(0),
		intervalEditable(false),
		minPos(0),
		maxPos(0),
		lastFrame(LLONG_MAX),
		itemTXModelLeftValue(0),
		itemTXModelRightValue(0),
		clickOffset(0)
	{
		this->init();
	}

	QARehabFileControlWidget::~QARehabFileControlWidget(void)
	{
	}

	void QARehabFileControlWidget::loadSVGFiles(void)
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

		svgRestart = new QSVGInteractiveItem(":/svg/restart.svg", itemBtContainer);

		svgPlayPause = new QSVGInteractiveItem(":/svg/play.svg", itemBtContainer);
		svgPlayPause->setCheckable(true);

		svgKinectMaximize = new QSVGInteractiveItem(":/svg/maximize.svg", itemBtContainer);
		svgKinectMaximize->setCheckable(true);

		itemTimeline = new QGraphicsRectItem(0, 0, this->width() - 160, 5, containerRect);
		itemTimeline->setPen(Qt::NoPen);
		itemTimeline->setBrush(QBrush(QColor(200, 200, 255, 200)));

		itemCuttingInterval = new QGraphicsRectItem(0, 0, this->width() - 160, 20, containerRect);
		itemCuttingInterval->setPen(Qt::NoPen);
		itemCuttingInterval->setBrush(QBrush(QColor(100, 200, 0, 100)));

		svgLeftSelector = new QSVGInteractiveItem(":/svg/intervalLeft.svg", containerRect);
		svgRightSelector = new QSVGInteractiveItem(":/svg/intervalRight.svg", containerRect);

		QFont font("Verdana", 8);
		itemTXModelLeftValue = new QGraphicsSimpleTextItem("Left", this->svgLeftSelector);
		itemTXModelLeftValue->setFont(font);
		itemTXModelLeftValue->setPen(Qt::NoPen);
		itemTXModelLeftValue->setBrush(QBrush(QColor(200, 200, 200, 200)));
		itemTXModelRightValue = new QGraphicsSimpleTextItem("Right", this->svgRightSelector);
		itemTXModelRightValue->setFont(font);
		itemTXModelRightValue->setPen(Qt::NoPen);
		itemTXModelRightValue->setBrush(QBrush(QColor(200, 200, 200, 200)));
		itemTXModelRightValue->setText(QString::number(this->model.rightValue));
		itemTXModelLeftValue->setText(QString::number(this->model.leftValue));

		scene->addItem(containerRect);
	}

	void QARehabFileControlWidget::doLayout(void)
	{
		QGraphicsItemToolkit::moveTopLeft(svgRestart, 90);
		QGraphicsItemToolkit::centerItemVertically(svgRestart);

		QGraphicsItemToolkit::stackRight(svgPlayPause, svgRestart, 50);
		QGraphicsItemToolkit::centerItemVertically(svgPlayPause);

		QGraphicsItemToolkit::stackRight(svgKinectMaximize, svgPlayPause, 50);
		QGraphicsItemToolkit::centerItemVertically(svgKinectMaximize);

		itemBtContainer->setTransformOriginPoint(itemBtContainer->boundingRect().center().x(), itemBtContainer->boundingRect().height());
		itemBtContainer->setScale(0.7);
		QGraphicsItemToolkit::centerItemHorizontally(itemBtContainer);
		QGraphicsItemToolkit::moveToBottom(itemBtContainer);

		itemTimeline->setRect(0, 0, this->width() - QARehabFileControlWidget::leftPadding - QARehabFileControlWidget::rightPadding, 5);
		itemTimeline->setPos(0, 30);
		QGraphicsItemToolkit::centerItemHorizontally(itemTimeline);

		if (this->intervalEditable)
		{
			QGraphicsItemToolkit::stackRight(svgRightSelector, itemTimeline, -(itemTimeline->boundingRect().width() - mapModelPosToViewPos(this->model.rightValue)));
			QGraphicsItemToolkit::stackLeft(svgLeftSelector, itemTimeline, -mapModelPosToViewPos(this->model.leftValue));
			QGraphicsItemToolkit::centerItemVRelativeTo(svgRightSelector, itemTimeline);
			QGraphicsItemToolkit::centerItemVRelativeTo(svgLeftSelector, itemTimeline);

			float rectx = this->svgLeftSelector->pos().x() + this->svgLeftSelector->boundingRect().width();
			itemCuttingInterval->setRect(rectx, 0, this->svgRightSelector->pos().x() - rectx, 20);
			QGraphicsItemToolkit::centerItemVRelativeTo(itemCuttingInterval, itemTimeline);

			this->minPos = this->itemTimeline->pos().x();
			this->maxPos = this->minPos + this->itemTimeline->boundingRect().width();

			itemTXModelLeftValue->setPos(svgLeftSelector->boundingRect().width() - itemTXModelLeftValue->boundingRect().width(), svgLeftSelector->boundingRect().height());
			itemTXModelRightValue->setPos(svgRightSelector->boundingRect().x(), svgRightSelector->boundingRect().height());

			svgRightSelector->show();
			svgLeftSelector->show();
			itemCuttingInterval->show();
		}
		else{
			svgRightSelector->hide();
			svgLeftSelector->hide();
			itemCuttingInterval->hide();
		}
		QGraphicsItemToolkit::centerItem(containerRect);
	}

	void QARehabFileControlWidget::setModel(const QARehabFileControlModel &model)
	{
		this->model = model;

		itemTXModelRightValue->setText(QString::number(this->model.rightValue));
		itemTXModelLeftValue->setText(QString::number(this->model.leftValue));
	}

	void QARehabFileControlWidget::resetPlayButton(void)
	{
		emit sig_PlayPause(false);
		this->svgPlayPause->reset();
	}

	void QARehabFileControlWidget::resetWidget(void)
	{
		this->svgPlayPause->reset();
		this->svgKinectMaximize->reset();
		this->svgLeftSelector->reset();
		this->svgRightSelector->reset();
		this->update();
	}

	void QARehabFileControlWidget::resetMaximizeButton(void)
	{
		emit sig_Maximize(false);
		this->svgKinectMaximize->reset();
	}


	void QARehabFileControlWidget::slot_PlayPause(bool on)
	{
		emit sig_PlayPause(on);
	}

	void QARehabFileControlWidget::slot_Restart(void)
	{
		emit sig_Restart();
	}

	void QARehabFileControlWidget::slot_Maximize(bool on)
	{
		emit sig_Maximize(on);
	}

	void QARehabFileControlWidget::connectItems(void)
	{
		QObject::connect(this->svgRestart, &QSVGInteractiveItem::clicked, this, &QARehabFileControlWidget::slot_Restart);
		QObject::connect(this->svgPlayPause, &QSVGInteractiveItem::stateChanged, this, &QARehabFileControlWidget::slot_PlayPause);
		QObject::connect(this->svgKinectMaximize, &QSVGInteractiveItem::stateChanged, this, &QARehabFileControlWidget::slot_Maximize);
	}

	void QARehabFileControlWidget::init(void)
	{
		this->setMinimumWidth(1000);
		this->setMinimumHeight(130);
		this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
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

	void QARehabFileControlWidget::resizeEvent(QResizeEvent * event)
	{
		Q_UNUSED(event);
		QGraphicsScene * scene = this->scene();
		scene->setSceneRect(0, 0, width(), height());
		containerRect->setRect(0, 0, this->width(), this->height());
		containerRect->setPos((this->width() / 2.0f) - (containerRect->boundingRect().width() / 2.0f), (this->height() / 2.0f) - (containerRect->boundingRect().height() / 2.0f));

		doLayout();
	}

	void QARehabFileControlWidget::mousePressEvent(QMouseEvent * event)
	{
		QGraphicsView::mousePressEvent(event);
		QSVGInteractiveItem * selector = dynamic_cast<QSVGInteractiveItem *>(this->itemAt(event->pos()));
		if (selector)
		{
			if ((&(*selector) == this->svgLeftSelector) || (&(*selector) == this->svgRightSelector))
			{
				this->selectorMoving = selector;
				clickOffset = event->pos().x() - selectorMoving->pos().x();
			}
			else
			{
				clickOffset = 0;
				this->selectorMoving = 0;
			}
		}
		else
		{
			this->selectorMoving = 0;
		}
	}

	void QARehabFileControlWidget::mouseMoveEvent(QMouseEvent * event)
	{
		if (this->selectorMoving)
		{
			qreal	selectorWidth = this->selectorMoving->boundingRect().width(),
				posY = this->selectorMoving->pos().y(),
				rightSelectorWidth = this->svgRightSelector->boundingRect().width(),
				leftSelectorWidth = this->svgLeftSelector->boundingRect().width();

			if ((&(*this->selectorMoving)) == this->svgLeftSelector) //Left selector
			{
				qreal posX = event->pos().x() - clickOffset;
				posX = qMin(qMin(qMax(posX, minPos), maxPos), svgRightSelector->pos().x()) - leftSelectorWidth + 1;
				this->selectorMoving->setPos(qCeil(posX), posY);
				this->model.leftValue = mapViewPosToModelPos(posX + leftSelectorWidth - this->itemTimeline->pos().x());
				itemTXModelLeftValue->setText(QString::number(this->model.leftValue));
				itemTXModelLeftValue->setPos(leftSelectorWidth - itemTXModelLeftValue->boundingRect().width(), svgLeftSelector->boundingRect().height());

				if (lastFrame != model.leftValue)
				{
					lastFrame = model.leftValue;
					emit sig_LeftChanged(this->model.leftValue);
				}
			}
			else //Right selector
			{
				qreal posX = event->pos().x() - clickOffset;
				posX = qMax(qMin(qMax(posX, minPos), maxPos), svgLeftSelector->pos().x() + leftSelectorWidth + 1);
				this->selectorMoving->setPos(qFloor(posX), posY);
				this->model.rightValue = mapViewPosToModelPos(posX - this->itemTimeline->pos().x());

				itemTXModelRightValue->setText(QString::number(this->model.rightValue));
				itemTXModelRightValue->setPos(rightSelectorWidth - itemTXModelRightValue->boundingRect().width(), svgRightSelector->boundingRect().height());

				if (lastFrame != model.rightValue){
					this->lastFrame = model.rightValue;
					emit sig_RightChanged(this->model.rightValue);
				}
			}
			float rectx = this->svgLeftSelector->pos().x() + this->svgLeftSelector->boundingRect().width();
			itemCuttingInterval->setRect(rectx, 0, this->svgRightSelector->pos().x() - rectx, 20);
		}
		QGraphicsView::mouseMoveEvent(event);
	}

	void QARehabFileControlWidget::mouseReleaseEvent(QMouseEvent * event)
	{
		if (this->selectorMoving)
		{
			if ((&(*this->selectorMoving)) == this->svgLeftSelector) //Left selector
			{
				if (lastFrame != model.leftValue)
				{
					this->lastFrame = model.leftValue;
					emit sig_LeftChanged(this->model.leftValue);
				}
			}
			else {
				if (lastFrame != model.rightValue)
				{
					this->lastFrame = model.rightValue;
					emit sig_RightChanged(this->model.rightValue);
				}
			}
			this->doLayout();
		}
		clickOffset = 0;
		this->selectorMoving = 0;
		QGraphicsView::mouseReleaseEvent(event);
	}

};