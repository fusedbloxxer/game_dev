precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_texture;

void main()
{
vec4 vecColor = texture2D(u_texture, v_uv);

if (vecColor.a < 0.1)
{
	discard;
}

gl_FragColor = vecColor;
}