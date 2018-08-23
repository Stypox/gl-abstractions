#include "ebo.h"

namespace stypox::gl {
	Ebo::Ebo() {}
	Ebo::~Ebo() {
		glDeleteBuffers(1, &m_id);
	}

	void Ebo::generate() {
		if (m_idGenerated) {
			glDeleteBuffers(1, &m_id);
		}
		glGenBuffers(1, &m_id);
		m_idGenerated = true;
	}
	void Ebo::remove() {
		if (m_idGenerated) {
			glDeleteBuffers(1, &m_id);
			m_idGenerated = false;
		}
	}
}