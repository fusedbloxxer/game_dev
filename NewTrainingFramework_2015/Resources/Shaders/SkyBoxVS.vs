attribute vec3 a_binormL;
attribute vec3 a_colorL;
attribute vec3 a_normL;
attribute vec3 a_tgtL;
attribute vec3 a_posL;
attribute vec2 a_uvL;

uniform mat4 u_matrix;
uniform samplerCube u_texture_0;

varying vec3 v_coord;

void main()
{
	v_coord = a_posL;
	gl_Position = u_matrix * vec4(a_posL, 1.0);
}