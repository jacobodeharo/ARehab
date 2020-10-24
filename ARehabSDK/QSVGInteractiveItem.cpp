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

#include "qsvginteractiveitem.h"

namespace ARehabUI
{

	QSVGInteractiveItem::QSVGInteractiveItem(const QString filename, QGraphicsItem * parent) :
		QGraphicsSvgItem(filename, parent),
		filename(filename),
		effect(new  QGraphicsColorizeEffect()),
		colorHover(200, 150, 0),
		colorDisabled(200, 200, 200),
		colorChecked(250, 180, 0),
		enabled(true),
		checkable(false),
		checked(false)
	{
		setAcceptHoverEvents(true);
		effect->setColor(this->colorHover);
		this->setGraphicsEffect(effect);
		effect->setEnabled(false);
	}

	void QSVGInteractiveItem::setEnabled(bool enable)
	{
		this->enabled = enable;
		if (!enable)
		{
			this->effect->setColor(colorDisabled);
			this->effect->setEnabled(true);
		}
		else
		{
			this->effect->setEnabled(false);
			this->effect->setColor(colorHover);
		}
		this->update(this->boundingRect());
	}

	void QSVGInteractiveItem::setCheckable(bool checkable)
	{
		this->checked = false;
		this->checkable = checkable;
	}

	void QSVGInteractiveItem::reset(bool enabled)
	{
		this->checked = false;
		if (this->checkable)
			this->setEnabled(enabled);
	}

	void QSVGInteractiveItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
	{
		if (this->enabled)
		{
			this->setCursor(Qt::PointingHandCursor);
			this->effect->setColor(this->colorHover);
			this->effect->setEnabled(true);
			this->update(this->boundingRect());
		}
		else
		{
			this->setCursor(Qt::ArrowCursor);
		}
	}

	void QSVGInteractiveItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
	{
		if (this->enabled)
		{
			this->setCursor(Qt::ArrowCursor);
			if (this->checkable && this->checked)
			{
				this->effect->setColor(this->colorChecked);
			}
			else {
				this->effect->setColor(this->colorHover);
				this->effect->setEnabled(false);
			}
			this->update(this->boundingRect());
		}
	}

	void QSVGInteractiveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
	{
		if (this->enabled)
		{
			if (this->checkable)
				this->checked = !this->checked;
			if (!this->checked)
			{
				this->effect->setColor(this->colorHover);
				this->update(this->boundingRect());
			}
			if (this->checkable)
				emit stateChanged(this->checked);
			else
				emit clicked();
		}
	}

	void QSVGInteractiveItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
	{
		if (this->enabled)
		{
			this->effect->setColor(this->colorChecked);
			this->update(this->boundingRect());
		}
	}

};