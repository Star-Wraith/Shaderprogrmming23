#version 330

in vec3 a_Position; //ATTRIBUTE (VS INPUT)

const float PI = 3.141592;

void Test()
{
	float x = a_Position.x;
	float value = (a_Position.x + 0.5) * 2.0 * PI;
	float y = a_Position.y + 0.5 * sin(value);

	vec4 newPosition = vec4(x, y, 0.0, 1.0);
	gl_Position = newPosition;
}

void main()
{
	Test();
}