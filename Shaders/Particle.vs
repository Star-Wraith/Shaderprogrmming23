#version 330

in vec3 a_Position; //ATTRIBUTE (VS INPUT)
in vec3 a_Vel;
in float a_EmitTime;
in float a_LifeTime;
in float a_Period;
in float a_Amp;
in float a_Value;
in vec4 a_Color;
in vec2 a_UV;

uniform float u_Time;

const vec3 c_Gravity = vec3(0.0, -1.8, 0.0);
const float c_lifeTime = 2.0;
const vec3 c_Vel = vec3(0.0, -0.8, 0.0);
const float c_PI = 3.141592;
const float c_Period = 1.0;
const float c_Amp = 1.0;

varying vec4 v_Color;
out vec2 v_UV;

void GraphSin()
{
	float t = u_Time - a_EmitTime;
	vec4 newPosition = vec4(0, 0, 0, 1);
	float newAlpha = 0.0f;
	
	if(t < 0.0)
	{

	}
	else
	{

		float newT = a_LifeTime * fract(t/a_LifeTime);
		float nX = sin(a_Value * 2.0 * c_PI );
		float nY = cos(a_Value * 2.0 * c_PI );
		newPosition.x = a_Position.x + nX + newT*a_Vel.x;
		newPosition.y = a_Position.y + nY + newT*a_Vel.y;
		
	
		
		vec2 newDir = vec2(-a_Vel.y, a_Vel.x);
		newDir = normalize(newDir);
		newPosition.xy += newT*a_Amp*sin(a_Period * newT * 2.0 * c_PI) * 
							newDir;

		newAlpha = 1.0 - newT / a_LifeTime;
		newAlpha = pow(newAlpha, 2);


		
	}	

	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, a_Color.a * newAlpha);
	v_UV = a_UV;

}

void P1()
{
	float t = u_Time - a_EmitTime;
	vec4 newPosition = vec4(0, 0, 0, 1);


	if(t < 0.0)
	{

	}
	else
	{
		
		float newT = a_LifeTime * fract(t/a_LifeTime);
		newPosition.xyz = a_Position 
							+ a_Vel*newT
							+ 0.5 * c_Gravity * newT * newT;
		
		newPosition.w = 1;
	}

	gl_Position = newPosition;
	v_Color = a_Color;
	v_UV = a_UV;


}

void main()
{
	GraphSin();
	
	/*
	vec4 newPosition = vec4(0, 0, 0, 1);

	float t = fract(u_Time/10.0)*10.0;
	
	//float t = u_Time; // 아 속도 차이 미세하게 바꿀때 스칼라 값 바꾸려고 저런듯?

	
	newPosition.x = -1.0 + a_Position.x + abs(t*a_Vel.x);
	float value = 4 * c_PI * newPosition.x; // 내가 추가한 코든데 점이 sin좌표계가 이동하듯이 변경해줌(pos.y에서 빼면서)


	float yTime = (t/10.0) * 2 *c_PI;		  
	newPosition.y = a_Position.y + sin(value - yTime); 

	//newPosition.x = a_Position.x + t * c_Vel.x;
	//float yTime = (t/10.0) * 2 *c_PI;		   // 시험문제!!!!	
	//newPosition.y = a_Position.y + sin(yTime); // 시험문제!!!!

	gl_Position = newPosition;
	v_Color = a_Color;
	v_UV = a_UV;
	*/

	/*
	// 원 갯수 구하려고 낫둔 식
	gl_Position = vec4(10*a_Position,1);
	v_Color = a_Color;
	v_UV = a_UV;
	*/

	//P1();

}