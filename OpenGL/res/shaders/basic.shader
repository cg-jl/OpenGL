//$shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main() {
        gl_Position = position;
};

//$shader fragment
#version 330 core

// location -> 0 (index of attribute)
// vec4 bc of glPosition  is vec4
layout(location = 0) out vec4 color;
uniform vec4 u_color;

void main() {
    // assign color to the color from CPU
     color = u_color;
};