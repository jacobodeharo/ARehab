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
