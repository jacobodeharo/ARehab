#ifndef QSVGTRISTATEITEM_H
#define QSVGTRISTATEITEM_H

#include "QSVGInteractiveItem.h"

namespace ARehabUI
{
	class QSVGTristateItem : public QSVGInteractiveItem
	{
		Q_OBJECT

	public:
		static enum ItemState { Initial, NotValid, Valid };
		QSVGTristateItem(const QString filename, const QString filenameOk, const QString filenameBad, QGraphicsItem * parent = 0);
		virtual void doLayout(void);
		void setState(ItemState state);
		virtual void reset(void);

	private:
		QGraphicsSvgItem * svgOK;
		QGraphicsSvgItem * svgBad;
	};
};

#endif // QSVGTRISTATEITEM_H
