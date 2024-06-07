#version 440 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_UV;
layout(location = 2) in vec3 in_Normal;

out vec3 v_Position;
out vec2 v_UV;
out vec3 v_Normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

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

uniform Material u_Material;

void main()
{
    vec4 worldPosition = u_Model * vec4(in_Position, 1.0);
    
    vec4 displacement = texture(u_Material.displacementTexture, in_UV);
	if(displacement == vec4(0, 0, 0, 1)) {
		vec3 displacedPosition = in_Position + normalize(in_Normal) * displacement.rgb;
		worldPosition = u_Model * vec4(displacedPosition, 1.0);
	}
	
    gl_Position = u_Projection * u_View * worldPosition;

    v_Position = worldPosition.xyz;
    v_UV = in_UV;
    v_Normal = in_Normal;
};