#include "qgraphicsitemtoolkit.h"

namespace ARehabUI
{

	void  QGraphicsItemToolkit::moveCenterTo(QGraphicsItem * item, qreal x, qreal y)
	{
		if (item)
		{
			item->setPos(x - (item->boundingRect().width() / 2.0), y - (item->boundingRect().height() / 2.0));
		}
	}

	void QGraphicsItemToolkit::centerItemHorizontally(QGraphicsItem * item)
	{
		QGraphicsItem * parent = item->parentItem();
		if (parent)
		{
			item->setPos((parent->boundingRect().width() - item->boundingRect().width()) / 2.0, item->pos().y());
		}
	}

	void QGraphicsItemToolkit::centerItemVertically(QGraphicsItem * item)
	{
		QGraphicsItem * parent = item->parentItem();
		if (parent)
		{
			item->setPos(item->pos().x(), (parent->boundingRect().height() - item->boundingRect().height()) / 2.0);
		}
	}

	void QGraphicsItemToolkit::centerItemHRelativeTo(QGraphicsItem * item, QGraphicsItem * itemRef)
	{
		if (itemRef && item)
		{
			item->setPos(itemRef->x() - (item->boundingRect().width() / 2.0) + (itemRef->boundingRect().width() / 2.0),
				item->pos().y()
				);
		}
	}

	void QGraphicsItemToolkit::centerItemVRelativeTo(QGraphicsItem * item, QGraphicsItem * itemRef)
	{
		if (itemRef && item)
		{
			item->setPos(item->x(),
				itemRef->pos().y() - (item->boundingRect().height() / 2.0) + (itemRef->boundingRect().height() / 2.0)
				);
		}
	}

	void QGraphicsItemToolkit::centerItem(QGraphicsItem * item)
	{
		QGraphicsItem * parent = item->parentItem();
		if (parent)
		{

			item->setPos((parent->boundingRect().width() / 2.0) - (item->boundingRect().width() / 2.0),
				((parent->boundingRect().height() / 2.0) - (item->boundingRect().height()) / 2.0)
				);
		}
	}

	void QGraphicsItemToolkit::moveTopRight(QGraphicsItem * item, float margin)
	{
		QGraphicsItem * parent = item->parentItem();
		if (parent)
		{
			item->setPos((parent->boundingRect().width() - item->boundingRect().width() - margin),
				item->pos().y()
				);
		}
	}

	void QGraphicsItemToolkit::moveTopLeft(QGraphicsItem * item, float margin)
	{
		item->setPos(margin, item->pos().y());
	}

	void QGraphicsItemToolkit::moveToBottom(QGraphicsItem * item, float margin)
	{
		QGraphicsItem * parent = item->parentItem();
		if (parent)
		{
			item->setPos(item->pos().x(),
				parent->boundingRect().height() - item->boundingRect().height() - margin
				);
		}
	}

	void QGraphicsItemToolkit::stackLeft(QGraphicsItem * item, QGraphicsItem * itemRef, float margin)
	{
		if (item && itemRef)
		{
			item->setPos(itemRef->pos().x() - item->boundingRect().width() - margin,
				itemRef->pos().y()
				);
		}
	}

	void QGraphicsItemToolkit::stackTop(QGraphicsItem * item, QGraphicsItem * itemRef, float margin)
	{
		if (item && itemRef)
		{
			item->setPos(itemRef->x() - (item->boundingRect().width() / 2.0) + (itemRef->boundingRect().width() / 2.0),
				itemRef->y() - (itemRef->boundingRect().height() / 2.0) - margin
				);
		}
	}

	void QGraphicsItemToolkit::stackBottom(QGraphicsItem * item, QGraphicsItem * itemRef, float margin)
	{
		if (item && itemRef)
		{
			item->setPos(itemRef->boundingRect().center().x(),
				itemRef->pos().y() + itemRef->boundingRect().height() + margin
				);
		}
	}

	void QGraphicsItemToolkit::stackRight(QGraphicsItem * item, QGraphicsItem * itemRef, float margin)
	{
		if (item && itemRef)
		{
			item->setPos(itemRef->pos().x() + itemRef->boundingRect().width() + margin,
				itemRef->pos().y()
				);
		}
	}

};//namespace ARehabUI
