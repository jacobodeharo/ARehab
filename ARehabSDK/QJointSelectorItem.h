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

#ifndef QJOINTSELECTORITEM_H
#define QJOINTSELECTORITEM_H

#include "QSVGInteractiveItem.h"

namespace ARehabUI
{	
	class QJointSelectorItem : public QSVGInteractiveItem
	{
		Q_OBJECT

		public:
			QJointSelectorItem(unsigned int id, const QString filename, QGraphicsItem * parent = 0);
			virtual void reset(void);
			void setState(bool value);
			virtual void setActive(bool enabled);
			virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
			virtual void doLayout(void);
			virtual ~QJointSelectorItem(void);

		private:			
			unsigned int id;
			QGraphicsSvgItem * svgCheck;

		signals:
			void sig_stateChanged(unsigned int, bool);
	};

};

#endif // QJOINTSELECTORITEM_H
