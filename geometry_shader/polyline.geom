#version 330

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 4) out;

const float WIDTH = 0.1;

void main()
{
    vec2 prev = gl_in[0].gl_Position.xy;
    vec2 start = gl_in[1].gl_Position.xy;
    vec2 end = gl_in[2].gl_Position.xy;
    vec2 next = gl_in[3].gl_Position.xy;

    vec2 prevNorm = normalize(vec2(-(start.y - prev.y), start.x - prev.x));
    vec2 curNorm = normalize(vec2(-(end.y - start.y), end.x - start.x));
    vec2 nextNorm = normalize(vec2(-(next.y - end.y), next.x - end.x));

    // Here I use the average of two vectors' norms rather than themselves,
    // otherwise a zero vector would be produces when <prev,start,end> is a line.
    vec2 startNorm = normalize(prevNorm + curNorm);
    vec2 endNorm = normalize(curNorm + nextNorm);

    // Apply width
    startNorm *= WIDTH / 2 / dot(curNorm, startNorm);
    endNorm *= WIDTH / 2 / dot(curNorm, endNorm);

    gl_Position = vec4(start + startNorm, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(start - startNorm, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(end + endNorm, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(end - endNorm, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
