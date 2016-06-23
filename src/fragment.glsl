#version 150

in vec3 vert_color;
in vec2 vert_tex_coord;

out vec4 frag_color;

uniform sampler2D tex;

void main()
{
    frag_color = texture(tex, vert_tex_coord) * vec4(vert_color, 1.0f);
}
