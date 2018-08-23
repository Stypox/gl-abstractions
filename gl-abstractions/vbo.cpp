#include "vbo.h"

namespace stypox::gl {
	Vbo::Vbo() {}
	Vbo::~Vbo() {
		remove();
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
}