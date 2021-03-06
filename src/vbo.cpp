#include "../include/stypox/gl/vbo.hpp"

namespace stypox::gl {
	Vbo::Vbo() {}
	Vbo::~Vbo() {
		remove();
	}

	Vbo::Vbo(Vbo&& other) :
		m_id{other.m_id},
		m_idGenerated{other.m_idGenerated} {
		other.m_idGenerated = false;
	}
	Vbo& Vbo::operator= (Vbo&& other) {
		remove();
		if (other.m_idGenerated) {
			m_id = other.m_id;
			m_idGenerated = true;
			other.m_idGenerated = false;
		}
		return *this;
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