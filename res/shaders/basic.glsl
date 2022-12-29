//$shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main() {
  gl_Position = position;
}

//$shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform float u_time;
uniform vec2 iResolution;

void main() {
    vec2 uv = gl_FragCoord.xy/iResolution.xy;

    vec3 col = 0.5 + 0.5*cos(u_time+uv.xyx+vec3(0,2,4));
    color = vec4(col, 1.0);
}
