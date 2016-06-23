#version 150

in vec3 vert_color;
in vec2 vert_tex_coord;

out vec4 frag_color;

uniform sampler2D tex_kitten;
uniform sampler2D tex_puppy;
uniform float time;

void main()
{
    vec4 color_kitten;

    if (vert_tex_coord.y > 0.5f) {
        color_kitten = texture(tex_kitten, vec2(vert_tex_coord.x + sin(vert_tex_coord.y * 60.0f + time * 2.0f) / 30.0f, 1.0f - vert_tex_coord.y)) * vec4(0.7f, 0.7f, 1.0f, 1.0f);
//        color_kitten = texture(tex_kitten, vert_tex_coord);
//        vec4 color_puppy = texture(tex_puppy, vert_tex_coord);
//        frag_color = mix(color_kitten, color_puppy, (sin(time * 4.0f) + 1.0f) / 2.0f);
    }
    else {
        color_kitten = texture(tex_kitten, vert_tex_coord);
    }

    frag_color = color_kitten;
}
