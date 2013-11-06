#version 330 core
//vertex

in vec4 in_Position;
in vec4 in_Color;

uniform mat4 Matrix;

out vec4 ex_Color;

void main()
{
	gl_Position = Matrix * in_Position;
	ex_Color = in_Color;
} 