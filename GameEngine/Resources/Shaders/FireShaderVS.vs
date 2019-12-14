attribute vec2 a_uvL;
attribute vec3 a_posL;
attribute vec3 a_normL;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_projection;

varying vec2 v_uv;
varying vec4 v_pos;
varying vec4 v_Wnorm;

void main()
{
	vec4 position = vec4(a_posL, 1.0);

	v_uv = a_uvL;
	v_pos = u_model * position;
	v_Wnorm = u_model * vec4(a_normL, 0.0); 
	gl_Position =  u_projection * u_view * u_model * position;
}