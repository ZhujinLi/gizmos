#version 330

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 4) out;

const float WIDTH = 0.05;

void main()
{
    vec2 prev = gl_in[0].gl_Position.xy;
    vec2 start = gl_in[1].gl_Position.xy;
    vec2 end = gl_in[2].gl_Position.xy;
    vec2 next = gl_in[3].gl_Position.xy;


    gl_Position = vec4(start.x, start.y + WIDTH * 0.5, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(start.x, start.y - WIDTH * 0.5, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(end.x, end.y + WIDTH * 0.5, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(end.x, end.y - WIDTH * 0.5, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
