#version 330

layout(location=0) out vec4 FragColor;

varying vec4 v_Color;
in vec2 v_UV;

void circle(){

	vec2 temp = v_UV - vec2(0.5,0.5);
	float d = length(temp);


	if( d < 0.5){
		FragColor = vec4(1) * v_Color;
	}
	else{
		FragColor = vec4(0) * v_Color;
	}


}

void circles(){

	vec2 temp = v_UV - vec2(0.5,0.5);
	float d = length(temp);
	float value = sin(50*d);  // 원을 ?개 만드려면 어느 정도 값을 넣어야 하나를 공부하기!
	// 필독!!!! 10도에 하나인듯 합니다!!!!
	
	FragColor = vec4(value) * v_Color;
}

void main()
{
	//FragColor = v_Color;
	//circle();
	circles();

}
