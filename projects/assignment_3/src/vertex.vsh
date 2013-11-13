#version 330 core
//vertex

in vec3 in_Position;
//in vec4 in_Color;
in vec3 in_Normal;
in vec2 in_TexCoord;

uniform mat4 Matrix;
uniform vec3 Color;

layout (std140) uniform SharedMatrices {
    mat4 View;
    mat4 Projection;
};

out vec4 ex_Color;

vec3 LightDir = vec3(-1,-1,-1);

void main()
{
	gl_Position = Projection * View * Matrix * vec4(in_Position,1.0f);
	vec3 normLightDir = normalize(LightDir);
	vec4 transNormal = normalize(View * Matrix * vec4(in_Normal,0.0f));
	float lightFactor = abs(dot(normLightDir,transNormal.xyz));
	ex_Color = vec4(Color*lightFactor,1.0f);
} 