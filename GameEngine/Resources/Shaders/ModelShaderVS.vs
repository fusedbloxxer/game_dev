attribute vec2 a_uvL;
attribute vec3 a_posL;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_projection;
uniform sampler2D u_texture_0;

varying vec2 v_uv;
varying vec4 v_pos;

void main()
{
	vec4 position = vec4(a_posL, 1.0);

	v_uv = a_uvL;
	v_pos = u_model * position;
	gl_Position =  u_projection * u_view * u_model * position;
}