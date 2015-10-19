#include "ARehabFileMetadata.h"

namespace ARehabUI
{

	ARehabFileMetadata::ARehabFileMetadata(QObject *parent) :
	exerciseName(""),
	metadataFileURL(""),
	arehabFileURL(""),
	description(""),
	jointsInvolved(25, false),
	numRepetitions(0),
	loaded(false)
	{
	}

	ARehabFileMetadata::~ARehabFileMetadata(void)
	{
	}

	bool ARehabFileMetadata::isLoaded(void)
	{
		return loaded;
	}

	bool ARehabFileMetadata::load(const QString &jsonFileURL)
	{
		QFile metadataFile(jsonFileURL);
		if (!metadataFile.open(QIODevice::ReadOnly)) {
			qWarning() << "Error abriendo el fichero para guardar." << endl;
			return false;
		}
		metadataFileURL = jsonFileURL;
		QByteArray saveData = metadataFile.readAll();
		QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
		QJsonObject jsonObj = loadDoc.object();

		exerciseName = jsonObj.take("ExerciseName").toString();
		//metadataFileURL = jsonObj["MetadataFileURL"].toString();
		arehabFileURL = jsonObj["ARehabFileURL"].toString();
		description = jsonObj["Descripcion"].toString();
		QJsonObject jointsInv = jsonObj["JointsInvolved"].toObject();
		for (unsigned int i = 0; i < jointsInvolved.size(); ++i)
		{
			jointsInvolved[i] = jointsInv[QString::number(i)].toBool();
		}		
		numRepetitions = jsonObj["NumRepetitions"].toString().toUInt();
		loaded = true;
		return loaded;
	}

	bool ARehabFileMetadata::save(const QString &jsonFileURL)
	{
		QFile metadataFile(jsonFileURL);
		if (!metadataFile.open(QIODevice::WriteOnly)) {
			qWarning() << "Error abriendo el fichero para guardar." << endl;
			return false;
		}
		QJsonObject arehabJsonObject;
		arehabJsonObject.insert("ExerciseName", exerciseName);
		//arehabJsonObject.insert("MetadataFileURL", metadataFileURL);
		arehabJsonObject.insert("ARehabFileURL", arehabFileURL);
		arehabJsonObject.insert("Descripcion", description);
		arehabJsonObject.insert("NumRepetitions", QString::number(numRepetitions));

		QJsonObject jointsInv;
		for (unsigned int i = 0; i < jointsInvolved.size(); ++i)
		{
			jointsInv.insert(QString::number(i), QJsonValue(jointsInvolved[i]));
		}
		arehabJsonObject.insert("JointsInvolved", jointsInv);
		QJsonDocument jsonDocument(arehabJsonObject);
		metadataFile.write(jsonDocument.toJson());

		return true;
	}

	void ARehabFileMetadata::clear(void)
	{
		this->arehabFileURL.clear();
		this->description.clear();
		this->exerciseName.clear();
		this->jointsInvolved = QBitArray(this->jointsInvolved.size(), false);
		this->numRepetitions = 0;
		this->metadataFileURL.clear();
		loaded = false;
	}

};