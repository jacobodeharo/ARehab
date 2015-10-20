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

#ifndef QGRAPHICSITEMTOOLKIT_H
#define QGRAPHICSITEMTOOLKIT_H

#include <QGraphicsItem>

namespace ARehabUI
{
	class QGraphicsItemToolkit
	{
		public:
			static void moveCenterTo(QGraphicsItem * item, qreal x, qreal y);
			static void centerItemHorizontally(QGraphicsItem * item);
			static void centerItemVertically(QGraphicsItem * item);
			static void centerItemHRelativeTo(QGraphicsItem * item, QGraphicsItem * itemRef);
			static void centerItemVRelativeTo(QGraphicsItem * item, QGraphicsItem * itemRef);
			static void centerItem(QGraphicsItem * item);
			static void moveTopRight(QGraphicsItem * item, float margin = 0);
			static void moveTopLeft(QGraphicsItem * item, float margin = 0);
			static void moveToBottom(QGraphicsItem * item, float margin = 0);
			static void stackLeft(QGraphicsItem * item, QGraphicsItem * itemRef, float margin = 0);
			static void stackTop(QGraphicsItem * item, QGraphicsItem * itemRef, float margin = 0);
			static void stackRight(QGraphicsItem * item, QGraphicsItem * itemRef, float margin = 0);
			static void stackBottom(QGraphicsItem * item, QGraphicsItem * itemRef, float margin = 0);
	};

}
#endif // QGRAPHICSITEMTOOLKIT_H
