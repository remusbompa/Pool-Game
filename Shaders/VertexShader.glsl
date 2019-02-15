#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;

layout(location = 3) in vec3 v_color;

layout(location = 1) in vec3 v_normal;

layout(location = 2) in vec2 v_texture;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float radius;

uniform int object;
uniform int animation;
uniform vec3 difPoz;
uniform float time;

// TODO: output values to fragment shader
out vec3 position_out;
out vec3 color;

void main()
{
	// TODO: send output to fragment shader
	position_out = v_position;
	color = v_color;
	// TODO: compute gl_Position
	gl_Position =  Model * vec4(v_position, 1.0);

	vec3 depl = normalize(difPoz) * ( length(difPoz) - radius );
	if(object == 0){
		if(animation == 1){
			gl_Position.x += -sin(2 * time) * depl.x;
			gl_Position.y += -sin(2 * time) * depl.y;
			gl_Position.z += -sin(2 * time) * depl.z;
		}
	}

	gl_Position = Projection * View * gl_Position;
}
