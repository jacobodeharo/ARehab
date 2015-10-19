#include "ARehabMainWindow.h"
#include "GeneratedFiles/qrc_ARehab.cpp"

#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QDir>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Q_INIT_RESOURCE(ARehab);

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

	ARehabUI::ARehabMainWindow w(&app);
	app.setStyleSheet("QToolTip { font-size: 18px; color: #dddddd; background-color: #444444; border: none; }");
	//app.setStyleSheet(" *{color:lightgrey; background-color: darkgrey;}");
	w.show();
	return (app.exec());
}
