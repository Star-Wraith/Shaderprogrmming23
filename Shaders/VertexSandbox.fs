#version 330

layout(location=0) out vec4 FragColor;

in float v_Alpha;



void main()
{	
	// sin 100 값 높일수록 ㅡ ㅡ ㅡ ㅡ ㅡ 이렇게 가던게 - - - - - - 로 바뀜
	float newLines = 2 * sin(200.0 * (1.0 - v_Alpha));
	FragColor = vec4(1.0,1.0,1.0, newLines);
}