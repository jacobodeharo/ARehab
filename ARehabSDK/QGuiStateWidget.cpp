
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


namespace ARehabUI
{

	unsigned int QGUIStateWidget::numStates = 3;

	QGUIStateWidget::QGUIStateWidget(QWidget * parent) :
		QGraphicsView(new QGraphicsScene(), parent),
		containerRect(0),
		svgBack(0),
		svgNext(0),
		svgSave(0),
		svgCurrentState(0),
		svgFinalState(0),
		svgInitialState(0),
		svgInnerState(0),
		svgstate1(0),
		svgstate2(0),
		svgstate3(0),
		txtBack(0),
		txtNext(0),
		txtSave(0),
		txtState0(0),
		txtState1(0),
		txtState2(0),
		itemHovered(0),
		currentState(1),
		validationState(QGUIStateWidget::numStates)
	{
		this->init();
	}

	QGUIStateWidget::~QGUIStateWidget(void)
	{

	}

	void QGUIStateWidget::loadSVGFiles(void)
	{
		QGraphicsScene * scene = this->scene();
		scene->setSceneRect(0, 0, this->width(), this->height());
		scene->setBackgroundBrush(Qt::NoBrush);

		containerRect = new QGraphicsRectItem(0, 0, this->width(), this->height());
		containerRect->setBrush(Qt::NoBrush);
		containerRect->setPen(Qt::NoPen);
		scene->addItem(containerRect);

		svgBack = new QSVGInteractiveItem(":/svg/back.svg", containerRect);
		svgInitialState = new QGraphicsSvgItem(":/svg/initialState.svg", containerRect);
		svgInnerState = new QGraphicsSvgItem(":/svg/innerState.svg", containerRect);
		svgFinalState = new QGraphicsSvgItem(":/svg/finalState.svg", containerRect);
		svgNext = new QSVGInteractiveItem(":/svg/next.svg", containerRect);
		svgSave = new QSVGInteractiveItem(":/svg/save.svg", containerRect);

		svgstate1 = new QSVGTristateItem(":/svg/state.svg", ":/svg/stateok.svg", ":/svg/stateFail.svg", containerRect);
		svgstate2 = new QSVGTristateItem(":/svg/state.svg", ":/svg/stateok.svg", ":/svg/stateFail.svg", containerRect);
		svgstate3 = new QSVGTristateItem(":/svg/state.svg", ":/svg/stateok.svg", ":/svg/stateFail.svg", containerRect);

		txtState0 = new QGraphicsSimpleTextItem(QLatin1String("Definition"), containerRect);
		txtState1 = new QGraphicsSimpleTextItem(QLatin1String("Recording"), containerRect);
		txtState2 = new QGraphicsSimpleTextItem(QLatin1String("Cutting"), containerRect);
		txtBack = new QGraphicsSimpleTextItem(tr("Back"), svgBack);
		txtNext = new QGraphicsSimpleTextItem(tr("Next"), svgNext);
		txtSave = new QGraphicsSimpleTextItem(tr("Save"), svgSave);

		svgCurrentState = new QGraphicsSvgItem(":/svg/currentState.svg", containerRect);

		QFont font("Verdana", 12);
		txtState0->setFont(font);
		txtState2->setPen(Qt::NoPen);
		txtState0->setBrush(QBrush(QColor(0, 0, 0, 150)));

		txtState1->setFont(font);
		txtState2->setPen(Qt::NoPen);
		txtState1->setBrush(QBrush(QColor(0, 0, 0, 150)));

		txtState2->setFont(font);
		txtState2->setPen(Qt::NoPen);
		txtState2->setBrush(QBrush(QColor(0, 0, 0, 150)));

		txtBack->setFont(font);
		txtBack->setPen(Qt::NoPen);
		txtBack->setBrush(QBrush(QColor(200, 200, 200, 255)));

		txtNext->setFont(font);
		txtNext->setPen(Qt::NoPen);
		txtNext->setBrush(QBrush(QColor(200, 200, 200, 255)));

		font.setPixelSize(11);
		txtSave->setFont(font);
		txtSave->setPen(Qt::NoPen);
		txtSave->setBrush(QBrush(QColor(200, 200, 200, 255)));
	}

