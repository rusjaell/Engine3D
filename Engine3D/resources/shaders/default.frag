#version 440 core

layout(location = 0) out vec4 o_Color;

in vec3 v_Position;
in vec2 v_UV;
in vec3 v_Normal;

struct Material
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    
    sampler2D diffuseTexture;
    sampler2D ambientTexture;
    sampler2D normalTexture;
    sampler2D alphaTexture; // Added alpha texture
    sampler2D displacementTexture;
};

uniform Material u_Material;

void main()
{
    vec4 diffuseColor = texture(u_Material.diffuseTexture, v_UV);
    vec4 ambientColor = texture(u_Material.ambientTexture, v_UV);
    vec4 alphaColor = texture(u_Material.alphaTexture, v_UV); 
  
    vec3 diffuse = diffuseColor.rgb * u_Material.diffuseColor;
    vec3 ambient = ambientColor.rgb * u_Material.ambientColor;
    
    vec3 color = diffuse + ambient;
	
	float alphaValue = diffuseColor.a;
	
    o_Color = vec4(color, alphaValue);
}