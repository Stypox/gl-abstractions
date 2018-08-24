#ifndef _STYPOX_VAO_H_
#define _STYPOX_VAO_H_

#include <GL/glew.h>
#include <sp/declarations.h>

namespace stypox::gl {
	class Vao {
		GLuint m_id;
		bool m_idGenerated = false;
	public:
		Vao();
		~Vao();

		void generate();
		void remove();

		void attribPointer(GLuint index, GLint sizeOfSet, GLenum type, GLsizei strideOfSet, GLsizei offsetOfSet, GLboolean normalize = GL_FALSE);

		inline void bind() { glBindVertexArray(m_id); }
		inline static void unbind() { glBindVertexArray(0); }
	};
}

#include "vao.cpp"
#endif