attribute vec3 a_colorL;
attribute vec3 a_posL;

uniform mat4 u_matrix;

varying vec3 v_color;

void main()
{
	gl_Position = u_matrix * vec4(a_posL, 1.0);
	v_color = a_colorL;
}