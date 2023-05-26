#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_TexPos;
in float v_greyScale;

uniform sampler2D u_TexSampler;

void main()
{
	//FragColor = vec4(v_TexPos, 0,1.0);
	vec2 newTexPos = fract(v_TexPos * 4.0);
	FragColor = 0.5*(v_greyScale + 1.0 + 0.2) * texture(u_TexSampler, newTexPos);
}
