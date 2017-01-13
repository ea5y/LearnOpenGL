#version 330 core

in vec2 ourTexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	color = mix(texture(ourTexture1, ourTexCoord), texture(ourTexture2, vec2(ourTexCoord.x - 0.1, 1 - ourTexCoord.y)), 0.4);
}