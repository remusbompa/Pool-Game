#version 330

// TODO: get values from fragment shader
in vec3 color;
in vec3 position_out;


uniform float tableSizeX;
uniform float tableSizeZ;
uniform int object;

uniform vec3 ball;
uniform float interAxis;
uniform float radius;

layout(location = 0) out vec4 out_color;

void main()
{
	// TODO: write pixel out color
	vec3 newColor = color;
	out_color = vec4(newColor, 1);

	if(object == 1){
		if(!(position_out.x > -tableSizeX / 2 && position_out.x < tableSizeX / 2 &&
		position_out.z > -tableSizeZ / 2 && position_out.z < tableSizeZ / 2)){
			discard;
		}

		float dist = distance(position_out, ball);
		float fnr = dist / interAxis;
		highp int nr = int(fnr);
		if(nr %2 == 0) discard;
	}
}