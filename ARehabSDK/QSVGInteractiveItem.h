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

#ifndef QSVGINTERACTIVEITEM_H
#define QSVGINTERACTIVEITEM_H

#include <QGraphicsSvgItem>
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
#include <QHideEvent>
#include <QDebug>
#include <QLinearGradient>
#include <QStateMachine>
#include <QState>

#include "QGraphicsItemToolkit.h"

namespace ARehabUI
{

	class QSVGInteractiveItem : public QGraphicsSvgItem
	{
		Q_OBJECT

	public:
		bool enabled;
		bool checkable;
		bool checked;
		QString filename;

		QSVGInteractiveItem(const QString filename, QGraphicsItem * parent = 0);
		QSVGInteractiveItem(const QSVGInteractiveItem &item);
		virtual void doLayout(void){};
		virtual void setEnabled(bool enable);
		virtual void setCheckable(bool checkable);
		virtual void reset(bool enabled = true);

	signals:
		void clicked(void);
		void stateChanged(bool);

	protected:
		virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
		virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;
		virtual void mousePressEvent(QGraphicsSceneMouseEvent * event) Q_DECL_OVERRIDE;		

		QGraphicsColorizeEffect * effect;
		QColor colorHover;
		QColor colorDisabled;
		QColor colorChecked;
		QString hoverText;

	};
}

#endif // QSVGINTERACTIVEITEM_H
