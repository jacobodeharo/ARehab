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