#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
 
uniform Mat{
	vec4 aAmbient;
	vec4 aDiffuse;
	vec4 aSpecular;
    float aShinness;
};
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

out vec4 Ambient;
out vec4 Diffuse;
out vec4 Specular;
out float Shinness;
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
 
void main()
{
 
    FragPos = vec3( model * vec4(aPos, 1.0));
    //Normal =vec3(projection * vec4(mat3(transpose(inverse(view * model))) * aNormal,0.0));  
	Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
	Ambient = aAmbient;
	Diffuse = aDiffuse;
	Specular = aSpecular;
    Shinness = aShinness;
 	
    gl_Position = projection * view * vec4(FragPos, 1.0);
}