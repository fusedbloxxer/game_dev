precision highp float;

uniform vec3 u_camera;
uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fog_color;
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;

varying vec2 v_uv;
varying vec2 v_uv2;
varying vec4 v_pos;

void main()
{
	vec4 c_rock	 = texture2D(u_texture_0, v_uv);
	vec4 c_grass = texture2D(u_texture_1, v_uv);
	vec4 c_dirt	 = texture2D(u_texture_2, v_uv);
	vec4 c_blend = texture2D(u_texture_3, v_uv2);

	vec4 obj_color = c_blend.r * c_rock + c_blend.g * c_grass + c_blend.b * c_dirt;
	obj_color.a = 1.0;

	float distance  = distance(vec4(u_camera, 1.0), v_pos);
	float alpha = (clamp(distance, u_fog_r, u_fog_R) - u_fog_r) / (u_fog_R - u_fog_r); 
	gl_FragColor = alpha * vec4(u_fog_color, 1.0) + (1.0 - alpha) * obj_color;
}