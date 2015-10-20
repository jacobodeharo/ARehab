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

#include "qsvgtristateitem.h"

namespace ARehabUI
{

	QSVGTristateItem::QSVGTristateItem(const QString filename, const QString filenameOk, const QString filenameBad, QGraphicsItem * parent) :
		QSVGInteractiveItem(filename, parent),
		svgOK(new QGraphicsSvgItem(filenameOk, this)),
		svgBad(new QGraphicsSvgItem(filenameBad, this))
	{
		doLayout();
		svgOK->hide();
		svgBad->hide();
	}

	void QSVGTristateItem::doLayout(void)
	{
		if (svgOK->isVisible())
			QGraphicsItemToolkit::centerItem(svgOK);
		if (svgBad->isVisible())
			QGraphicsItemToolkit::centerItem(svgBad);
	}

	void QSVGTristateItem::setState(ItemState state)
	{
		switch (state)
		{
		case Initial:
			svgOK->hide();
			svgBad->hide();
			break;

		case NotValid:
			svgOK->hide();
			svgBad->show();
			break;

		case Valid:
			svgOK->show();
			svgBad->hide();
			break;

		default:
			svgOK->hide();
			svgBad->hide();
		}
	}

	void QSVGTristateItem::reset(void)
	{
		this->setState(Initial);
	}

};