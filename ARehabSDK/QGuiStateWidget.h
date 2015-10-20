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

#ifndef QGUISTATEWIDGET_H
#define QGUISTATEWIDGET_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QtSvg/QGraphicsSvgItem>
#include <QGraphicsColorizeEffect>
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>
#include <QLinearGradient>
#include <QStateMachine>
#include <QState>
#include <QPropertyAnimation>
#include <QBitArray>

#include "QSVGTristateItem.h"

namespace ARehabUI
{

	class QGUIStateWidget : public QGraphicsView
	{
		Q_OBJECT

		public:
			static enum {
				Initial = 0,
				Definition = 1
			} StatesCommon;
			
			static enum {
				Recording = 2,
				Cutting = 3,
				NumStatesDesigner = 4
			} StatesDesigner;
			
			static enum {
				Preview = 2,
				Playing = 3,
				NumStatesPlayer = 4
			} StatesPlayer;

			explicit QGUIStateWidget(QWidget * parent = 0);
			virtual ~QGUIStateWidget(void);
			void doLayout(void);
			void showNextButton(bool);
			void setButtonSaveEnabled(bool);
			void setStateValidation(unsigned int guistate, QSVGTristateItem::ItemState state);
			bool getStateValidation(unsigned int guistate);
			void resetWidget(void);
			void setStateName(unsigned int stateID, QLatin1String name);

			static unsigned int numStates;
			int currentState;

		public slots:
			void slot_actionBack(void);
			void slot_actionState0Selected(void);
			void slot_actionState1Selected(void);
			void slot_actionState2Selected(void);
			void slot_actionNext(void);
			void slot_actionSave(void);
		
		signals:
			void sig_previousState(void);
			void sig_nextState(void);
			void sig_changeState(unsigned int state0, unsigned int state1);
			void sig_Save(void);

		protected:
			virtual void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
			virtual void showEvent(QShowEvent * event) Q_DECL_OVERRIDE;
			virtual void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE {}
			virtual void hideEvent(QHideEvent * event) Q_DECL_OVERRIDE;

		private:
			QBitArray validationState; //Model

			QGraphicsRectItem * containerRect;
			QGraphicsItem * itemHovered;

			QSVGInteractiveItem * svgBack;
			QGraphicsSvgItem * svgInnerState;
			QGraphicsSvgItem * svgInitialState;
			QGraphicsSvgItem * svgFinalState;
			QSVGInteractiveItem * svgNext;
			QSVGInteractiveItem * svgSave;

			QVector<QSVGTristateItem *> stateItems;
			QSVGTristateItem * svgstate1;
			QSVGTristateItem * svgstate2;
			QSVGTristateItem * svgstate3;

			QGraphicsSvgItem * svgCurrentState;

			QGraphicsSimpleTextItem * txtState0;
			QGraphicsSimpleTextItem * txtState1;
			QGraphicsSimpleTextItem * txtState2;
			QGraphicsSimpleTextItem * txtBack;
			QGraphicsSimpleTextItem * txtNext;
			QGraphicsSimpleTextItem * txtSave;

			void connectItems(void);
			void init(void);
			void loadSVGFiles(void);
	};

};

#endif // QGUISTATEWIDGET_H
