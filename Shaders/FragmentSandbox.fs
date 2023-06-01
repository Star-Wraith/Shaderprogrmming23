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
	float outColorGrayVertical = sin(newValueX); // | | | | | �Ͼ�� | �� ������ ���� ������ 1/2��
	float outColorGrayHorizontal = sin(newValueY);// �� �Ͼ�� �� �� ������ ���� ������ 1/2��


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
	float value = sin(50*d);  // ���� ?�� ������� ��� ���� ���� �־�� �ϳ��� �����ϱ�!
	
	FragColor2 = vec4(value);
}

void rader(){
	
	vec2 temp = v_Texcoord - vec2(0.5,1.0);
	
	float d = length(temp);
	//float value = sin(c_PI*d - u_Time); // ���� �ɱ׶���. -�� Ȯ��Ǵ� ����

	//c_PI*d * x <- ���� ũ�� �ص� ���� ������� pow, 12 < �̺κе� ũ���ϸ� �������
	//0.2�� ũ�� �ϸ� Ŀ���� -0.5�� -�� ũ���ϸ� �۾���

	float value = 0.2* (pow(sin(c_PI*d*2 - 1 * u_Time), 12)-0.5); // ���� �������.
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

	


	float sinValue = 0.5 * sin(2* v_Texcoord.x * 2.0 * c_PI - u_Time); // -u_Time�̸� ������ +�� �������ΰ�

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
