attribute vec2 a_uvL;
attribute vec3 a_posL;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_projection;

varying vec2 v_uv;
varying vec4 v_pos;

// Normal Mapping
attribute vec3 a_tgtL;
attribute vec3 a_normL;
attribute vec3 a_binormL;

varying vec3 v_tgt;
varying vec3 v_Wnorm;
varying vec3 v_binorm;


void main()
{
	vec4 position = vec4(a_posL, 1.0);

	v_uv = a_uvL;
	v_pos = u_model * position;

	v_tgt = vec3(u_model * vec4(a_tgtL, 0.0));
	v_Wnorm = vec3(u_model * vec4(a_normL, 0.0)); 
	v_binorm = vec3(u_model * vec4(a_binormL, 0.0));

	gl_Position =  u_projection * u_view * u_model * position;
}