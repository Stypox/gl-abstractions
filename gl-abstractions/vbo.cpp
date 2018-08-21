#include "vbo.h"

namespace stypox::gl {
	Vbo::Vbo(bool generate, bool bind) {
		if (generate) {
			glGenBuffers(1, &m_id);
			if (bind)
				glBindBuffer(GL_ARRAY_BUFFER, m_id);
		}
	}
	Vbo::~Vbo() {
		glDeleteBuffers(1, &m_id);
	}

	void Vbo::generate() {
		if (m_idGenerated) {
			glDeleteBuffers(1, &m_id);
		}
		glGenBuffers(1, &m_id);
		m_idGenerated = true;
	}
	void Vbo::remove() {
		if (m_idGenerated) {
			glDeleteBuffers(1, &m_id);
			m_idGenerated = false;
		}
	}

	void Vbo::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}
	void Vbo::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}