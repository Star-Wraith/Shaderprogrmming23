#version 330

layout(location=0) out vec4 FragColor;

in float v_Alpha;



void main()
{	
	// sin 100 �� ���ϼ��� �� �� �� �� �� �̷��� ������ - - - - - - �� �ٲ�
	float newLines = 2 * sin(200.0 * (1.0 - v_Alpha));
	FragColor = vec4(1.0,1.0,1.0, newLines);
}