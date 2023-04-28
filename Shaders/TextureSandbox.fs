#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexSampler;
in vec2 v_TexPos;

void main()
{
	/*
	vec2 newTexPos = v_TexPos;
	FragColor = texture(u_TexSampler,newTexPos);
	*/
	
	
	float x = v_TexPos.x;
	float y = 1.0 - abs(v_TexPos.y * 2.0 - 1.0);
	vec2 newTexPos = vec2(x,y); 
	
	/*
	float x = 3*v_TexPos.x;
	
	float y = v_TexPos.y;

	vec2 newTexPos = vec2(x,y);*/

	FragColor = texture(u_TexSampler, newTexPos);
}
