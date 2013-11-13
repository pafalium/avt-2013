#version 330 core
//vertex

in vec3 in_Position;
//in vec4 in_Color;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 Matrix;

layout (std140) uniform SharedMatrices {
    mat4 View;
    mat4 Projection;
};

out vec4 ex_Color;

vec3 LightDir = vec3(-1,-1,-1);

void main()
{
	gl_Position = Projection * View * Matrix * vec4(in_Position,1.0f);
	ex_Color = vec4(dot(normalize(LightDir),in_Normal));
} 