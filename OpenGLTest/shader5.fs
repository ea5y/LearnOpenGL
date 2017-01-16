#version 330 core 

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 color;

struct Material
{
	//vec3 ambient;
	//vec3 diffUse;
	sampler2D diffUse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffUse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;


void main()
{
	//norm
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	//ambient
	//vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = light.ambient * vec3(texture(material.diffUse, TexCoords));

	//diffUse
	float diff = max(dot(lightDir, norm), 0.0f);
	//vec3 diffUse = light.diffUse * diff * material.diffUse;
	vec3 diffUse = light.diffUse * diff * vec3(texture(material.diffUse, TexCoords));

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

	//result
	vec3 result = ambient + diffUse + specular;
	color = vec4(result, 1.0f);
}