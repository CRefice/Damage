#include <gl\core-3-3.h>
#ifdef _WIN32
#include <gl\glext.h>
#include <gl\wglext.h>
#endif

#include <Gooey\Include\GLWindow.hpp>

#include "ogl-backend.hpp"

namespace video
{
uint32 OGLBackend::videoColor(uint16 color) const {
	if(!cgb()) {
		static const uint32 palette[4] = {
			0xFF'FF'FF'FF, //White
			0xFF'AA'AA'AA, //Light Gray
			0xFF'55'55'55, //Dark Gray
			0xFF'00'00'00  //Black
		};
		return palette[color & 3];
	}
	else {
		//TODO: find a better way to normalize color
		uint R = color.bits(0, 4) * 8;
		uint G = color.bits(5, 9) * 8;
		uint B = color.bits(10, 14) * 8;

		return (0xFF << 24) | (B << 16) | (G << 8) | (R << 0);
	}
}

void OGLBackend::init(void* windowPtr) {
	window = (goo::GLWindow*)windowPtr;
	ogl_LoadFunctions();
#ifdef _WIN32
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	const char *extensions = (char*)glGetString(GL_EXTENSIONS);

	if (strstr(extensions, "WGL_EXT_swap_control") == 0)
	{
		return;
	}
	else
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");

		if (wglSwapIntervalEXT)
			wglSwapIntervalEXT(1);
	}
#endif	

	static const GLfloat vertexData[] = {
		 1.0f,  1.0f, 1.0f, 0.0f,	// Top Right
		 1.0f, -1.0f, 1.0f, 1.0f,	// Bottom Right
		-1.0f, -1.0f, 0.0f, 1.0f,	// Bottom Left
		-1.0f,  1.0f, 0.0f, 0.0f	// Top Left 
	};
	static const GLuint indices[] = {
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (GLvoid*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	const GLchar* vertexSource = R"(
		#version 330 core
		layout (location = 0) in vec2 position;
		layout (location = 1) in vec2 texCoord;

		out vec2 TexCoord;

		void main() {
			gl_Position = vec4(position, 0.0, 1.0);
			TexCoord = texCoord;
		})";

	const GLchar* fragmentSource = R"(
		#version 330 core
		in vec2 TexCoord;
		uniform sampler2D ourTexture;

		out vec4 color;

		void main() {
			color = texture(ourTexture, TexCoord);
		})";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void OGLBackend::draw(const uint16* buffer, uint width, uint height) {
	if(width != texWidth || height != texHeight) {
		delete[] texData;
		texData = new uint32_t[width * height];

		texWidth = width;
		texHeight = height;

		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glUseProgram(program);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
				0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	}

	//Translate from GB to OpenGL pixel data
	for (uint n = 0; n < 160 * 144; ++n) {
		texData[n] = videoColor(buffer[n]);
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
			GL_RGBA, GL_UNSIGNED_BYTE, texData);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	window->swapBuffers();
}

void OGLBackend::term() {
	delete[] texData;

	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(program);
}
}