	void QGUIStateWidget::doLayout(void)
	{
		QGraphicsItemToolkit::centerItem(svgInnerState);

		QGraphicsItemToolkit::stackLeft(svgInitialState, svgInnerState, -10.0);
		QGraphicsItemToolkit::stackRight(svgFinalState, svgInnerState, -10.0);

		QGraphicsItemToolkit::centerItemVertically(svgstate1);
		svgstate1->setX(svgInitialState->pos().x() - (svgstate1->boundingRect().width() / 2.0f));
		QGraphicsItemToolkit::moveTopLeft(svgBack);
		QGraphicsItemToolkit::centerItemVertically(svgBack);

		QGraphicsItemToolkit::centerItem(svgstate2);

		QGraphicsItemToolkit::centerItemVertically(svgstate3);
		svgstate3->setX(svgFinalState->pos().x() + svgFinalState->boundingRect().width() - (svgstate3->boundingRect().width() / 2.0f));
		QGraphicsItemToolkit::moveTopRight(svgNext);
		QGraphicsItemToolkit::centerItemVertically(svgNext);

		svgSave->setPos(svgNext->pos());

		QGraphicsItemToolkit::stackTop(txtState0, svgstate1, 10);
		QGraphicsItemToolkit::stackTop(txtState1, svgstate2, 10);
		QGraphicsItemToolkit::stackTop(txtState2, svgstate3, 10);

		QSVGTristateItem * item = this->stateItems.at(this->currentState);
		if (item != 0)
		{
			this->svgCurrentState->setPos(item->pos());
		}
		else{
			this->hide();
		}

		svgstate1->doLayout();
		svgstate2->doLayout();

		QGraphicsItemToolkit::moveTopRight(txtBack, 8);
		QGraphicsItemToolkit::centerItemVertically(txtBack);

		QGraphicsItemToolkit::moveTopLeft(txtNext, 8);
		QGraphicsItemToolkit::centerItemVertically(txtNext);

		QGraphicsItemToolkit::centerItemHorizontally(txtSave);
		txtSave->setPos(txtSave->pos().x(), 40);

		QGraphicsItemToolkit::centerItem(containerRect);
		this->containerRect->setTransformOriginPoint(this->containerRect->rect().center());
	}

	void QGUIStateWidget::showNextButton(bool show)
	{
		if (this->svgNext->isVisible() && !show)
		{
			this->svgNext->hide();
			this->svgSave->show();
		}
		else
		{
			this->svgNext->show();
			this->svgSave->hide();
		}
	}

	void QGUIStateWidget::setButtonSaveEnabled(bool enabled)
	{
		this->svgSave->setEnabled(enabled);
	}

	void QGUIStateWidget::setStateValidation(unsigned int guistate, QSVGTristateItem::ItemState state)
	{
		if (state == QSVGTristateItem::ItemState::Valid)
		{
			this->validationState.setBit(true);
		}
		else
		{
			this->validationState.setBit(false);
		}

		switch (guistate)
		{
		case Initial:
			break;

		case Definition:
			this->svgstate1->setState(state);
			break;

		case Recording:
			this->svgstate2->setState(state);
			break;

		case Cutting:
			this->svgstate3->setState(state);
			break;
		}
	}

	bool QGUIStateWidget::getStateValidation(unsigned int guistate)
	{
		return this->validationState.testBit(guistate);
	}

	void QGUIStateWidget::resetWidget(void)
	{
		for (unsigned int i = 0; i < stateItems.size(); ++i)
		{
			if (NULL != stateItems[i]) //WARNING: NULL pointer at first position, inserted for functionality purposes (TODO: change this in the future)
				stateItems[i]->reset();
		}
		this->showNextButton(true);
	}

	void QGUIStateWidget::setStateName(unsigned int stateID, QLatin1String name)
	{
		switch (stateID)
		{
			case 0:
				this->txtState0->setText(name);
				break;
			case 1:
				this->txtState1->setText(name);
				break;
			case 2:
				this->txtState2->setText(name);
				break;
		}
	}

	void QGUIStateWidget::slot_actionBack(void)
	{
		unsigned int previousState = this->currentState;
		this->currentState--;
		if (this->currentState < 0)
			this->currentState = 0;

		QSVGTristateItem * item = this->stateItems.at(this->currentState);
		if (item != 0)
		{
			this->svgCurrentState->setPos(item->pos());
		}
		else{
			this->hide();
		}

		emit sig_changeState(previousState, currentState);
	}

	void QGUIStateWidget::slot_actionState0Selected(void)
	{
		unsigned int previousState = this->currentState;
		this->currentState = QGUIStateWidget::Definition;
		QSVGTristateItem * item = this->stateItems.at(this->currentState);
		if (item != 0)
		{
			this->svgCurrentState->setPos(item->pos());
		}
		else{
			this->hide();
		}

		emit sig_changeState(previousState, QGUIStateWidget::Definition);
	}

