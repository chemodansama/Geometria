uniform mat4 u_ModelviewProjectionMat;

layout(location = 0) in vec2 position;

void main(void)
{
    gl_Position = u_ModelviewProjectionMat * vec4 (position, 0.0, 1.0);
}