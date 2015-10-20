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

#ifndef AREHABFILEMETADATA_H
#define AREHABFILEMETADATA_H

#include <QBitArray>
#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

namespace ARehabUI
{

	class ARehabFileMetadata : public QObject
	{
		Q_OBJECT

		public:
			QString exerciseName;
			QString metadataFileURL;
			QString arehabFileURL;
			QString description;
			QBitArray jointsInvolved;
			unsigned int numRepetitions;
			bool loaded;

			ARehabFileMetadata(QObject * parent = 0);
			virtual ~ARehabFileMetadata(void);
			bool isLoaded(void);
			bool load(const QString &jsonFileURL);
			bool save(const QString &jsonFileURL);
			void clear(void);
	};

};
#endif // AREHABFILEMETADATA_H
