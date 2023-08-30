uniform mat4 u_ViewProjectionMat;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_texcoords;
layout(location = 3) in mat4 in_ModelMat;

out vec3 v_color;
out vec2 v_texcoords;

void main(void)
{
    v_color = in_color;
    v_texcoords = in_texcoords;
    gl_Position = u_ViewProjectionMat * in_ModelMat * vec4 (in_position, 1.0);
}
