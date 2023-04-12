#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
	Class0310();
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
	m_FragmentSandboxShader = CompileShaders("./Shaders/FragmentSandbox.vs", "./Shaders/FragmentSandbox.fs");


	//Create VBOs
	CreateVertexBufferObjects();

	if (m_SolidRectShader > 0 && m_VBORect > 0)
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
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float vertices[] = { 0, 0, 0,
						 1, 0, 0,
						 1, 1, 0 }; //CPU memory

	glGenBuffers(1, &m_testVBO); //get Buffer Object ID
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO); //bind to array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //data transfer from CPU to GPU

	float vertices1[] = { -1, -1, 0,
						   0, -1, 0,
						   0, 0, 0 }; //CPU memory

	glGenBuffers(1, &m_testVBO1); //get Buffer Object ID
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1); //bind to array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW); //data transfer from CPU to GPU

	float verticesColor[] = { 1, 0, 0, 1,
							  0, 1, 0, 1,
							  0, 0, 1, 1 }; //CPU memory

	glGenBuffers(1, &m_testVBOColor); //get Buffer Object ID
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor); //bind to array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColor), verticesColor, GL_STATIC_DRAW); //data transfer from CPU to GPU



	/*float* temp;
	int size = 400000000000000;
	temp = new float[size];
	memset(temp, 1, sizeof(float)*size);

	glBindBuffer(GL_ARRAY_BUFFER, testVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size, temp, GL_STATIC_DRAW);

	std::cout << "asdf" << std::endl;*/

	float rect1[]
		=
	{
		 -1.f , -1.f , 0.f,			0.f, 1.f   // x, y, z , tx, ty
		,-1.f ,  1.f , 0.f,			0.f, 0.f
		, 1.f ,  1.f , 0.f,			1.f, 0.f
		,-1.f , -1.f , 0.f,			0.f, 1.f
		, 1.f ,  1.f , 0.f,			1.f, 0.f
		, 1.f , -1.f , 0.f,			1.f, 1.f
	};

	glGenBuffers(1, &m_FragmentSandboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_FragmentSandboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect1), rect1, GL_STATIC_DRAW);

	CreateParticles(10000);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string* target)
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
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
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

float g_time = 0.f;

void Renderer::Class0310_Render()
{
	//Program select
	glUseProgram(m_SolidRectShader);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), 0, 0, 0, 1);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), 1, 0, 0, 1);

	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribLoc_Position);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);
	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_Position1 = -1;
	attribLoc_Position1 = glGetAttribLocation(m_SolidRectShader, "a_Position1");
	glEnableVertexAttribArray(attribLoc_Position1);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBO1);
	glVertexAttribPointer(attribLoc_Position1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_Color = -1;
	attribLoc_Color = glGetAttribLocation(m_SolidRectShader, "a_Color");
	glEnableVertexAttribArray(attribLoc_Color);
	glBindBuffer(GL_ARRAY_BUFFER, m_testVBOColor);
	glVertexAttribPointer(attribLoc_Color, 4, GL_FLOAT, GL_FALSE, 0, 0);

	int unifromLoc_Scale = -1;
	unifromLoc_Scale = glGetUniformLocation(m_SolidRectShader, "u_Scale");
	glUniform1f(unifromLoc_Scale, g_time);
	//g_time += 0.016;
	//if (g_time > 1.f)
	//	g_time = 0.f;

	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void Renderer::DrawParticleEffect()
{
	//Program select
	int program = m_ParticleShader;
	glUseProgram(program);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//int attribLoc_Position = -1;
	//attribLoc_Position = glGetAttribLocation(program, "a_Position");
	//glEnableVertexAttribArray(attribLoc_Position);
	//glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	//glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//int attribLoc_Color = -1;
	//attribLoc_Color = glGetAttribLocation(program, "a_Color");
	//glEnableVertexAttribArray(attribLoc_Color);
	//glBindBuffer(GL_ARRAY_BUFFER, m_ParticleColorVBO);
	//glVertexAttribPointer(attribLoc_Color, 4, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_Position = -1;
	attribLoc_Position = glGetAttribLocation(program, "a_Position");
	glEnableVertexAttribArray(attribLoc_Position);


	int attribLoc_Color = -1;
	attribLoc_Color = glGetAttribLocation(program, "a_Color");
	glEnableVertexAttribArray(attribLoc_Color);

	int attribLoc_Vel = -1;
	attribLoc_Vel = glGetAttribLocation(program, "a_Vel");
	glEnableVertexAttribArray(attribLoc_Vel);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePositionColorVelVBO);

	glVertexAttribPointer(attribLoc_Position, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, 0);
	glVertexAttribPointer(attribLoc_Color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLvoid*)(3 * sizeof(float)));
	glVertexAttribPointer(attribLoc_Vel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (GLvoid*)(7 * sizeof(float)));


	//int attribLoc_Vel = -1;
	//attribLoc_Vel = glGetAttribLocation(program, "a_Vel");
	//glEnableVertexAttribArray(attribLoc_Vel);
	//glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVelVBO);
	//glVertexAttribPointer(attribLoc_Vel, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_EmitTime = -1;
	attribLoc_EmitTime = glGetAttribLocation(program, "a_EmitTime");
	glEnableVertexAttribArray(attribLoc_EmitTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleEmitTimeVBO);
	glVertexAttribPointer(attribLoc_EmitTime, 1, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_LifeTime = -1;
	attribLoc_LifeTime = glGetAttribLocation(program, "a_LifeTime");
	glEnableVertexAttribArray(attribLoc_LifeTime);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleLifeTimeVBO);
	glVertexAttribPointer(attribLoc_LifeTime, 1, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_Period = -1;
	attribLoc_Period = glGetAttribLocation(program, "a_Period");
	glEnableVertexAttribArray(attribLoc_Period);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePeriodVBO);
	glVertexAttribPointer(attribLoc_Period, 1, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_Amp = -1;
	attribLoc_Amp = glGetAttribLocation(program, "a_Amp");
	glEnableVertexAttribArray(attribLoc_Amp);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleAmpVBO);
	glVertexAttribPointer(attribLoc_Amp, 1, GL_FLOAT, GL_FALSE, 0, 0);

	int attribLoc_Value = -1;
	attribLoc_Value = glGetAttribLocation(program, "a_Value");
	glEnableVertexAttribArray(attribLoc_Value);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleValueVBO);
	glVertexAttribPointer(attribLoc_Value, 1, GL_FLOAT, GL_FALSE, 0, 0);



	int uniformLoc_Time = -1;
	uniformLoc_Time = glGetUniformLocation(program, "u_Time");
	glUniform1f(uniformLoc_Time, g_time);
	g_time += 0.0005;

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleVerticesCount);

	glDisable(GL_BLEND);
}

