#version 330 core

in vec3 normal;
in vec3 fragPos;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;


void main()
{
	//ambient
	//==========
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;//result

	//normalize
	//===========
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);

	//diffuse
	//============
	float diff = max(dot(norm, lightDir), 0.0f);//diff->1 when angle->0, diff->0 when angle->1
	vec3 diffuse = diff * lightColor;//result

	//specular
	//==================	
	float specularStrength = 0.5f;//specularStrength
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);//32 is Shininess
	vec3 specular = specularStrength * spec * lightColor;//result

	//result
	//======================
	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0f);
}