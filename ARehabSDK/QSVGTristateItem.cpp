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