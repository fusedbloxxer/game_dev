attribute vec2 a_posL;

uniform sampler2D u_texture_0;
varying vec2 v_uv;

void main()
{
	v_uv = (a_posL + 1.0) / 2.0;
	gl_Position = vec4(a_posL, 0.0, 1.0);
}