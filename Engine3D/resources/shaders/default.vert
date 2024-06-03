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

void main()
{
    vec4 worldPosition = u_Model * vec4(in_Position, 1.0);
    
    gl_Position = u_Projection * u_View * worldPosition;

    v_Position = in_Position;
    v_UV = in_UV;
    v_Normal = in_Normal;
};