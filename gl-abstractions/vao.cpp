#include "vao.h"

namespace stypox::gl {
	constexpr GLuint toSize(GLenum type) {
		switch (type) {
			case GL_BYTE: return sizeof(GLbyte);
			case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
			case GL_SHORT: return sizeof(GLshort);
			case GL_UNSIGNED_SHORT: return sizeof(GLushort);
			case GL_INT: return sizeof(GLint);
			case GL_UNSIGNED_INT: return sizeof(GLuint);
			case GL_HALF_FLOAT: return sizeof(GLhalf);
			case GL_FLOAT: return sizeof(GLfloat);
			case GL_DOUBLE: return sizeof(GLdouble);
			case GL_FIXED: return sizeof(GLfixed);
			case GL_INT_2_10_10_10_REV: return sizeof(GLint);
			case GL_UNSIGNED_INT_2_10_10_10_REV: return sizeof(GLuint);
			case GL_UNSIGNED_INT_10F_11F_11F_REV: return sizeof(GLuint);
			default: return 0;
		}
	}
	
	Vao::Vao() {}
	Vao::~Vao() {
		remove();
	}

	void Vao::generate() {
		if (m_idGenerated) {
			glDeleteVertexArrays(1, &m_id);
		}
		glGenVertexArrays(1, &m_id);
		m_idGenerated = true;
	}
	void Vao::remove() {
		if (m_idGenerated) {
			glDeleteVertexArrays(1, &m_id);
			m_idGenerated = false;
		}
	}

	void Vao::attribPointer(GLuint index, GLint sizeOfSet, GLenum type, GLsizei strideOfSet, GLsizei offsetOfSet, GLboolean normalize) {
		bind();
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, sizeOfSet, type, normalize, strideOfSet * toSize(type), reinterpret_cast<void*>(offsetOfSet * toSize(type)));
	}
}