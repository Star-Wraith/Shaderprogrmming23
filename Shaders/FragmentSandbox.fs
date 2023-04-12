#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Texcoord;

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


	if( d < 0.5){
		FragColor = vec4(1);
	}
	else{
		FragColor = vec4(0);
	}
	
}

void circles(){

	vec2 temp = v_Texcoord - vec2(0.5,0.5);
	float d = length(temp);
	float value = sin(30*d);  // 원을 ?개 만드려면 어느 정도 값을 넣어야 하나를 공부하기!
	
	FragColor = vec4(value);

	
}

void main()
{
	//test();
	//circle();
	circles();
}
