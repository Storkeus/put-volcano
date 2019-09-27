#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    //vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLight;
uniform DirectionalLight directionalLight;

vec3 calcPointLight(PointLight light,  vec3 norm, vec3 fragPos, vec3 viePos)
{

 // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse

    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    //vec3 diffuse = light.diffuse * diff *   vec3(normalize(texture(material.diffuse, TexCoords).r+0.3f),texture(material.diffuse, TexCoords).g,texture(material.diffuse, TexCoords).b);

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.specular * spec *  vec3(normalize(texture(material.specular, TexCoords).r+0.3f),texture(material.specular, TexCoords).g,texture(material.specular, TexCoords).g);
    vec3 specular = light.specular * spec * texture(material.specular,TexCoords).rgb;

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    return result;

}

vec3 calcDirectionalLight(DirectionalLight light,vec3 norm, vec3 FragPos,vec3 viewPost)
{
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

     // diffuse
    // vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

     // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec;

    vec3 result=ambient+diffuse+specular;
    return result;
}

void main()
{
    vec3 norm = normalize(Normal);

   vec3 result=calcDirectionalLight(directionalLight,norm,FragPos,viewPos)+calcPointLight(pointLight,norm,FragPos,viewPos);
    FragColor = vec4(result, 1.0);
}
