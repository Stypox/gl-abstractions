#include "ebo.h"

namespace stypox::gl {
	Ebo::Ebo() {}
	Ebo::~Ebo() {
		remove();
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