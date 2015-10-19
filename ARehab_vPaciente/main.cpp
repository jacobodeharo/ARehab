#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QDir>

#include "ARehab_vPaciente.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Q_INIT_RESOURCE(ARehab_vPaciente);

	QSurfaceFormat format;
	format.setVersion(4, 3);	
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setOption(QSurfaceFormat::DebugContext);
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);	
	format.setSamples(4);
	format.setSwapInterval(0); //Disable VSync
	QSurfaceFormat::setDefaultFormat(format);

	ARehabGUIPlayer::ARehabMainWindow w;
	app.setStyleSheet("QToolTip { font-size: 18px; color: #dddddd; background-color: #444444; border: none; }");	
	w.show();
	return (app.exec());
}
