// attribute vec3 a_binormL;
// attribute vec3 a_colorL;
// attribute vec3 a_normL;
attribute vec3 a_tgtL;
attribute vec3 a_posL;
attribute vec2 a_uv2L;
attribute vec2 a_uvL;

uniform mat4 u_matrix;
uniform vec3 u_height;
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;

varying vec2 v_uv;
varying vec2 v_uv2;
// varying vec3 v_color;

void main()
{
	vec4 new_pos = vec4(a_posL, 1.0);
	vec4 c_blend = texture2D(u_texture_3, a_uv2L);
	new_pos.y += c_blend.r * u_height.r + c_blend.g * u_height.g + c_blend.b * u_height.b;

	gl_Position = u_matrix * new_pos;
	// v_color = a_colorL;
	v_uv2 = a_uv2L;
	v_uv = a_uvL;
}