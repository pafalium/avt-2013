#version 330 core
//vertex

layout(location = 0) in vec4 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor;

out vec4 color;

void main()
{
	gl_Position = vertexPosition_modelspace;
	color = vertexColor;
} 