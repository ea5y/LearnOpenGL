#version 330 core

in vec3 ourColor;
in vec2 ourTexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float mixValue;

void main()
{
	color = mix(texture(ourTexture1, ourTexCoord), texture(ourTexture2, vec2(ourTexCoord.x - 0.1, 1 - ourTexCoord.y)), mixValue) * vec4(ourColor, 1.0f);
}