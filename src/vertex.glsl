#version 150

in vec2 position;
in vec3 color;
in vec2 tex_coord;

out vec3 vert_color;
out vec2 vert_tex_coord;

uniform float time;

void main()
{
    gl_Position = vec4(position, 0.0f, 1.0f);
    vert_color = color;
    vert_tex_coord = tex_coord;
}
