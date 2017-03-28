#version 330 core

//in vec2 TexCoords;
//in vec3 Normal;
//in vec3 FragPos;

//Change to struct
struct Output
{
	vec2 TexCoords;
	vec3 Normal;
	vec3 FragPos;
};
in Output output;

out vec4 color;

//uniform sampler2D texture_diffuse1;

struct Material
{
	//vec3 ambient;
	//vec3 diffuse;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;

	sampler2D texture_diffuse1;

};

struct PointLight
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

struct SpotLight{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

uniform Material material;
uniform PointLight pointLights;
uniform SpotLight spotLight;

uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec2 texCoords, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	//ambient
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));
	
	//diffuse
	//=========
	//calc diffuse intensity
	float diff = max(dot(normal, lightDir), 0.0f);
	//merge
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoords));

	//specular
	//============
	vec3 reflecDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflecDir, viewDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	//calc attenuation
	//===========
	// 1.0f / (constant + linear * distance + quadratic * (distance * distance));
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * (distance * distance));

	//result
	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, Material material, vec3 normal, vec2 texCoords, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	//ambient
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCoords));

	//diffuse
	//==========
	//calc diff intensity
	float diffIntensity = max(dot(lightDir, normal), 0.0f);
	//merge
	vec3 diffuse = light.diffuse * diffIntensity * vec3(texture(material.texture_diffuse1, texCoords));

	//specular
	//===========
	//calc reflecDir
	vec3 reflecDir = reflect(-lightDir, normal);
	//calc spec
	float spec = max(dot(reflecDir, viewDir), 0.0f);
	//merge
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	//calc attenuation
	//===========
	// 1.0f / (constant + linear * distance + quadratic * (distance * distance));
	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * (distance * distance));

	float theta = dot(lightDir, normalize(-light.direction));
	//calc smooth
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	//result
	return (ambient + (diffuse + specular) * intensity) * attenuation;
}

void main()
{
	vec3 normal = normalize(output.Normal);
	vec3 viewDir = normalize(viewPos - output.FragPos);

	//color = vec4(texture(material.texture_diffuse1, TexCoords));
	//vec3 result = CalcPointLight(pointLights, material, normal, output.TexCoords, output.FragPos, viewDir);

	//spot light
	vec3 result = CalcSpotLight(spotLight, material, normal, output.TexCoords, output.FragPos, viewDir);

	color = vec4(result, 1.0f);
}