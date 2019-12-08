attribute vec3 a_posL;
attribute vec3 a_colorL;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_projection;

varying vec4 v_pos;
varying vec3 v_color;

void main()
{
	vec4 position = vec4(a_posL, 1.0);

	v_color = a_colorL;
	v_pos = u_model * position;
	gl_Position = u_projection * u_view * u_model * position;
}