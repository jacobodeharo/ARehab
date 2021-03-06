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

#include "qglviewer.h"

namespace ARehabUI
{

	QGLViewer::QGLViewer(QWidget * parent) :
		QOpenGLWidget(parent),
		framebuffer(0),
		textureID(0),
		trackedJoints(GLVBOBody::maxJoints, false),
		fbo(0),
		aspectRatio(1920.0f / 1080.0f),
		expectedAspectRatio(1920.0f / 1080.0f),
		scaleP(1.0f),
		logger(0),
		defaultShader(0),
		shaderProgVideo(0),
		shaderProgBodyJoints(0),
		shaderYUY2toRGB(0),
		textureVideo(0),
		textureInitial(0),
		vboVideo(0),
		vboBody(0),
		vboBody2(0),
		aspectCorrection(glm::scale(glm::vec3(1.0f, 1.0f, 1.0f))),
		model(glm::scale(glm::vec3(1.0f, 1.0f, 1.0f))),
		view(glm::scale(glm::vec3(1.0f, 1.0f, 1.0f))),
		projection(glm::scale(glm::vec3(1.0f, 1.0f, 1.0f))),
		yuv2rgba(glm::scale(glm::vec3(1.0f, 1.0f, 1.0f)))
	{
		this->connectInternals();
	}

	QString QGLViewer::getOpenGLInfo(void)
	{
		const GLubyte * renderer = glGetString(GL_RENDERER);
		const GLubyte * vendor = glGetString(GL_VENDOR);
		const GLubyte * version = glGetString(GL_VERSION);
		const GLubyte * glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		QString rendererString(QString("GL renderer: <strong>") + reinterpret_cast<const char*>(renderer)+"</strong>");
		QString vendorString(QString("GL vendor: <strong>") + reinterpret_cast<const char*>(vendor)+"</strong>");
		QString versionString(QString("GL version: <strong>") + reinterpret_cast<const char*>(version)+"</strong>");
		QString glslVersionString(QString("GLSL version: <strong>") + reinterpret_cast<const char*>(glslVersion)+"</strong>");
		QString majorMinor(QString("Version: <strong>") + QString::number(major) + "." + QString::number(minor) + "</strong>");

		return (rendererString + "<br>" + vendorString + "<br>" + versionString + "<br>" + glslVersionString + "<br>" + majorMinor);
	}

	QString QGLViewer::getOpenGLExtensions(void) {

		QString extensions;
		GLint nExtensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
		for (GLuint i = 0; i < nExtensions; ++i) {

			const GLubyte * ext = gl->glGetStringi(GL_EXTENSIONS, i);
			extensions = extensions + "<br>" + QString(reinterpret_cast<const char*>(ext));
		}
		return extensions;
	}

	void QGLViewer::slot_LoggedGLMessage(const QOpenGLDebugMessage &msg) {

		qDebug() << endl << "================================" << endl << endl << " GL Error: " << msg.id() << endl;
		qDebug() << " Message: " << msg.message() << endl;

		QString severity;
		switch (msg.severity())
		{
		case QOpenGLDebugMessage::InvalidSeverity:
			severity = "Invalid";
			break;

		case QOpenGLDebugMessage::HighSeverity:
			severity = "High";
			break;

		case QOpenGLDebugMessage::MediumSeverity:
			severity = "Medium";
			break;

		case QOpenGLDebugMessage::LowSeverity:
			severity = "Low";
			break;

		case QOpenGLDebugMessage::AnySeverity:
			severity = "Any";
			break;

		case QOpenGLDebugMessage::NotificationSeverity:
			severity = "Notification";
			break;
		}
		qDebug() << " Severity: " << severity << endl;


		QString source;
		switch (msg.source())
		{
		case QOpenGLDebugMessage::InvalidSource:
			source = "The source of the message is invalid; this is the source of a default-constructed QOpenGLDebugMessage object.";
			break;

		case QOpenGLDebugMessage::APISource:
			source = "The message was generated in response to OpenGL API calls.";
			break;

		case QOpenGLDebugMessage::WindowSystemSource:
			source = "The message was generated by the window system.";
			break;

		case QOpenGLDebugMessage::ShaderCompilerSource:
			source = "The message was generated by the shader compiler.";
			break;

		case QOpenGLDebugMessage::ThirdPartySource:
			source = "The message was generated by a third party, for instance an OpenGL framework a or debugging toolkit.";
			break;

		case QOpenGLDebugMessage::ApplicationSource:
			source = "The message was generated by the application itself.";
			break;

		case QOpenGLDebugMessage::OtherSource:
			source = "The message was generated by a source not included in this enumeration.";
			break;

		case QOpenGLDebugMessage::AnySource:
			source = "This value corresponds to a mask of all possible message sources.";
			break;
		}
		qDebug() << " Source:  " << source << endl;
	}

