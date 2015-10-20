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

#include "QJointSelectorItem.h"

namespace ARehabUI
{
	QJointSelectorItem::QJointSelectorItem(unsigned int id, const QString filename, QGraphicsItem * parent) :
		QSVGInteractiveItem(filename, parent),
		svgCheck(new QGraphicsSvgItem(":/svg/jointSelected.svg")),
		id(id)
	{
		this->scene()->addItem(svgCheck);
		//svgCheck->setPos(this->pos());
		svgCheck->hide();
		this->setCheckable(true);
		this->colorChecked = QColor(0, 255, 0);
	}

	void QJointSelectorItem::reset(void)
	{
		QSVGInteractiveItem::reset();

		this->setSelected(false);
		svgCheck->hide();
		if (this->enabled)
		{
			this->effect->setColor(this->colorHover);
		}
	}

	void QJointSelectorItem::setState(bool value)
	{
		if (value)
		{
			this->setSelected(true);
			svgCheck->show();
			if (this->enabled)
			{
				this->effect->setColor(this->colorChecked);
				this->effect->setEnabled(true);
			}
		}
		else{
			this->setSelected(false);
			svgCheck->hide();
			if (this->enabled)
			{
				this->effect->setColor(this->colorHover);
			}
		}
		this->update(this->boundingRect());
		this->checked = value;
		doLayout();
	}

	void QJointSelectorItem::setActive(bool enabled)
	{
		this->setActive(enabled);
	}

	void QJointSelectorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
	{
		QSVGInteractiveItem::mouseReleaseEvent(event);
		if (this->checked)
		{
			svgCheck->show();
		}
		else{
			svgCheck->hide();
		}
		doLayout();
		emit sig_stateChanged(this->id, this->checked);
	}

	void QJointSelectorItem::doLayout(void)
	{
		svgCheck->setPos(this->scenePos());
	}

	QJointSelectorItem::~QJointSelectorItem(void)
	{
	}

};