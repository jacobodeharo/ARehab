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

#ifndef QGLVIEWER_H
#define QGLVIEWER_H

#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QElapsedTimer>
#include <QtConcurrent>
#include <QOpenGLWidget>
#include <QOpenGLDebugLogger>
#include <QBitArray>

#include "Toolkit.h"
#include "GLShader.h"
#include "GLVBOVideo.h"
#include "GLVBOBody.h"
#include "GLFBO.h"
#include "ARehabFrame.h"

using namespace ARehabControl;

namespace ARehabUI
{
	class QGLViewer : public QOpenGLWidget, public GLFunctions
	{
		Q_OBJECT

	public:		
		QGLViewer(QWidget * parent = 0);
		QString getOpenGLInfo(void);
		QString getOpenGLExtensions(void);
		void initializeGL(void);
		void prepareShaderPrograms(void);
		void prepareVertexBuffers(void);
		void prepareTextures(void);
		GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);
		void prepareFBOs(void);
		void updateBodyJoints(BodyFrame * body);
		void updateTexture(ColorFrame * color);
		void updateJointsTracked(const QBitArray &trackedJoints);
		void resizeGL(int w, int h);
		void paintGL(void);
		virtual ~QGLViewer(void);
		void enableOpenGLDebuging(bool);
		void connectInternals(void);

	signals:
		void openGLinitialized(void);

		public slots:
		void slot_LoggedGLMessage(const QOpenGLDebugMessage &msg);
		void slot_newARehabFrame(ARehabFrame * frame);
		void slot_newBodyFrame(BodyFrame * body);
		void slot_newColorFrame(ColorFrame * img);
		void slot_openGLinitialized(void);
		void slot_startAnimation(void);
		void slot_pauseAnimation(void);
		void slot_updateTrackedJoints(const QBitArray &trackedJoints);

	private:
		GLuint framebuffer;
		GLuint textureID;
		QBitArray trackedJoints;
		QTimer refreshGL;
		GLFBO * fbo;
		QOpenGLDebugLogger * logger;
		GLShader * defaultShader;
		GLShader * shaderProgVideo;
		GLShader * shaderProgBodyJoints;
		GLShader * shaderYUY2toRGB;
		GLTexture2D * textureVideo;
		GLTexture2D * textureInitial;
		GLVBOVideo * vboVideo; //Vertex for the video frame rectangle
		GLVBOBody * vboBody; //Joints positions and colors
		GLVBOBody * vboBody2; //Joints for the second corpse, if it is needed
		float aspectRatio;
		float expectedAspectRatio;
		float scaleP;
		glm::mat4 aspectCorrection; //Aspect ratio aspectCorrection
		glm::mat4 model; //Model matrix
		glm::mat4 view; //View matrix
		glm::mat4 projection;
		glm::mat4 yuv2rgba; //Color conversion matrix
	};

};

#endif // QGLVIEWER_H
