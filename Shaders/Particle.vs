#version 330

in vec3 a_Position; //ATTRIBUTE (VS INPUT)
in vec3 a_Vel;

const vec3 c_Gravity = vec3(0.0,-0.8,0.0);

uniform float u_Time;

void main()
{
	vec4 newPosition;
	newPosition.xyz = a_Position 
					+ a_Vel * u_Time 
					+ 0.5 * c_Gravity * u_Time * u_Time;
	newPosition.w = 1;
	gl_Position = newPosition;
}
