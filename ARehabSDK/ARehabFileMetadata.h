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
