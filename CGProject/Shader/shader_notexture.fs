#version 330 core
out vec4 FragColor;
 
struct Light {
	vec3 position;  
  
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	//float constant;
	//float linear;
	//float quadratic;
};
 
in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
//从Mtl中读取的数据
//Material
in vec4 Ambient;
in vec4 Diffuse;
in vec4 Specular;
in float Shinness; 
in vec4 FragPosLightSpace;
uniform vec3 viewPos;
uniform Light light;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	if (currentDepth > 1.0)
		return 0.0;
	vec3 lightDir = normalize(light.position - FragPos);
	float bias = max(0.05 * (1.0 - dot(Normal, lightDir)), 0.005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}
 
void main()
{    
	// ambient
	vec3 ambient = light.ambient * Diffuse.rgb * Ambient.rgb;
	  
	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse =light.diffuse * diff * Diffuse.rgb;
	  
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shinness);
	vec3 specular = light.specular * spec * Specular.rgb;  
	
	//calculate shadow
	float shadow = ShadowCalculation(FragPosLightSpace);

	vec3 result = ambient + (1.0 - shadow)*(diffuse +specular);
	FragColor = vec4(result ,1.0);
}
	