	void QGLViewer::slot_newARehabFrame(ARehabFrame * frame)
	{
		if (NULL != frame)
		{
			if (frame->color)
				this->updateTexture(frame->color);
			if (frame->body)
				this->updateBodyJoints(frame->body);
			frame->freeMem();
		}
	}

	void QGLViewer::slot_newBodyFrame(BodyFrame * frame)
	{
		if (NULL != frame)
		{
			this->updateBodyJoints(frame);
		}
	}

	void QGLViewer::slot_newColorFrame(ColorFrame * frame)
	{
		if (NULL != frame)
		{
			this->updateTexture(frame);
		}
	}

	void QGLViewer::slot_openGLinitialized(void)
	{
		qDebug() << "OpenGL Initialized" << endl;

		connect(&this->refreshGL, SIGNAL(timeout()), this, SLOT(update()));
	}

	void QGLViewer::slot_startAnimation(void)
	{
		if (!this->refreshGL.isActive())
			this->refreshGL.start(30);
	}

	void QGLViewer::slot_pauseAnimation(void)
	{
		if (this->refreshGL.isActive())
			this->refreshGL.stop();
	}

	void QGLViewer::slot_updateTrackedJoints(const QBitArray &tracked)
	{
		this->trackedJoints = tracked;
		if (this->vboBody)
		{
			this->updateJointsTracked(this->trackedJoints);
		}
	}

