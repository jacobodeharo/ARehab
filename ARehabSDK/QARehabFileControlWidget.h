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

#ifndef QAREHABFILECONTROLWIDGET_H
#define QAREHABFILECONTROLWIDGET_H

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
#include <QtMath>

#include "QGraphicsItemToolkit.h"
#include "QSVGInteractiveItem.h"
#include "QARehabFileControlModel.h"


namespace ARehabUI
{

	class QARehabFileControlWidget : public QGraphicsView
	{
		Q_OBJECT

	public:
		static int leftPadding;
		static int rightPadding;

		explicit QARehabFileControlWidget(QWidget * parent = 0);
		virtual ~QARehabFileControlWidget(void);
		void doLayout(void);
		void setModel(const QARehabFileControlModel &model);
		void resetPlayButton(void);
		void resetWidget(void);
		void resetMaximizeButton(void);

		unsigned long long int GetLeftFrameIndex(void)
		{			
			return model.leftValue;			
		}

		unsigned long long int GetRightFrameIndex(void)
		{
			return this->model.rightValue;
		}

		unsigned long long int GetFirstFrameIndex(void)
		{
			return model.minValue;
		}

		void SetEditable(bool editable)
		{
			this->intervalEditable = editable;
			if (this->isVisible())
				this->doLayout();
		}

	public slots:
		void slot_PlayPause(bool on);
		void slot_Restart(void);
		void slot_Maximize(bool on);

	signals:
		void sig_PlayPause(bool on);
		void sig_Restart(void);
		void sig_Maximize(bool on);
		void sig_LeftChanged(unsigned long long int);
		void sig_RightChanged(unsigned long long int);

	protected:
		virtual void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
		virtual void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
		virtual void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
		virtual void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;

	private:
		QARehabFileControlModel model;
		bool intervalEditable;

		QGraphicsRectItem * containerRect;
		QGraphicsItem * itemHovered;
		QSVGInteractiveItem * svgRestart;
		QSVGInteractiveItem * svgPlayPause;
		QSVGInteractiveItem * svgKinectMaximize;
		QGraphicsPolygonItem * itemBtContainer;

		QGraphicsRectItem * itemTimeline;
		QGraphicsRectItem * itemCuttingInterval;
		QSVGInteractiveItem * svgLeftSelector;
		QSVGInteractiveItem * svgRightSelector;
		QSVGInteractiveItem * selectorMoving;
		QGraphicsSimpleTextItem * itemTXModelLeftValue;
		QGraphicsSimpleTextItem * itemTXModelRightValue;

		double maxPos;
		double minPos;
		unsigned long long int lastFrame;
		qreal clickOffset; //Offset for the right positioning of the  interval selectors (Updated when user press mouse button)

		void connectItems(void);
		void init(void);
		void loadSVGFiles(void);

		inline virtual double mapModelPosToViewPos(unsigned long long int x)
		{
			return ((x - model.minValue) * ((this->itemTimeline->boundingRect().width()) / (model.maxValue - model.minValue)));
		}

		inline virtual unsigned long long int mapViewPosToModelPos(double x)
		{
			return ((x * ((model.maxValue - model.minValue) / (this->itemTimeline->boundingRect().width()))) + model.minValue);
		}
	};

};

#endif // QAREHABFILECONTROLWIDGET_H
