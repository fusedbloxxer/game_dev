precision highp float;

uniform sampler2D u_texture;
uniform float u_isTexture;

varying vec3 v_color;
varying vec2 v_uv;

void main()
{
	if (u_isTexture != 0.0)
	{
		vec4 vecColor = texture2D(u_texture, v_uv);

		if (vecColor.a < 0.1)
		{
			discard;
		}

		gl_FragColor = vecColor;
	}
	else
	{
		gl_FragColor = vec4(v_color, 1.0);
	}
}