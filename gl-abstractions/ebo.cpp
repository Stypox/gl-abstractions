#include "ebo.h"

namespace stypox::gl {
	Ebo::Ebo() {}
	Ebo::~Ebo() {
		remove();
	}

	Ebo::Ebo(Ebo&& other) :
		m_id{other.m_id},
		m_idGenerated{other.m_idGenerated} {
		other.m_idGenerated = false;
	}
	Ebo& Ebo::operator= (Ebo&& other) {
		remove();
		if (other.m_idGenerated) {
			m_id = other.m_id;
			m_idGenerated = true;
			other.m_idGenerated = false;
		}
		return *this;
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