#version 330

in vec3 a_Position; //ATTRIBUTE (VS INPUT)
in vec3 a_Vel;
in float a_EmitTime;
in float a_LifeTime;

uniform float u_Time;

const vec3 c_Gravity = vec3(0.0,-0.8,0.0);
const float c_lifeTime = 2.0;
const vec3 c_Vel = vec3(0.1, 0.0, 0.0);
const float c_PI = 3.141592;

void main()
{
	

	vec4 newPosition = vec4(0, 0, 0, 1);
	float t = fract(u_Time/10.0)* 10.0;
	newPosition.x = a_Position.x + t*c_Vel.x;
	float yTime = (t/10.0) * 2 *c_PI;		   // 시험문제!!!!	
	newPosition.y = a_Position.y + sin(yTime); // 시험문제!!!!
	gl_Position = newPosition;




	/*float t = u_Time - a_EmitTime;
	vec4 newPosition = vec4(0,0,0,1);
	if(t < 0.0)
	{

	}
	else
	{
		float newT = a_LifeTime * fract(t/a_LifeTime);
		newPosition.xyz = a_Position 
					+ a_Vel * newT
					+ 0.5 * c_Gravity * newT * newT;
		newPosition.w = 1;
	}
	gl_Position = newPosition;*/
}
