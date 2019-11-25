precision highp float;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;

varying vec3 v_color;
varying vec2 v_uv2;
varying vec2 v_uv;

void main()
{
	vec4 c_rock	 = texture2D(u_texture_0, v_uv);
	vec4 c_grass = texture2D(u_texture_1, v_uv);
	vec4 c_dirt	 = texture2D(u_texture_2, v_uv);
	vec4 c_blend = texture2D(u_texture_3, v_uv2);

	vec4 c_final = c_blend.r * c_rock + c_blend.g * c_grass + c_blend.b * c_dirt;
	c_final.a = 1.0;

	gl_FragColor = c_final;
}