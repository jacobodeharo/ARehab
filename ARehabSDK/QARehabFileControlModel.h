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