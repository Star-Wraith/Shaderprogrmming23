#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Texcoord;

uniform vec2 u_Point;
uniform vec2 u_Points[3];
uniform float u_Time;
const float c_PI = 3.141592;

void test(){
	if(v_Texcoord.x > 0.5){
		FragColor = vec4(v_Texcoord, 0.0 ,1.0);
	}
	else{
		FragColor = vec4(0.0);
	}
}

void circle(){

	vec2 temp = v_Texcoord - vec2(0.5,0.5);
	float d = length(temp);
	vec2 temp1 = v_Texcoord - u_Points[1];
	float d1 = length(temp1);


	if( d < 0.1 || d1 < 0.1){
		FragColor = vec4(1);
	}
	else{
		FragColor = vec4(0);
	}
}

void circles(){

	vec2 temp = v_Texcoord - u_Point;
	
	float d = length(temp);
	float value = sin(30*d);  // 원을 ?개 만드려면 어느 정도 값을 넣어야 하나를 공부하기!
	
	FragColor = vec4(value);
}

void rader(){
	
	vec2 temp = v_Texcoord - u_Point;
	
	float d = length(temp);
	//float value = sin(c_PI*d - u_Time); // 원이 쪼그라든다. -면 확대되는 느낌
	float value = 0.2* (pow(sin(c_PI*d*2 - u_Time), 12)-0.5); // 원이 얇아진다.
	float temp1 = ceil(value);



	vec4 result = vec4(0);

	for(int i = 0; i<3; ++i){
		vec2 temp = v_Texcoord - u_Points[i];
		float d = length(temp);


		if( d < 0.1){
			result += 1.0 * temp1;
		}
	}
	




	FragColor = vec4(result + 10 * value);

}

void main()
{
	//test();
	//circle();
	//circle();
	rader();
}
