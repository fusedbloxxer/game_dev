attribute vec2 a_uvL;
attribute vec3 a_posL;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_projection;
uniform samplerCube u_texture_0;

varying vec2 v_uv;
varying vec3 v_coord;

void main()
{
	v_uv = a_uvL;
	v_coord = a_posL;
	gl_Position = u_projection * u_view * u_model * vec4(a_posL, 1.0);
}