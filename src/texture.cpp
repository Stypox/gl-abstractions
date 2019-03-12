#include "../include/stypox/gl/texture.hpp"

#include <SOIL.h>

namespace stypox::gl {
	using Tstr = std::string;
	using Tpath = std::filesystem::path;

	constexpr GLenum channelsToFormat(int nrChannels) {
		switch (nrChannels) {
			case 1: return GL_RED;
			case 2: return GL_RG;
			case 3: return GL_RGB;
			case 4: return GL_RGBA;
			default: return 0;
		}
	}

	Tpath Texture2D::m_directory {""};

	Texture2D::Texture2D(GLint position) :
		m_position{position} {}
	Texture2D::Texture2D(GLint position, const Tpath& filename, GLenum saveFormat, GLint detailLevel, bool relativeToDirectory) :
		m_position{position} {
		generate();
		data(filename, saveFormat, detailLevel, relativeToDirectory);
	}
	Texture2D::Texture2D(GLint position, const Tpath& filename, GLenum saveFormat, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter, GLint detailLevel, bool relativeToDirectory) :
		m_position{position} {
		generate();
		data(filename, saveFormat, detailLevel, relativeToDirectory);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	}
	Texture2D::~Texture2D() {
		remove();
	}


	Texture2D::Texture2D(Texture2D&& other) :
		m_id{other.m_id},
		m_position{other.m_position},
		m_idGenerated{other.m_idGenerated},
		m_fileOk{other.m_fileOk} {
		other.m_idGenerated = false;
	}
	Texture2D& Texture2D::operator= (Texture2D&& other) {
		m_position = other.m_position;
		m_fileOk = other.m_fileOk;
		remove();
		if (other.m_idGenerated) {
			m_id = other.m_id;
			m_idGenerated = true;
			other.m_idGenerated = false;
		}
		return *this;
	}

	void Texture2D::generate() {
		if (m_idGenerated) {
			glDeleteTextures(1, &m_id);
		}
		glGenTextures(1, &m_id);
		m_idGenerated = true;
	}
	void Texture2D::remove() {
		if (m_idGenerated) {
			glDeleteTextures(1, &m_id);
			m_idGenerated = false;
		}
	}

	void Texture2D::data(const Tpath& filename, GLenum saveFormat, GLint detailLevel, bool relativeToDirectory) {
		bind();
		int width, height, nrChannels;
		unsigned char* pixels = SOIL_load_image((relativeToDirectory ? m_directory / filename : filename).c_str(), &width, &height, &nrChannels, SOIL_LOAD_AUTO);
		if (pixels) {
			m_fileOk = true;
			glTexImage2D(GL_TEXTURE_2D, detailLevel, saveFormat, width, height, 0, channelsToFormat(nrChannels), GL_UNSIGNED_BYTE, pixels);
			SOIL_free_image_data(pixels);
		}
		else {
			m_fileOk = false;
		}
	}
	void Texture2D::data(void * pixels, GLenum type, GLsizei width, GLsizei height, GLenum sourceFormat, GLenum saveFormat, GLint detailLevel) {
		bind();
		glTexImage2D(GL_TEXTURE_2D, detailLevel, saveFormat, width, height, 0, sourceFormat, type, pixels);
	}
	void Texture2D::parameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) {
		bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	}

	Tstr Texture2D::debugInfo(const Tstr& name) {
		return "Texture status id=" + std::to_string(m_id) +
			",position=" + std::to_string(m_position) +
			(name.empty() ? "" : ",name=" + name) +
			(m_fileOk ? ": ok" : ": file error");
	}

	void Texture2D::bind() {
		glActiveTexture(GL_TEXTURE0 + m_position);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
	void Texture2D::unbind() {
		glActiveTexture(GL_TEXTURE0 + m_position);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture2D::bind(GLint position) {
		glActiveTexture(GL_TEXTURE0 + position);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
	void Texture2D::unbind(GLint position) {
		glActiveTexture(GL_TEXTURE0 + position);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}