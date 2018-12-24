#ifndef _STYPOX_EBO_H_
#define _STYPOX_EBO_H_

#include <glad/glad.h>

namespace stypox::gl {
	class Ebo {
		GLuint m_id;
		bool m_idGenerated = false;
	public:
		Ebo();
		template <class T>
		Ebo(T* pointer, size_t count, GLenum usage);
		~Ebo();

		Ebo(const Ebo& other) = delete;
		Ebo& operator= (const Ebo& other) = delete;
		Ebo(Ebo&& other);
		Ebo& operator= (Ebo&& other);

		void generate();
		void remove();

		template <class T>
		void data(T* pointer, size_t count, GLenum usage) { bind(); glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(T), pointer, usage); }

		inline void bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }
		inline static void unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
	};

	template <class T>
	inline Ebo::Ebo(T* pointer, size_t count, GLenum usage) :
		m_idGenerated{true} {
		glGenBuffers(1, &m_id);
		this->data(pointer, count, usage);
	}
}
#endif