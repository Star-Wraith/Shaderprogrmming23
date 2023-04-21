#version 330

in vec3 a_Position; //ATTRIBUTE (VS INPUT)


const float PI = 3.141592;

uniform float u_Time;

void main()
{
	float value = PI * (a_Position.x + 1.0) ;
	float d = a_Position.x + 1.f;

	float newY = 0.5 * d * sin(value - u_Time* 10);
	vec4 newPos = vec4(a_Position, 1.0) ;
	newPos.y = newY;
	gl_Position = newPos;

}