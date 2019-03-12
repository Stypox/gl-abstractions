#ifndef _STYPOX_GL_TEXTURE_HPP_
#define _STYPOX_GL_TEXTURE_HPP_

#include <glad/glad.h>
#include <filesystem>

namespace stypox::gl {
	class Texture2D {
		static std::filesystem::path m_directory;

		GLuint m_id;
		GLint m_position;
		bool m_idGenerated = false;
		bool m_fileOk = true;
	public:
		Texture2D(GLint position = 0);
		Texture2D(GLint position, const std::filesystem::path& filename, GLenum saveFormat, GLint detailLevel = 0, bool relativeToDirectory = true);
		Texture2D(GLint position, const std::filesystem::path& filename, GLenum saveFormat, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter, GLint detailLevel = 0, bool relativeToDirectory = true);
		~Texture2D();

		Texture2D(const Texture2D& other) = delete;
		Texture2D& operator= (const Texture2D& other) = delete;
		Texture2D(Texture2D&& other);
		Texture2D& operator= (Texture2D&& other);

		inline GLint position() { return m_position; }
		inline void setPosition(GLint position) { m_position = position; }
		inline static std::filesystem::path directory() { return m_directory; }
		inline static void setDirectory(const std::filesystem::path& directory) { m_directory = directory; }

		void generate();
		void remove();
		inline bool generated() { return m_idGenerated; }

		void data(const std::filesystem::path& filename, GLenum saveFormat, GLint detailLevel = 0, bool relativeToDirectory = true);
		void data(void * data, GLenum type, GLsizei width, GLsizei height, GLenum sourceFormat, GLenum saveFormat, GLint detailLevel = 0);
		void parameters(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter);

		inline bool fileOk() { return m_fileOk; }
		std::string debugInfo(const std::string& name = "");

		void bind();
		void unbind();
		void bind(GLint position);
		static void unbind(GLint position);
	};
}
#endif