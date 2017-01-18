#version 330 core 

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 color;

struct Material
{
	//vec3 ambient;
	//vec3 diffUse;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;


void main()
{
	//norm
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(light.position - FragPos);
	vec3 lightDir = normalize(-light.direction);

	//ambient
	//vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//diffuse
	float diff = max(dot(norm, lightDir), 0.0f);
	//vec3 diffuse = light.diffuse * diff * material.diffuse;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

	// Attenuation
    //float distance    = length(light.position - FragPos);
    //float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    //ambient  *= attenuation;  
    //diffuse  *= attenuation;
    //specular *= attenuation;   

	//result
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}