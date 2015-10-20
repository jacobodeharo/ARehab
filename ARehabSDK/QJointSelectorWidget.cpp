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

#include "qguistatewidget.h"

#include "qjointselectorwidget.h"

namespace ARehabUI
{

	QJointSelectorWidget::QJointSelectorWidget(QWidget * parent) :
		QGraphicsView(new QGraphicsScene(), parent),
		containerRect(0),
		humanSilohuete(0),
		textLeft(0),
		textRight(0),
		itemHovered(0),
		jointSelectionModel(JointCount, false)
	{
		this->init();
		this->setMinimumWidth(455);
	}

	QJointSelectorWidget::~QJointSelectorWidget(void)
	{
	}

	void QJointSelectorWidget::loadSVGFiles(void)
	{
		QGraphicsScene * scene = this->scene();
		//scene->setSceneRect(0, 0, this->width(), this->height());
		scene->setBackgroundBrush(Qt::NoBrush);

		containerRect = new QGraphicsRectItem(0, 0, this->width(), this->height());
		containerRect->setBrush(Qt::NoBrush);
		containerRect->setPen(Qt::NoPen);
		scene->addItem(containerRect);

		humanSilohuete = new QGraphicsSvgItem(":/svg/humansilhouette.svg", containerRect);
		for (unsigned int i = 0; i < JointCount; ++i)
		{
			this->jointSelectors.push_back(new QJointSelectorItem(i, ":/svg/jointSelector.svg", humanSilohuete));
		}

		textLeft = new QGraphicsSimpleTextItem("IZQUIERDA", containerRect);
		textRight = new QGraphicsSimpleTextItem("DERECHA", containerRect);

		QFont font("Verdana", 12);

		textLeft->setFont(font);
		textLeft->setPen(Qt::NoPen);
		textLeft->setBrush(QBrush(QColor(0, 0, 0, 100)));

		textRight->setFont(font);
		textRight->setPen(Qt::NoPen);
		textRight->setBrush(QBrush(QColor(0, 0, 0, 100)));
	}

	QString QJointSelectorWidget::jointToString(unsigned int i)
	{
		switch (i)
		{
			case Head: return(QLatin1String("Cabeza")); break;
			case Neck: return(QLatin1String("Cuello")); break;
			case SpineShoulder: return(QLatin1String("Espalda")); break;
			case SpineMid: return(QLatin1String("Media Espalda")); break;
			case SpineBase: return(QLatin1String("Base de columna")); break;
			case ShoulderLeft: return(QLatin1String("Hombro Izquierdo")); break;
			case ShoulderRight: return(QLatin1String("Hombro Derecho")); break;
			case ElbowLeft: return(QLatin1String("Codo Izquierdo")); break;
			case ElbowRight: return(QLatin1String("Codo Derecho")); break;
			case WristLeft: return(QLatin1String("Muñeca Izquierda")); break;
			case HandLeft: return(QLatin1String("Mano Izquierda")); break;
			case WristRight: return(QLatin1String("Muñeca Derecha")); break;
			case HandRight: return(QLatin1String("Mano Derecha"));	break;
			case HipLeft: return(QLatin1String("Cadera Izquierda")); break;
			case HipRight: return(QLatin1String("Cadera Derecha")); break;
			case KneeLeft: return(QLatin1String("Rodilla Izquierda"));	break;
			case KneeRight: return(QLatin1String("Rodilla Derecha"));	break;
			case AnkleLeft: return(QLatin1String("Tobillo Izquierdo")); break;
			case AnkleRight: return(QLatin1String("Tobillo Derecho"));	break;
			case FootLeft: return(QLatin1String("Pié Izquierdo"));	break;
			case FootRight: return(QLatin1String("Pié Derecho"));	break;
			default: return(QLatin1String("Sin definir")); break;
		}
	}

