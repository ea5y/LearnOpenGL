#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out ourColor;
out ourTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

