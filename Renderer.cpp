#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
	//Class0310();

}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_ParticleShader = CompileShaders("./Shaders/Particle.vs", "./Shaders/Particle.fs");
	
	//Create VBOs
	CreateVertexBufferObjects();

	if (m_ParticleShader > 0 && m_ParticleVBO > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	//float rect[]
	//	=
	//{
	//	-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
	//	-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	//};

	//glGenBuffers(1, &m_VBORect);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	CreateParticles();
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.";

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);

	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::Class0310()
{
	float vertices[] = { 0, 0, 0, 
						 1, 0, 0, 
						 1, 1, 0 }; //CPU memory

	float vertices_color[] = { 1, 0, 0, 1,
							   0, 1, 0, 1,
							   0, 0, 1, 1 };
	
	glGenBuffers(1,&m_testVBOPos); //get Buffers Object ID
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOPos); //bind to array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //data trandfer cpu -> gpu
																			   //������ ������ ��� �Ѵ�.

	glGenBuffers(2, &m_testVBOcolor); //get Buffers Object ID
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOcolor); //bind to array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);

	float vertices1[] = { -1 ,-1,0,
							0,-1,0,
							0,0,0};
	glGenBuffers(1, &m_testVBOPos1);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOPos1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	float SIZE = 0.1f;
	float particle[] = { SIZE,SIZE,0,
						 -SIZE,SIZE,0,
						-SIZE,-SIZE,0,
						SIZE,SIZE,0,
						-SIZE,-SIZE,0,
						SIZE,-SIZE,0 };

	//glGenBuffers(1, &m_particle);
	//glBindBuffer(GL_ARRAY_BUFFER, m_particle);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(particle), particle, GL_STATIC_DRAW);



}
float g_time = 1;
void Renderer::Class0310_Render()
{
	//Program select
	glUseProgram(m_SolidRectShader);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), 0, 0, 0, 1);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), 1, 0, 0, 1);

	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(m_SolidRectShader, "a_Position");
	
	glEnableVertexAttribArray(attribLoc_Position);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOPos);
	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_Position1 = -1;
	attribLoc_Position1 = glGetAttribLocation(m_SolidRectShader, "a_Position1");

	glEnableVertexAttribArray(attribLoc_Position1);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOPos1);
	glVertexAttribPointer(attribLoc_Position1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_Color = -1;
	attribLoc_Color = glGetAttribLocation(m_SolidRectShader, "a_Color");

	glEnableVertexAttribArray(attribLoc_Color);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOcolor);
	glVertexAttribPointer(attribLoc_Color, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//g_time += 0.00001;
	//if (g_time > 1.f) {
	//	g_time = 0.f;
	//}
	//uniform ����
	int uniformLoc_Scale = -1;
	uniformLoc_Scale = glGetUniformLocation(m_SolidRectShader, "u_Scale");
	glUniform1f(uniformLoc_Scale, g_time);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void Renderer::DrawParticleEffect()
{
	//Program select
	int shaderProgram = m_ParticleShader;
	glUseProgram(shaderProgram);

	int attribLoc_Particle = -1;
	attribLoc_Particle = glGetAttribLocation(shaderProgram, "a_Position");
	glEnableVertexAttribArray(attribLoc_Particle);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(attribLoc_Particle, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glDrawArrays(GL_TRIANGLES, 0, m_ParticleVerticesCount);

}

void Renderer::CreateParticles()
{
	float centerX, centerY;
	centerX = 0.f;
	centerY = 0.f;
	float size = 0.5f;

	int particleCount = 1;
	m_ParticleVerticesCount = particleCount * 6 * 3;
	float* vertices = NULL;
	vertices = new float[m_ParticleVerticesCount];

	int index = 0;
	vertices[index] = centerX - size; ++index;
	vertices[index] = centerY + size; ++index;
	vertices[index] = 0.f; ++index;
	vertices[index] = centerX - size; ++index;
	vertices[index] = centerY - size; ++index;
	vertices[index] = 0.f; ++index;
	vertices[index] = centerX + size; ++index;
	vertices[index] = centerY + size; ++index;
	vertices[index] = 0.f; ++index;

	vertices[index] = centerX + size; ++index;
	vertices[index] = centerY + size; ++index;
	vertices[index] = 0.f; ++index;
	vertices[index] = centerX - size; ++index;
	vertices[index] = centerY - size; ++index;
	vertices[index] = 0.f; ++index;
	vertices[index] = centerX + size; ++index;
	vertices[index] = centerY - size; ++index;
	vertices[index] = 0.f; ++index;

	glGenBuffers(1, &m_ParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_ParticleVerticesCount, vertices, GL_STATIC_DRAW);
}