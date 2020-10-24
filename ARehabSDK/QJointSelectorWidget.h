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

#ifndef QJOINTSELECTORWIDGET_H
#define QJOINTSELECTORWIDGET_H

#include <QBitArray>

#include "QGraphicsItemToolkit.h"
#include "QSVGInteractiveItem.h"
#include "QJointSelectorItem.h"

namespace ARehabUI
{

	class QJointSelectorWidget : public QGraphicsView
	{
		Q_OBJECT

		public:
			enum Joints {
				SpineBase,		//	Base of the spine
				SpineMid,		// 	Middle of the spine
				Neck,			// 	Neck
				Head,			//	Head
				ShoulderLeft,	//	Left shoulder
				ElbowLeft,		//	Left elbow
				WristLeft,		// 	Left wrist
				HandLeft,		//	Left hand
				ShoulderRight,	//	Right shoulder
				ElbowRight,		//	Right elbow
				WristRight,		// Right wrist
				HandRight,		// 	Right hand
				HipLeft,		// 	Left hip
				KneeLeft,		// 	Left knee
				AnkleLeft,		// 	Left ankle
				FootLeft,		//	Left foot
				HipRight,		// 	Right hip
				KneeRight,		// 	Right knee
				AnkleRight,		//	Right ankle			
				FootRight,		// 	Right foot
				SpineShoulder,	//	Spine			
				//HandTipLeft,	// 	Tip of the left hand
				//ThumbLeft,		// 	Left thumb
				//HandTipRight,	// 	Tip of the right hand			
				//ThumbRight,		// 	Right thumb
				JointCount		//Joint count
			};

			explicit QJointSelectorWidget(QWidget * parent = 0);
			virtual ~QJointSelectorWidget(void);			
			void doLayout(void);
			void setJointSelectorModel(const QBitArray &jointsStateModel);
			QBitArray& getJointSelectorModel(void);
			void resetWidget(void);

		public slots:
			void slot_jointStateChanged(unsigned int, bool);

		signals:
			void sig_jointsStateChanged(QBitArray);

		protected:
			virtual void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;
			virtual void wheelEvent(QWheelEvent * event){}

		private:
			QGraphicsRectItem * containerRect;
			QGraphicsItem * itemHovered;
			QGraphicsSvgItem * humanSilohuete;
			QGraphicsSimpleTextItem * textLeft;
			QGraphicsSimpleTextItem * textRight;

			std::vector<QJointSelectorItem*> jointSelectors;
			QBitArray jointSelectionModel; //Internal Model containing the state of the widget

			void connectItems(void);
			void init(void);
			void loadSVGFiles(void);
			QString jointToString(unsigned int i);
	};

};

#endif // QJOINT_SELECTOR_H