	void QJointSelectorWidget::doLayout(void)
	{
		QGraphicsScene * scene = this->scene();
		//scene->setSceneRect(0, 0, this->width(), this->height());
		
		QGraphicsItemToolkit::centerItemHorizontally(humanSilohuete);
		QGraphicsItemToolkit::centerItemHorizontally(containerRect);

		for (unsigned int i = 0; i < JointCount; ++i)
		{
			this->jointSelectors[i]->setToolTip(jointToString(i));
		}

		QGraphicsItemToolkit::moveCenterTo(jointSelectors[Head], 104, 25);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[Neck], 104, 55);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[SpineShoulder], 104, 88);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[SpineMid], 104, 165);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[SpineBase], 104, 242);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[ShoulderLeft], 58, 100);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[ShoulderRight], 150, 100);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[ElbowLeft], 44, 176);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[ElbowRight], 166, 176);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[WristLeft], 28, 228);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[HandLeft], 20, 250);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[WristRight], 182, 228);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[HandRight], 190, 250);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[HipLeft], 72, 248);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[HipRight], 135, 248);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[KneeLeft], 77, 360);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[KneeRight], 132, 360);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[AnkleLeft], 86, 446);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[AnkleRight], 124, 446);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[FootLeft], 86, 466);
		QGraphicsItemToolkit::moveCenterTo(jointSelectors[FootRight], 125, 466);

		this->containerRect->setTransformOriginPoint(this->containerRect->rect().width() / 2.0, this->containerRect->rect().y());	

		QGraphicsItemToolkit::stackLeft(this->textLeft, this->humanSilohuete, 30);
		QGraphicsItemToolkit::stackRight(this->textRight, this->humanSilohuete, 30);
		QGraphicsItemToolkit::centerItemVertically(this->textLeft);
		QGraphicsItemToolkit::centerItemVertically(this->textRight);
	}

	void QJointSelectorWidget::setJointSelectorModel(const QBitArray &jointsStateModel)
	{
		this->jointSelectionModel = jointsStateModel;
		for (unsigned int i = 0; i < JointCount; ++i)
		{
			this->jointSelectors[i]->setState(this->jointSelectionModel[i]);
		}
		emit sig_jointsStateChanged(this->jointSelectionModel);
	}

	QBitArray& QJointSelectorWidget::getJointSelectorModel(void)
	{
		return this->jointSelectionModel;
	}

	void QJointSelectorWidget::resetWidget(void)
	{
		QBitArray modelReset(JointCount, false);
		for (unsigned int id = 0; id < JointCount; ++id)
		{
			this->jointSelectors[id]->reset();
		}
		this->jointSelectionModel = modelReset;
		emit sig_jointsStateChanged(this->jointSelectionModel);
	}

	void QJointSelectorWidget::slot_jointStateChanged(unsigned int id, bool value)
	{
		if (id < 0 || id >= JointCount)
		{
			qDebug() << "QJointSelectorWidget::slot_jointStateChanged: ERROR" << endl;
			return;
		}
		this->jointSelectionModel[id] = value;
		emit sig_jointsStateChanged(this->jointSelectionModel);
	}

	void QJointSelectorWidget::connectItems(void)
	{
		for (unsigned int i = 0; i < JointCount; ++i)
		{
			QObject::connect(jointSelectors[i], &QJointSelectorItem::sig_stateChanged, this, &QJointSelectorWidget::slot_jointStateChanged);
		}
	}

	void QJointSelectorWidget::init(void)
	{
		this->setMinimumWidth(300);
		//this->setMinimumHeight(500);
		this->setMouseTracking(true);
		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
		this->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
		this->setStyleSheet("border: none; background: transparent");

		//QGraphicsScene * scene = this->scene();

		loadSVGFiles();
		connectItems();
		doLayout();
	}

	void QJointSelectorWidget::resizeEvent(QResizeEvent * event)
	{
		QGraphicsScene * scene = this->scene();
		scene->setSceneRect(0, 0, width(), height());
		containerRect->setRect(0, 0, this->width(), this->height());

		doLayout();
		for (unsigned short i = 0; i < this->jointSelectors.size(); ++i)
		{
			this->jointSelectors[i]->doLayout();
		}
	}

};