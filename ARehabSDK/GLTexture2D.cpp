#include "GLTexture2D.h"


namespace ARehabGL
{

	GLTexture2D::GLTexture2D(QOpenGLFunctions_4_3_Core * gl, int width, int height, unsigned int format, unsigned int type) :
		GLTexture(gl)
	{
		this->width = width;
		this->height = height;
		this->format = format;

		gl->glBindTexture(GL_TEXTURE_2D, textureHandle);
		//gl->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
		gl->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, 0);

		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//gl->glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		gl->glBindTexture(GL_TEXTURE_2D, 0);
	}


	GLTexture2D::GLTexture2D(QOpenGLFunctions_4_3_Core * gl, uchar * buff, int width, int height, unsigned int format ) :
		GLTexture(gl)
	{
		this->width = width;
		this->height = height;
		this->format = format;

		// TODO: Add error handling.
		if (!buff)
			return;
		gl->glBindTexture(GL_TEXTURE_2D, textureHandle);
		gl->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buff);

		//gl->glGenerateMipmap(GL_TEXTURE_2D);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//gl->glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		gl->glBindTexture(GL_TEXTURE_2D, 0);
	}

	GLTexture2D::GLTexture2D(QOpenGLFunctions_4_3_Core * gl, const QString& fileName) throw(GLTextureException) :
		GLTexture(gl)
	{
		this->width = width;
		this->height = height;
		this->format = GL_RGBA;

		QImage image(fileName);
		//image = image.mirrored(false, true);
		
		if (image.isNull()) {
			m_failed = true;
			throw GLTextureException("GLTexture2D > Error: image is null.");
			return;
		}

		image = image.convertToFormat(QImage::Format_RGBA8888);
		
		gl->glBindTexture(GL_TEXTURE_2D, textureHandle);
		
		gl->glTexImage2D(GL_TEXTURE_2D, 0, this->format, image.width(), image.height(), 0, this->format, GL_UNSIGNED_BYTE, image.bits());		

		gl->glGenerateMipmap(GL_TEXTURE_2D);
		//gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		//gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		gl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gl->glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		gl->glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::bind(void)
	{
		gl->glBindTexture(GL_TEXTURE_2D, textureHandle);		
	}

	void GLTexture2D::unbind(void)
	{
		gl->glBindTexture(GL_TEXTURE_2D, 0);		
	}

	void GLTexture2D::update(uchar * buff, int width, int height, unsigned int format) {
		if (!buff)
			return;
		gl->glBindTexture(GL_TEXTURE_2D, textureHandle);
		gl->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height, format, GL_UNSIGNED_BYTE, (GLvoid*)buff);		
		gl->glBindTexture(GL_TEXTURE_2D, 0);
	}
};