void Renderer::DrawFragmentSandbox()
{
	GLuint shader = m_FragmentSandboxShader;
	glUseProgram(shader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int posLoc = glGetAttribLocation(shader, "a_Position");
	int texLoc = glGetAttribLocation(shader, "a_Texcoord");

	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(texLoc);

	glBindBuffer(GL_ARRAY_BUFFER, m_FragmentSandboxVBO);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glVertexAttribPointer(texLoc, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (GLvoid*)(sizeof(float) * 3));

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::GetGLPosition(float x, float y, float* newX, float* newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::Class0310()
{
}

void Renderer::CreateParticles(int numParticles)
{
	float centerX, centerY;
	centerX = 0.f;
	centerY = 0.f;
	float size = 0.01f;
	int particleCount = numParticles;
	m_ParticleVerticesCount = particleCount * 6;
	int floatCount = particleCount * 6 * 3; // x,y,z per vertex
	int floatCountColor = particleCount * 6 * 4; // r, g ,b ,a per vertex
	int floatCountSingle = particleCount * 6;
	int floatCountPosColorVel = particleCount * 6 * (3 + 4 + 3); 
											// x, y, z, r, g, b, a, vx, vy, vz (10)

	float* vertices = NULL;
	vertices = new float[floatCount];

	//position
	int index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		centerX = 0.f;// ((float)rand() / (float)RAND_MAX)*2.f - 1.f;
		centerY = 0.f;// ((float)rand() / (float)RAND_MAX)*2.f - 1.f;
		vertices[index] = centerX - size; index++;
		vertices[index] = centerY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = centerX - size; index++;
		vertices[index] = centerY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = centerX + size; index++;
		vertices[index] = centerY + size; index++;
		vertices[index] = 0.f; index++;

		vertices[index] = centerX + size; index++;
		vertices[index] = centerY + size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = centerX - size; index++;
		vertices[index] = centerY - size; index++;
		vertices[index] = 0.f; index++;
		vertices[index] = centerX + size; index++;
		vertices[index] = centerY - size; index++;
		vertices[index] = 0.f; index++;
	}

	glGenBuffers(1, &m_ParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount,
		vertices, GL_STATIC_DRAW);

	delete[] vertices;

	//Color
	float* verticesColor = NULL;
	verticesColor = new float[floatCountColor];

	index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		float r = ((float)rand() / (float)RAND_MAX);
		float g = ((float)rand() / (float)RAND_MAX);
		float b = ((float)rand() / (float)RAND_MAX);
		float a = ((float)rand() / (float)RAND_MAX);

		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++;
		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++;
		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++;

		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++;
		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++;
		verticesColor[index] = r; index++;
		verticesColor[index] = g; index++;
		verticesColor[index] = b; index++;
		verticesColor[index] = a; index++;
	}

	glGenBuffers(1, &m_ParticleColorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleColorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountColor,
		verticesColor, GL_STATIC_DRAW);

	delete[] verticesColor;

	//pos_color
	float* verticesPosColorVel = NULL;
	verticesPosColorVel = new float[floatCountPosColorVel];

	index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		float centerX = 0.f;// ((float)rand() / (float)RAND_MAX)*2.f - 1.f;
		float centerY = 0.f;// ((float)rand() / (float)RAND_MAX)*2.f - 1.f;
		float r = ((float)rand() / (float)RAND_MAX);
		float g = ((float)rand() / (float)RAND_MAX);
		float b = ((float)rand() / (float)RAND_MAX);
		float a = ((float)rand() / (float)RAND_MAX);

		float centerXVel = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		float centerYVel = ((float)rand() / (float)RAND_MAX) * 2.f;// -1.f;

		verticesPosColorVel[index] = centerX - size; index++;
		verticesPosColorVel[index] = centerY + size; index++;
		verticesPosColorVel[index] = 0.f; index++; // x, y, z
		verticesPosColorVel[index] = r; index++;
		verticesPosColorVel[index] = g; index++;
		verticesPosColorVel[index] = b; index++;
		verticesPosColorVel[index] = a; index++; // r, g, b, a,
		verticesPosColorVel[index] = centerXVel; index++;
		verticesPosColorVel[index] = centerYVel; index++;
		verticesPosColorVel[index] = 0; index++;

		verticesPosColorVel[index] = centerX - size; index++;
		verticesPosColorVel[index] = centerY - size; index++;
		verticesPosColorVel[index] = 0.f; index++;// x, y, z
		verticesPosColorVel[index] = r; index++;
		verticesPosColorVel[index] = g; index++;
		verticesPosColorVel[index] = b; index++;
		verticesPosColorVel[index] = a; index++; // r, g, b, a,
		verticesPosColorVel[index] = centerXVel; index++;
		verticesPosColorVel[index] = centerYVel; index++;
		verticesPosColorVel[index] = 0; index++;

		verticesPosColorVel[index] = centerX + size; index++;
		verticesPosColorVel[index] = centerY + size; index++;
		verticesPosColorVel[index] = 0.f; index++;// x, y, z
		verticesPosColorVel[index] = r; index++;
		verticesPosColorVel[index] = g; index++;
		verticesPosColorVel[index] = b; index++;
		verticesPosColorVel[index] = a; index++; // r, g, b, a,
		verticesPosColorVel[index] = centerXVel; index++;
		verticesPosColorVel[index] = centerYVel; index++;
		verticesPosColorVel[index] = 0; index++;

		verticesPosColorVel[index] = centerX + size; index++;
		verticesPosColorVel[index] = centerY + size; index++;
		verticesPosColorVel[index] = 0.f; index++;// x, y, z
		verticesPosColorVel[index] = r; index++;
		verticesPosColorVel[index] = g; index++;
		verticesPosColorVel[index] = b; index++;
		verticesPosColorVel[index] = a; index++; // r, g, b, a,
		verticesPosColorVel[index] = centerXVel; index++;
		verticesPosColorVel[index] = centerYVel; index++;
		verticesPosColorVel[index] = 0; index++;

		verticesPosColorVel[index] = centerX - size; index++;
		verticesPosColorVel[index] = centerY - size; index++;
		verticesPosColorVel[index] = 0.f; index++;// x, y, z
		verticesPosColorVel[index] = r; index++;
		verticesPosColorVel[index] = g; index++;
		verticesPosColorVel[index] = b; index++;
		verticesPosColorVel[index] = a; index++; // r, g, b, a,
		verticesPosColorVel[index] = centerXVel; index++;
		verticesPosColorVel[index] = centerYVel; index++;
		verticesPosColorVel[index] = 0; index++;

		verticesPosColorVel[index] = centerX + size; index++;
		verticesPosColorVel[index] = centerY - size; index++;
		verticesPosColorVel[index] = 0.f; index++;// x, y, z
		verticesPosColorVel[index] = r; index++;
		verticesPosColorVel[index] = g; index++;
		verticesPosColorVel[index] = b; index++;
		verticesPosColorVel[index] = a; index++; // r, g, b, a,
		verticesPosColorVel[index] = centerXVel; index++;
		verticesPosColorVel[index] = centerYVel; index++;
		verticesPosColorVel[index] = 0; index++;

	}

	glGenBuffers(1, &m_ParticlePositionColorVelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePositionColorVelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountPosColorVel,
		verticesPosColorVel, GL_STATIC_DRAW);

	delete[] verticesPosColorVel;

	//velocity
	float* verticesVel = NULL;
	verticesVel = new float[floatCount];

	index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		centerX = ((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerY = ((float)rand() / (float)RAND_MAX) * 2.f;// -1.f;
		verticesVel[index] = centerX; index++;
		verticesVel[index] = centerY; index++;
		verticesVel[index] = 0; index++;
		verticesVel[index] = centerX; index++;
		verticesVel[index] = centerY; index++;
		verticesVel[index] = 0; index++;
		verticesVel[index] = centerX; index++;
		verticesVel[index] = centerY; index++;
		verticesVel[index] = 0; index++;

		verticesVel[index] = centerX; index++;
		verticesVel[index] = centerY; index++;
		verticesVel[index] = 0; index++;
		verticesVel[index] = centerX; index++;
		verticesVel[index] = centerY; index++;
		verticesVel[index] = 0; index++;
		verticesVel[index] = centerX; index++;
		verticesVel[index] = centerY; index++;
		verticesVel[index] = 0; index++;
	}

	glGenBuffers(1, &m_ParticleVelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount,
		verticesVel, GL_STATIC_DRAW);

	delete[] verticesVel;

	//emit Time
	float* verticesEmitTime = NULL;
	verticesEmitTime = new float[floatCountSingle];

	index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		float emitTime = ((float)rand() / (float)RAND_MAX) * 10.f;

		verticesEmitTime[index] = emitTime; index++;
		verticesEmitTime[index] = emitTime; index++;
		verticesEmitTime[index] = emitTime; index++;

		verticesEmitTime[index] = emitTime; index++;
		verticesEmitTime[index] = emitTime; index++;
		verticesEmitTime[index] = emitTime; index++;
	}

	glGenBuffers(1, &m_ParticleEmitTimeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleEmitTimeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountSingle,
		verticesEmitTime, GL_STATIC_DRAW);

	delete[] verticesEmitTime;

	//life Time
	float* verticesLifeTime = NULL;
	verticesLifeTime = new float[floatCountSingle];

	index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		float lifeTime = ((float)rand() / (float)RAND_MAX) * 1.f;

		verticesLifeTime[index] = lifeTime; index++;
		verticesLifeTime[index] = lifeTime; index++;
		verticesLifeTime[index] = lifeTime; index++;

		verticesLifeTime[index] = lifeTime; index++;
		verticesLifeTime[index] = lifeTime; index++;
		verticesLifeTime[index] = lifeTime; index++;
	}

	glGenBuffers(1, &m_ParticleLifeTimeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleLifeTimeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountSingle,
		verticesLifeTime, GL_STATIC_DRAW);

	delete[] verticesLifeTime;

	//period
	float* verticesPeriod = NULL;
	verticesPeriod = new float[floatCountSingle];

	index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		float period = 1.f * ((float)rand() / (float)RAND_MAX);

		verticesPeriod[index] = period; index++;
		verticesPeriod[index] = period; index++;
		verticesPeriod[index] = period; index++;

		verticesPeriod[index] = period; index++;
		verticesPeriod[index] = period; index++;
		verticesPeriod[index] = period; index++;
	}

	glGenBuffers(1, &m_ParticlePeriodVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticlePeriodVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountSingle,
		verticesPeriod, GL_STATIC_DRAW);

	delete[] verticesPeriod;

	//amp
	float* verticesAmp = NULL;
	verticesAmp = new float[floatCountSingle];

	index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		float amp = 2.f * ((float)rand() / (float)RAND_MAX) - 1.f;

		verticesAmp[index] = amp; index++;
		verticesAmp[index] = amp; index++;
		verticesAmp[index] = amp; index++;

		verticesAmp[index] = amp; index++;
		verticesAmp[index] = amp; index++;
		verticesAmp[index] = amp; index++;
	}

	glGenBuffers(1, &m_ParticleAmpVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleAmpVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountSingle,
		verticesAmp, GL_STATIC_DRAW);

	delete[] verticesAmp;

	//parametric value
	float* verticesValue = NULL;
	verticesValue = new float[floatCountSingle];

	index = 0;
	for (int i = 0; i < particleCount; i++)
	{
		float value = 1.f * ((float)rand() / (float)RAND_MAX);

		verticesValue[index] = value; index++;
		verticesValue[index] = value; index++;
		verticesValue[index] = value; index++;

		verticesValue[index] = value; index++;
		verticesValue[index] = value; index++;
		verticesValue[index] = value; index++;
	}

	glGenBuffers(1, &m_ParticleValueVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleValueVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCountSingle,
		verticesValue, GL_STATIC_DRAW);

	delete[] verticesValue;


	
}
