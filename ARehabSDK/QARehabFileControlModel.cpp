
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

#include "QARehabFileControlModel.h"

namespace ARehabUI
{
	QARehabFileControlModel::QARehabFileControlModel(unsigned long long int minValue, unsigned long long maxValue, QObject * parent) :
		QObject(parent),
		minValue(minValue),
		maxValue(maxValue),
		leftValue(minValue),
		rightValue(maxValue)
	{
	}

	QARehabFileControlModel::QARehabFileControlModel(const QARehabFileControlModel &model) :
		QObject(model.parent()),
		minValue(model.minValue),
		maxValue(model.maxValue),
		leftValue(model.leftValue),
		rightValue(model.rightValue)
	{
	}

	QARehabFileControlModel& QARehabFileControlModel::operator= (const QARehabFileControlModel &model)
	{
		if (this != &model)
		{
			minValue = model.minValue;
			maxValue = model.maxValue;
			leftValue = model.leftValue;
			rightValue = model.rightValue;
		}
		return *this;
	}

	QARehabFileControlModel::~QARehabFileControlModel(void)
	{
	}

};