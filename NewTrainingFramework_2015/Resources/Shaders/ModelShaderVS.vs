attribute float a_isTextureL;

attribute vec3 a_binormL;
attribute vec3 a_colorL;
attribute vec3 a_normL;
attribute vec3 a_tgtL;
attribute vec3 a_posL;
attribute vec2 a_uvL;

uniform mat4 u_matrix;
uniform sampler2D u_texture;

varying vec2 v_uv;
varying vec3 v_color;
varying float v_isTexture;

void main()
{
	gl_Position = u_matrix * vec4(a_posL, 1.0);
	v_isTexture = a_isTextureL;

	if (v_isTexture != 0.0)
	{
		v_uv = a_uvL;
	}
	else
	{
		v_color = a_colorL;
	}
}