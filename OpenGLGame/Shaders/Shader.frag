#version 150

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;

void main(){
	outColor = texture(tex, -Texcoord); //  Multiply by color to color a black/white bmp* vec4(Color, 1.0)
}
