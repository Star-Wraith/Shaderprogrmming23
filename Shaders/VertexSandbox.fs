#version 330

layout(location=0) out vec4 FragColor;

in float v_Alpha;


void main()
{
	FragColor = vec4(1.0,1.0,1.0, v_Alpha);
}