	void QGUIStateWidget::slot_actionState1Selected(void)
	{
		unsigned int previousState = this->currentState;
		this->currentState = QGUIStateWidget::Recording;
		QSVGTristateItem * item = this->stateItems.at(this->currentState);
		if (item != 0)
		{
			this->svgCurrentState->setPos(item->pos());
		}
		else{
			this->hide();
		}
		emit sig_changeState(previousState, QGUIStateWidget::Recording);
	}

	void QGUIStateWidget::slot_actionState2Selected(void)
	{
		unsigned int previousState = this->currentState;
		this->currentState = QGUIStateWidget::Cutting;
		QSVGTristateItem * item = this->stateItems.at(this->currentState);
		if (item != 0)
		{
			this->svgCurrentState->setPos(item->pos());
		}
		else{
			this->hide();
		}
		emit sig_changeState(previousState, QGUIStateWidget::Cutting);
	}

	void QGUIStateWidget::slot_actionNext(void)
	{
		unsigned int previousState = this->currentState;
		this->currentState++;
		if (this->currentState >= QGUIStateWidget::NumStatesDesigner)
			this->currentState = 0;

		QSVGTristateItem * item = this->stateItems.at(this->currentState);
		if (item != 0)
		{
			this->svgCurrentState->setPos(item->pos());
		}
		else{
			this->hide();
		}

		emit sig_changeState(previousState, this->currentState);
	}

	void QGUIStateWidget::slot_actionSave(void)
	{
		emit sig_Save();
	}


	void QGUIStateWidget::connectItems(void)
	{
		QObject::connect(this->svgBack, &QSVGInteractiveItem::clicked, this, &QGUIStateWidget::slot_actionBack);
		QObject::connect(this->svgNext, &QSVGInteractiveItem::clicked, this, &QGUIStateWidget::slot_actionNext);
		QObject::connect(this->svgSave, &QSVGInteractiveItem::clicked, this, &QGUIStateWidget::slot_actionSave);
		QObject::connect(this->svgstate1, &QSVGInteractiveItem::clicked, this, &QGUIStateWidget::slot_actionState0Selected);
		QObject::connect(this->svgstate2, &QSVGInteractiveItem::clicked, this, &QGUIStateWidget::slot_actionState1Selected);
		QObject::connect(this->svgstate3, &QSVGInteractiveItem::clicked, this, &QGUIStateWidget::slot_actionState2Selected);
	}

	void QGUIStateWidget::init(void)
	{
		this->setMinimumWidth(1000);
		this->setMinimumHeight(140);
		this->setMouseTracking(true);
		this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
		this->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
		this->setStyleSheet("border: none; background: transparent;");

		loadSVGFiles();

		this->stateItems.push_back(0);
		this->stateItems.push_back(this->svgstate1);
		this->stateItems.push_back(this->svgstate2);
		this->stateItems.push_back(this->svgstate3);

		connectItems();
		doLayout();
	}

	void QGUIStateWidget::showEvent(QShowEvent * event)
	{
		QSVGTristateItem * item = this->stateItems.at(this->currentState);
		if (item != 0)
		{
			this->svgCurrentState->setPos(item->pos());
		}
		this->svgNext->graphicsEffect()->setEnabled(!this->svgNext->enabled);
		this->svgBack->graphicsEffect()->setEnabled(!this->svgBack->enabled);
		this->svgstate1->graphicsEffect()->setEnabled(!this->svgstate1->enabled);
		this->svgstate2->graphicsEffect()->setEnabled(!this->svgstate2->enabled);
		this->svgstate3->graphicsEffect()->setEnabled(!this->svgstate3->enabled);
		this->updateSceneRect(this->sceneRect());
	}

	void QGUIStateWidget::hideEvent(QHideEvent * event)
	{
		this->svgNext->graphicsEffect()->setEnabled(!this->svgNext->enabled);
		this->svgBack->graphicsEffect()->setEnabled(!this->svgBack->enabled);
		this->svgstate1->graphicsEffect()->setEnabled(!this->svgstate1->enabled);
		this->svgstate2->graphicsEffect()->setEnabled(!this->svgstate2->enabled);
		this->svgstate3->graphicsEffect()->setEnabled(!this->svgstate3->enabled);
		this->updateSceneRect(this->sceneRect());
	}


	void QGUIStateWidget::resizeEvent(QResizeEvent * event)
	{
		Q_UNUSED(event);
		QGraphicsScene * scene = this->scene();

		scene->setSceneRect(0, 0, width(), height());
		containerRect->setRect(0, 0, this->width(), this->height());
		containerRect->setPos((this->width() / 2.0f) - (containerRect->boundingRect().width() / 2.0f), (this->height() / 2.0f) - (containerRect->boundingRect().height() / 2.0f));

		doLayout();
	}

};
