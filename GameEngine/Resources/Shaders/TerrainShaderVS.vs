attribute vec3 a_posL;
attribute vec3 a_normL;
attribute vec3 a_binormL;
attribute vec3 a_tgtL;
attribute vec2 a_uv2L;
attribute vec2 a_uvL;

uniform mat4 u_view;
uniform mat4 u_model;
uniform vec3 u_height;
uniform mat4 u_projection;
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;

varying vec2 v_uv;
varying vec2 v_uv2;
varying vec4 v_pos;
varying vec4 v_Wnorm;

void main()
{
	vec4 new_pos = vec4(a_posL, 1.0);
	vec4 c_blend = texture2D(u_texture_3, a_uv2L);
	new_pos.y += c_blend.r * u_height.r + c_blend.g * u_height.g + c_blend.b * u_height.b;
	
	v_uv = a_uvL;
	v_uv2 = a_uv2L;
	v_pos = u_model *  new_pos;
	v_Wnorm = u_model * vec4(a_normL, 0.0);
	gl_Position = u_projection * u_view * u_model * new_pos;
}