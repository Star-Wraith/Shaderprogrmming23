#version 330

layout(location=0) out vec4 FragColor0;
layout(location=1) out vec4 FragColor1;
layout(location=2) out vec4 FragColor2;
layout(location=3) out vec4 FragColor3;
layout(location=4) out vec4 FragColor4;

in vec2 v_Texcoord;

uniform vec2 u_Point;
uniform vec2 u_Points[3];
uniform float u_Time;
uniform sampler2D u_Texture;
const float c_PI = 3.141592;


void test(){

	float newValueX = v_Texcoord.x * 10 * c_PI;
	float newValueY = v_Texcoord.y * 10 * c_PI;
	float outColorGrayVertical = sin(newValueX); // | | | | | 하얀색 | 의 갯수는 위에 숫자의 1/2개
	float outColorGrayHorizontal = sin(newValueY);// ㅡ 하얀색 ㅡ 의 갯수는 위에 숫자의 1/2개


	float newColor = max(outColorGrayVertical, outColorGrayHorizontal);
	FragColor0 = vec4(newColor);

}

void circle(){

	vec2 temp = v_Texcoord - vec2(0.5,0.5);
	float d = length(temp);

	vec2 temp1 = v_Texcoord - u_Points[1];
	float d1 = length(temp1);

	vec2 temp2 = v_Texcoord - u_Points[2];
	float d2 = length(temp2);



	if( d < 0.1 || d1 < 0.1 || d2 < 0.1){
		FragColor1 = vec4(1);
	}
	else{
		FragColor1 = vec4(0);
	}

}

void circles(){

	vec2 temp = v_Texcoord - u_Point;
	
	float d = length(temp);
	float value = sin(50*d);  // 원을 ?개 만드려면 어느 정도 값을 넣어야 하나를 공부하기!
	
	FragColor2 = vec4(value);
}

void rader(){
	
	vec2 temp = v_Texcoord - vec2(0.5,1.0);
	
	float d = length(temp);
	//float value = sin(c_PI*d - u_Time); // 원이 쪼그라든다. -면 확대되는 느낌

	//c_PI*d * x <- 여길 크게 해도 원이 얇아지고 pow, 12 < 이부분도 크게하면 얇아지네
	//0.2는 크게 하면 커지고 -0.5는 -를 크게하면 작아짐

	float value = 0.2* (pow(sin(c_PI*d*2 - 1 * u_Time), 12)-0.5); // 원이 얇아진다.
	float temp1 = ceil(value);



	vec4 result = vec4(0);

	for(int i = 0; i<3; ++i){
		vec2 temp = v_Texcoord - u_Points[i];
		float d = length(temp);


		if( d < 0.05){
			result += 1.0 * temp1;
		}
	}
	




	FragColor3 = vec4(result + 10 * value);

}

void flag(){

	float finalColor  = 0;
	/*
	for(int i = 0; i< 5; ++i){
		float newTime = u_Time + i * 0.2;
		float newColor = v_Texcoord.x * 0.5 * sin(v_Texcoord.x * c_PI*2*2 - 1 * newTime);
		float sinValue = sin(v_Texcoord.x * c_PI*2*10 - 5 * newTime);

		float width = 0.01* v_Texcoord.x * 5 + 0.001;
		if(2.0*(v_Texcoord.y - 0.5 ) > newColor && 
			2.0*(v_Texcoord.y - 0.5 ) < newColor + width){

		
			finalColor += 1*sinValue*(1.0-v_Texcoord.x);
		}
		else{
		}
	}*/

	


	float sinValue = 0.5 * sin(2* v_Texcoord.x * 2.0 * c_PI - u_Time); // -u_Time이면 오른쪽 +면 왼쪽으로감

	if(v_Texcoord.y * 2.0 - 1 < sinValue && v_Texcoord.y * 2.0 - 1> sinValue - 0.01 ){
		FragColor4 = vec4(1);
	}
	else{
		
		FragColor4 = vec4(0);
	}


	//FragColor = vec4(finalColor);
	
}


void realflag(){

	float period = (v_Texcoord.x + 1.0) * 1.0;
	float xValue = v_Texcoord.x*2.0*c_PI * period;
	float yValue = ((1.0 - v_Texcoord.y) - 0.5) * 2.0;
	float sinValue = 0.25*sin(xValue - 3.0* u_Time);
	
	if(sinValue*v_Texcoord.x + 0.75  > yValue 
	&& sinValue*v_Texcoord.x  - 0.75 < yValue){
		float vX = v_Texcoord.x;
		float yWidth = 1.5;
		float yDistance = yValue - (sinValue*v_Texcoord.x  - 0.75);
		float vY = 1.0 - yDistance / yWidth;

		FragColor4 = texture(u_Texture, vec2(vX, vY));
		//FragColor = vec4(vX, vY, 0, 1);
	}
	else{
		FragColor4 = vec4(0);
	}
}

void main()
{
	test();
	circle();
	circles();
	rader();
	realflag();

	//FragColor = texture(u_Texture, vec2(v_Texcoord.x,v_Texcoord.y));


}
