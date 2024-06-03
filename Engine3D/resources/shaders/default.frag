#version 440 core

layout(location = 0) out vec4 o_Color;

in vec3 v_Position;
in vec2 v_UV;
in vec3 v_Normal;

uniform sampler2D u_Albedo;
uniform vec4 u_AlbedoColor;

uniform sampler2D u_Ambient;
uniform vec4 u_AmbientColor;

uniform sampler2D u_Specular;
uniform sampler2D u_Normal;

void main()
{
    vec4 albedoColor = texture(u_Albedo, v_UV);
    vec4 ambientColor = texture(u_Ambient, v_UV);
    vec4 normalMap = texture(u_Specular, v_UV);

	vec3 albedo = albedoColor.rgb * u_AlbedoColor.rgb;

	vec3 ambient = ambientColor.rgb * u_AmbientColor.rgb;

	vec3 color = albedo + ambient;

    o_Color = vec4(color, albedoColor.a);
}