#version 440 core

layout(location = 0) out vec4 o_Color;

in vec3 v_Position;
in vec2 v_UV;
in vec3 v_Normal;

uniform sampler2D u_Diffuse;
uniform vec4 u_AlbedoColor;

uniform sampler2D u_Roughness;
uniform sampler2D u_Metallic; // Add metallic texture sampler

uniform sampler2D u_Normal;

uniform sampler2D u_Ambient;
uniform vec4 u_AmbientColor;

uniform sampler2D u_Specular;

void main()
{
    vec4 albedoColor = texture(u_Diffuse, v_UV);
    vec3 ambientColor = texture(u_Ambient, v_UV).rgb;
    vec3 normalMap = texture(u_Normal, v_UV).rgb;
    float roughness = texture(u_Roughness, v_UV).r;
    float metallic = texture(u_Metallic, v_UV).r; // Sample metallic map

    // Apply ambient color
    vec3 ambient = ambientColor * u_AmbientColor.rgb;

    // Calculate specular intensity based on roughness and metallic
    float specularIntensity = 1.0 - roughness;
    vec3 specularColor = texture(u_Specular, v_UV).rgb;
    vec3 specular = specularColor * u_AlbedoColor.rgb * specularIntensity * metallic;

    // Calculate diffuse color
    vec3 diffuse = albedoColor.rgb * u_AlbedoColor.rgb * (1.0 - metallic);

    // Calculate final color
    vec3 color = diffuse + ambient + specular;

    o_Color = vec4(color, albedoColor.a * u_AlbedoColor.a);
}
