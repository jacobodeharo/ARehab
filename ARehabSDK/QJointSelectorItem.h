#ifndef QJOINTSELECTORITEM_H
#define QJOINTSELECTORITEM_H

#include "QSVGInteractiveItem.h"

namespace ARehabUI
{	
	class QJointSelectorItem : public QSVGInteractiveItem
	{
		Q_OBJECT

		public:
			QJointSelectorItem(unsigned int id, const QString filename, QGraphicsItem * parent = 0);
			virtual void reset(void);
			void setState(bool value);
			virtual void setActive(bool enabled);
			virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
			virtual void doLayout(void);
			virtual ~QJointSelectorItem(void);

		private:			
			unsigned int id;
			QGraphicsSvgItem * svgCheck;

		signals:
			void sig_stateChanged(unsigned int, bool);
	};

};

#endif // QJOINTSELECTORITEM_H
