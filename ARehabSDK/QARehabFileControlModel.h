#ifndef QAREHABFILECONTROLMODEL_H_
#define QAREHABFILECONTROLMODEL_H_

#include <QObject>

namespace ARehabUI
{

	class QARehabFileControlModel : public QObject
	{
		Q_OBJECT

		public:
			QARehabFileControlModel(unsigned long long int minValue, unsigned long long maxValue, QObject * parent = 0);
			QARehabFileControlModel(const QARehabFileControlModel &model);
			QARehabFileControlModel& operator=(const QARehabFileControlModel &model);
			virtual ~QARehabFileControlModel(void);

			unsigned long long int minValue;
			unsigned long long int maxValue;
			unsigned long long int leftValue;
			unsigned long long int rightValue;

		signals :
			void sig_currentMinValueChanged(unsigned long long int);
			void sig_currentMaxValueChanged(unsigned long long int);

	};

};

#endif