#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexSampler;

uniform vec2 u_XYRepeat;

uniform float u_Step;

uniform float u_SeqNum;

in vec2 v_TexPos;

void P1(){
	float x = v_TexPos.x;
	float y = 1.0 - abs(v_TexPos.y * 2.0 - 1.0);
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler,newTexPos);
}

void P2(){
	float x = fract(v_TexPos.x * 3.0);
	float dy = v_TexPos.y / 3.0;
	float y = floor(3.0 * (1.0 - v_TexPos.x))/3.0 + dy ;
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler,newTexPos);
	//FragColor = vec4(dy*3);
}

void P3(){
	float x = fract(v_TexPos.x * 3.0);
	float dy = v_TexPos.y / 3.0;
	float y = floor(3.0 * v_TexPos.x)/3.0 + dy ;
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler,newTexPos);
}

void P4(){
	float x = v_TexPos.x;
	float dy = fract(3.0 * v_TexPos.y) / 3.0;
	float y = floor(3.0 * (1.0 - v_TexPos.y))/3.0 + dy ;
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler,newTexPos);
}

void P5(){
	float x_repeat = u_XYRepeat.x;
	float y_repeat = u_XYRepeat.y;
	float dx = v_TexPos.x * x_repeat;
	float x = dx + floor((1.0 - v_TexPos.y) * y_repeat) * 0.5;
	float y = fract(v_TexPos.y*y_repeat);
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler, newTexPos);
}

void P6(){
	float x_repeat = u_XYRepeat.x;
	float y_repeat = u_XYRepeat.y;
	float dy = v_TexPos.y * y_repeat;
	float x = fract(v_TexPos.x * x_repeat);
	float y = fract(dy + floor(v_TexPos.x * x_repeat) * 0.5);
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler, newTexPos);
}

void P7(){ // 시헝 50% <- 왠만하면 나온다는 뜻, 이거랑 똑같진 않지요
	//float x_repeat = u_XYRepeat.x;
	//float y_repeat = u_XYRepeat.y;
	float y = (v_TexPos.x + v_TexPos.y);
	float x = fract(v_TexPos.x + (1.0 - v_TexPos.y));
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler, newTexPos);
	//FragColor = vec4(x);
}

void MultiTexture(){


	float x = v_TexPos.x;
	float y = u_Step/6.0 + v_TexPos.y /6.0;
	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler, newTexPos);
}

void SATexture(){

	float seqNum = u_SeqNum;
	float nX = int(seqNum)%8;
	float nY = floor(seqNum/8.0);
	float offsetX = nX  / 8.0;
	float offsetY = nY / 6.0;
	float x = offsetX + v_TexPos.x/8.0;
	float y = offsetY + v_TexPos.y/6.0;

	vec2 newTexPos = vec2(x,y);
	FragColor = texture(u_TexSampler, newTexPos);
}

void main()
{
	//MultiTexture();
	
	SATexture();
}
