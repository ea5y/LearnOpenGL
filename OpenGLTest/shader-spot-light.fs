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
	float cutOff;
	float outerCutOff;

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
	vec3 lightDir = normalize(light.position - FragPos);
	//vec3 lightDir = normalize(-light.direction);


	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	//vec3 lightDiffuse = light.diffuse;

	//lightDiffuse *= intensity;
	
	//ambient
	//vec3 ambient = light.ambient * material.ambient;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//diffuse
	float diff = max(dot(norm, lightDir), 0.0f);
	//vec3 diffuse = light.diffuse * diff * material.diffuse;
	//vec3 diffuse = lightDiffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));


	//specular *= intensity;



	// Attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

	//result
	vec3 result = (ambient + (diffuse + specular) * intensity) * attenuation;

	
	//Because cutOff and theta are cos value, angle->0 when theta->1, so theta bigger than cutOff will be lighted
	//if(theta > light.cutOff)
	//{
		color = vec4(result, 1.0f);
	//}else{
	//	color = vec4(ambient, 1.0f);
	//}
}