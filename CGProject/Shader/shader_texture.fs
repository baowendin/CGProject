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
in vec4 Diffuse;
in vec4 Ambient;
in vec4 Specular;
in float Shinness; 
uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform Light light;
 
void main()
{    
	// ambient
	vec4 out_Diffuse = texture(texture_diffuse1, TexCoords) * Diffuse;
	vec3 ambient = light.ambient * out_Diffuse.rgb;
	  
	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse =light.diffuse * diff * out_Diffuse.rgb;
	  
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = max(0, pow(max(dot(viewDir, reflectDir), 0.0), Shinness)); // when shinness is 0, 0^0 turns to 1
	vec3 specular = light.specular * spec *  Specular.rgb;  
		  
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result ,1.0);
	//FragColor = Diffuse;
}
	