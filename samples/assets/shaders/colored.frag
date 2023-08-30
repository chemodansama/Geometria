uniform sampler2D u_Texture;

in vec3 v_color;
in vec2 v_texcoords;

out vec4 out_color;

void main(void) 
{
    out_color = vec4(v_color, 1.0f) * texture(u_Texture, v_texcoords);
}