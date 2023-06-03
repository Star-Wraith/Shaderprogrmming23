#version 330

layout(location=0) out vec4 FragColor0;
layout(location=1) out vec4 FragColorHigh;
//layout(location=2) out vec4 FragColor2;
//layout(location=3) out vec4 FragColor3;
//layout(location=4) out vec4 FragColor4;

varying vec4 v_Color;
in vec2 v_UV;

uniform sampler2D u_Texture;



void circle(){

	vec2 temp = v_UV - vec2(0.5,0.5);
	float d = length(temp);


	if( d < 0.2){
		FragColor0 = vec4(1) * v_Color;
		//FragColor1 = vec4(1,0,0,1);
		//FragColor2 = vec4(0,1,0,1);
		//FragColor3 = vec4(0,0,1,1);

	}
	else{
		FragColor0 = vec4(0) * v_Color;
		//FragColor1 = vec4(0) * v_Color;
		//FragColor2 = vec4(0) * v_Color;
		//FragColor3 = vec4(0) * v_Color;
	}
	

}

void circles(){

	vec2 temp = v_UV - vec2(0.5,0.5);
	float d = length(temp);
	float value = sin(50*d);  // 원을 ?개 만드려면 어느 정도 값을 넣어야 하나를 공부하기!
	// 필독!!!! 10도에 하나인듯 합니다!!!!
	
	FragColor0 = vec4(value) * v_Color;
}

void Textured()
{
	vec4 result = texture(u_Texture, v_UV) * v_Color;
	float brightness = dot(result.rgb, vec3(0.2126, 0.7152, 0.0722));
	FragColor0 = result;
	if(brightness > 1.0){
		FragColorHigh = result;
	}
	else{
		FragColorHigh = vec4(0);
	}
}

void main()
{
	//FragColor = v_Color;
	//circle();
	//circles();
	Textured();

}
