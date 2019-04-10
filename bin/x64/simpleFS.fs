#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	color = texture(ourTexture, texCoord);
}