	void QGLViewer::enableOpenGLDebuging(bool on) {

		this->makeCurrent();
		if (on && !this->logger) {
			QOpenGLContext * ctx = QOpenGLContext::currentContext();
			if (ctx)
			{
				this->logger = new QOpenGLDebugLogger(this);
				if (ctx->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
				{
					qDebug() << "Extension GL_KHR_debug OK" << endl;
					if (logger->initialize())  // initializes in the current context, i.e. ctx
					{
						connect(logger, &QOpenGLDebugLogger::messageLogged, this, &QGLViewer::slot_LoggedGLMessage);
						logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
						qDebug() << endl << "> Debug ON" << endl;
					}
				}
				else{
					qDebug() << "Extension GL_KHR_debug not available. It is needed for Debugging Qt OpenGL." << endl;
				}
			}
			else{
				qDebug() << "OpenGL current context not loaded." << endl;
			}
		}
		else if (this->logger) {
			this->logger->stopLogging();
			this->logger->disconnect();
			delete this->logger;
			this->logger = 0;
		}
	}

	void QGLViewer::connectInternals(void)
	{
		connect(this, &QGLViewer::openGLinitialized, this, &QGLViewer::slot_openGLinitialized);
	}

	void QGLViewer::initializeGL(void)
	{

		this->gl = GLFunctions::setupOpenGLFunctionsPtr(QOpenGLContext::currentContext());
		this->enableOpenGLDebuging(true);

		gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		gl->glEnable(GL_BLEND);

		gl->glEnable(GL_POLYGON_SMOOTH);
		gl->glEnable(GL_LINE_SMOOTH);
		gl->glPointSize(14);

		// tell GL to only draw onto a pixel if the shape is closer to the viewer
		gl->glEnable(GL_DEPTH_TEST); // enable depth-testing
		gl->glViewport(0, 0, this->width(), this->height());
		gl->glClearColor(0.9f, 0.9f, 0.9f, 1.0f); //Scene background color

		this->prepareShaderPrograms();
		this->prepareTextures();
		this->prepareVertexBuffers();
		this->defaultShader->use();
		emit openGLinitialized();
	}

	void QGLViewer::prepareShaderPrograms(void)
	{
		try {
			defaultShader = new GLShader(this->gl);
			defaultShader->compile("../ARehabSDK/shaders/default.vert");
			defaultShader->compile("../ARehabSDK/shaders/default.frag");
			defaultShader->link();
			defaultShader->validate();

			shaderProgVideo = new GLShader(this->gl);
			shaderProgVideo->compile("../ARehabSDK/shaders/video.vert");
			shaderProgVideo->compile("../ARehabSDK/shaders/video.frag");
			shaderProgVideo->link();
			shaderProgVideo->validate();

			shaderProgBodyJoints = new GLShader(this->gl);
			shaderProgBodyJoints->compile("../ARehabSDK/shaders/joints.vert");
			shaderProgBodyJoints->compile("../ARehabSDK/shaders/joints.frag");
			shaderProgBodyJoints->link();
			shaderProgBodyJoints->validate();
		}
		catch (GLShaderException & ex) {
			qDebug() << ex.what() << endl;
		}
	}

	void QGLViewer::prepareVertexBuffers(void)
	{	
		if (!this->vboVideo)
		{
			this->vboVideo = new GLVBOVideo(this->gl);			
		}

		if (!this->vboBody)
		{
			this->vboBody = new GLVBOBody(this->gl);	
			this->updateJointsTracked(this->trackedJoints);
		}
	}

	void QGLViewer::updateBodyJoints(BodyFrame * body)
	{
		if (NULL != body)
		{
			this->vboBody->updateJoints(body->joints2D, body->jointOrientations, body->kinectTrackedState, body->numJoints);
		}
	}

	void QGLViewer::updateTexture(ColorFrame * color)
	{
		if (NULL != color)
		{
			if (color->colorBufferRGBX)
			{
				this->textureVideo->update(reinterpret_cast<unsigned char*>(color->colorBufferRGBX), color->colorWidth, color->colorHeight);
			}
			else
			{
				this->textureVideo->update(reinterpret_cast<unsigned char*>(color->colorBufferYUY2), color->colorWidth, color->colorHeight, GL_RGBA);
			}
		}
	}

	void QGLViewer::updateJointsTracked(const QBitArray &trackedJoints)
	{		
		unsigned int numTracked = 0;		
		unsigned int tracked[25];
		memset(tracked, 0, sizeof(unsigned int) * 25);

		for (unsigned int i = 0; i < trackedJoints.size(); ++i)
		{
			if (trackedJoints.testBit(i))
			{
				tracked[numTracked] = i;
				numTracked++;
			}			
		}
		if (this->vboBody)
			this->vboBody->updateTrackedJoints(tracked, numTracked);
	}

	void QGLViewer::prepareTextures(void)
	{
		gl->glEnable(GL_TEXTURE1);
		try {
			this->textureVideo = new GLTexture2D(this->gl, 1920 / 2, 1080, GL_RGBA, GL_UNSIGNED_BYTE);
			this->textureInitial = new GLTexture2D(this->gl, ":/images/VideoInit.png");
		}
		catch (GLTextureException & ex) {
			qDebug() << ex.what() << endl;
		}
	}

	// Generates a texture that is suited for attachments to a framebuffer
	GLuint QGLViewer::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
	{
		this->makeCurrent();
		//Generate texture ID and load texture data 	
		gl->glGenTextures(1, &textureID);
		gl->glBindTexture(GL_TEXTURE_2D, textureID);
		gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1920, 1080, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl->glBindTexture(GL_TEXTURE_2D, 0);

		return textureID;
	}

	void QGLViewer::prepareFBOs(void)
	{
		this->makeCurrent();

		//Framebufers
		gl->glGenFramebuffers(1, &framebuffer);
		gl->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// Create a color attachment texture
		GLuint textureColorbuffer = generateAttachmentTexture(false, false);
		gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
		// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

		// Now that we created the framebuffer and added all attachments we want to check if it is actually complete now
		if (gl->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
		}
		gl->glBindFramebuffer(GL_FRAMEBUFFER, this->defaultFramebufferObject());
	}

	void QGLViewer::resizeGL(int w, int h)
	{
		GLfloat width = static_cast<float>(w),
			height = static_cast<float>(h);

		gl->glViewport(0, 0, width, height);

		expectedAspectRatio = 1920.0f / 1080.0f;
		aspectRatio = width / height;
		scaleP = expectedAspectRatio / aspectRatio;
		aspectCorrection = glm::scale(glm::vec3(1.0f / aspectRatio, 1.0f, 1.0f));

		GLfloat middleX = 1920.0f / 2.0f, middleY = 1080.0f / 2.0f;
		if (aspectRatio >= expectedAspectRatio)
		{
			model = glm::scale(glm::vec3(scaleP, 1.0f, 1.0f)) * glm::translate(glm::vec3(-middleX, -middleY, 0.0f)); //Center video frames on viewport and preserve aspectratio
		}
		else {
			model = glm::scale(glm::vec3(1.0f, 1.0f / scaleP, 1.0f)) *glm::translate(glm::vec3(-middleX, -middleY, 0.0f)); //Center video frames on viewport and preserve aspectratio
		}
		view = glm::translate(glm::vec3(0.0f, 0.0f, -0.01f));
		projection = glm::ortho(-middleX, middleX, middleY, -middleY, 0.001f, 1000.0f);
	}

	void QGLViewer::paintGL(void)
	{
		this->makeCurrent();
		gl->glDisable(GL_DEPTH_TEST);
		gl->glClear(GL_COLOR_BUFFER_BIT);

		if (this->refreshGL.isActive())
		{
			this->shaderProgVideo->use();
			this->shaderProgVideo->setUniform("model", model);
			this->shaderProgVideo->setUniform("view", view);
			this->shaderProgVideo->setUniform("projection", projection);
			this->shaderProgVideo->setUniform("dimH", static_cast<float>(this->width()));
			if (this->textureVideo)
			{
				this->textureVideo->bind();
			}
			if (this->vboVideo)
			{
				this->vboVideo->render();
			}
			if (this->textureVideo)
			{
				this->textureVideo->unbind();
			}
			this->shaderProgBodyJoints->use();
			this->shaderProgBodyJoints->setUniform("model", model);
			this->shaderProgBodyJoints->setUniform("view", view);
			this->shaderProgBodyJoints->setUniform("projection", projection);
			this->shaderProgBodyJoints->setUniform("correction", aspectCorrection);
			if (this->vboBody)
			{
				this->vboBody->render();
			}
		}
		else {
			this->defaultShader->use();
			this->defaultShader->setUniform("model", model);
			this->defaultShader->setUniform("view", view);
			this->defaultShader->setUniform("projection", projection);
			this->defaultShader->setUniform("dimH", static_cast<float>(this->width()));
			if (this->textureInitial)
			{
				this->textureInitial->bind();
				if (this->vboVideo)
				{
					this->vboVideo->render();
				}
				this->textureInitial->unbind();
			}
		}
	}

	QGLViewer::~QGLViewer(void)
	{
		this->makeCurrent();
		if (this->fbo)
		{
			delete this->fbo;
			this->fbo = 0;
		}

		if (this->textureVideo)
		{
			delete this->textureVideo;
			this->textureVideo = 0;
		}

		if (this->vboBody)
		{
			delete this->vboBody;
			this->vboBody = 0;
		}

		if (this->vboVideo)
		{
			delete this->vboVideo;
			this->vboVideo = 0;
		}

		if (this->shaderProgVideo)
		{
			delete this->shaderProgVideo;
			this->shaderProgVideo = 0;
		}

		if (this->shaderProgBodyJoints)
		{
			delete this->shaderProgBodyJoints;
			this->shaderProgBodyJoints = 0;
		}
		this->doneCurrent();
	}

}