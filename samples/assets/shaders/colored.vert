uniform mat4 u_ModelviewProjectionMat;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec2 in_texcoords;

out vec3 v_color;
out vec2 v_texcoords;

void main(void)
{
    v_color = in_color;
    v_texcoords = in_texcoords;
    gl_Position = u_ModelviewProjectionMat * vec4 (in_position, 1.0);
}
