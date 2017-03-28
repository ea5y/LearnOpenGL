#version 330 core

struct Output
{
    vec3 Normal;
    vec2 TexCoords;
    vec3 FragPos;
};
in Output output;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;

    sampler2D texture_diffuse1;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

struct SpotLight
{
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
uniform DirLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform vec3 viewPos;

out vec4 color;

vec3 CalcDirLight(DirLight light, Material material, vec3 normal, vec2 texCoords, ve3 viewDir)
{
    //lightDir
    vec3 lightDir = normalize(-light.direction);

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    //diffuse
    //calc diffuse intensity
    float diff = max(dot(normal, lightDir), 0.0);
    //merge light.diffuse,diff,material.diffuse
    vec3 diffuse = light.diffuse * diff * ve3(texture(material.diffuse, texCoords));

    //specular
    //calc reflecDir
    vec3 reflecDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflecDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

    //result
    return (ambient + diffuse + specular);
}


