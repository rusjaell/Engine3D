#version 440 core

layout (location = 0) out vec4 o_Color;

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
    sampler2D alphaTexture;
    sampler2D displacementTexture;
	
	float shininess;
};

struct Light 
{
    vec4 vector;

    vec3 diffuseColor;
    vec3 ambientColor;
    vec3 specularColor;
};

uniform Material u_Material;

uniform Light u_Light; 
uniform mat4 u_Model;
uniform vec3 u_CameraPos;

void main()
{	
    vec4 diffuseColor = texture(u_Material.diffuseTexture, v_UV);
    vec4 ambientColor = texture(u_Material.ambientTexture, v_UV);
	vec4 specularColor = vec4(0, 0, 0, 1); //texture(u_Material.specularTexture, v_UV);

	vec3 normal = normalize(v_Normal);
	
	vec3 lightDirection = vec3(0, 0, 0);
	if(u_Light.vector.a == 0.0f){
		lightDirection = normalize(-u_Light.vector.rgb);
	}
	else if(u_Light.vector.a == 1.0f){
		lightDirection = normalize(u_Light.vector.rgb - v_Position);  
	}
	
	vec3 viewDirection = normalize(u_CameraPos - v_Position);
	
	float diffuseAmount = max(dot(normal, lightDirection), 0.0);
	vec3 reflectDirection = reflect(-lightDirection, normal);  

	float specAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Material.shininess);

	vec3 diffuse = u_Light.diffuseColor * u_Material.diffuseColor * diffuseColor.rgb * diffuseAmount;
    //vec3 ambient = u_Light.ambientColor * u_Material.ambientColor * ambientColor.rgb;
    vec3 ambient = u_Light.ambientColor * ambientColor.rgb;
    vec3 specular = u_Light.specularColor * u_Material.specularColor * specularColor.rgb * specAmount;  
	
	if(u_Light.vector.a != 1.0f)
	{
		float constant = 1.0f;
		float linear = 0.09f;
		float quadratic = 0.08f;

		float distance    = length(u_Light.vector.rgb - v_Position);
		float attenuation = 1.0 / (constant + linear * distance +  quadratic * (distance * distance));    
		
		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;   
	}

    vec3 color = (diffuse + ambient + specular);
    
    o_Color = vec4(color, 1.0f);
}