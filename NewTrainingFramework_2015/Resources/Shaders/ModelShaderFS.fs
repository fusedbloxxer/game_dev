precision highp float;

uniform sampler2D u_texture_0;

varying vec3 v_color;
varying vec2 v_uv;

void main()
{
	vec4 vecColor = texture2D(u_texture_0, v_uv);

	if (vecColor.a < 0.1)
	{
		discard;
	}
	else
	{
		gl_FragColor = vecColor;	
	}
}