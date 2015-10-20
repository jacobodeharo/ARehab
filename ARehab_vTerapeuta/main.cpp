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

#include "ARehab_vTerapeuta.h"

#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QList>
#include <QByteArray>
#include <QString>
#include <QDir>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Q_INIT_RESOURCE(ARehab_vTerapeuta);

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

	ARehabGUIDesigner::ARehabMainWindow w;	
	
	QFile styleFile(":/styles/main.qss");
	styleFile.open(QFile::ReadOnly);	
	app.setStyleSheet(QLatin1String(styleFile.readAll()));
	styleFile.close();

	w.show();
	return (app.exec());
}
