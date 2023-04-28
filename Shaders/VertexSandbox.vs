#version 330

in vec3 a_Position; //ATTRIBUTE (VS INPUT)


const float PI = 3.141592;

uniform float u_Time;

out float v_Alpha;

void main()
{

/*
	float value = PI * (a_Position.x + 1.0) ;
	float d = a_Position.x + 1.0f; // +1 이래야 x 시작점이 왼쪽에서 하는듯 -1로하면 오른쪽 0이면 가운데
	float newY = 0.5 * d * sin(value - u_Time* 10);
	vec4 newPos = vec4(a_Position, 1.0) ;
	newPos.y = newY;
	gl_Position = newPos;
*/


	// 밑에는 Particle.vs처럼 했음 Sin좌표계
	
	float newy = a_Position.y + sin(PI*a_Position.x - u_Time);

	vec4 newPos = vec4(a_Position, 1.0);
	newPos.y = newy;

	gl_Position = newPos;
	
	//gl_Position = vec4(a_Position, 1);

	v_Alpha = 1.0 - (a_Position.x + 1.0)/2.0;

}