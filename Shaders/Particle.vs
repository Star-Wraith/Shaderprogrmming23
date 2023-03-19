#version 330

layout(location = 0) in vec3 a_Position; //ATTRIBUTE (VS INPUT)

out vec4 newPosition;
void main()
{
	vec4 newPosition;
	newPosition.xyz = a_Position;
	newPosition.w = 1;
}
