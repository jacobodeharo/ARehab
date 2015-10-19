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