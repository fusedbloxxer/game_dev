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

// ---- Light effects ----
uniform vec3 u_c_amb;
uniform float u_r_amb;

uniform vec3 u_c_spec_0;
uniform float u_spec_power_0;

uniform vec3 u_c_diff_0;

uniform vec3 u_light_dir_0;

void main()
{
	vec4 c_rock	 = texture2D(u_texture_0, v_uv);
	vec4 c_grass = texture2D(u_texture_1, v_uv);
	vec4 c_dirt	 = texture2D(u_texture_2, v_uv);
	vec4 c_blend = texture2D(u_texture_3, v_uv2);

	vec4 obj_color = c_blend.r * c_rock + c_blend.g * c_grass + c_blend.b * c_dirt;
	obj_color.a = 1.0;

	// Apply ambiental light
	vec3 Comp_amb = u_c_amb * obj_color.xyz;

	// Diffusion component
	vec3 L = normalize(u_light_dir_0);
	vec3 N = normalize(v_pos.xyz);

	vec3 Comp_diff = obj_color.xyz * u_c_diff_0 * max(dot(N, -L), 0.0);

	// Specular component
	vec3 E = normalize(v_pos.xyz - u_camera);
	vec3 R = reflect(L, N);

	vec3 Comp_spec = u_c_spec_0 * pow(max(dot(R, E), 0.0), u_spec_power_0);

	vec3 Comp_final = u_r_amb * Comp_amb + (1.0 - u_r_amb) * (Comp_diff + Comp_spec);

	obj_color = vec4(Comp_final, 1.0);

	// Apply fog
	float distance  = distance(vec4(u_camera, 1.0), v_pos);
	float alpha = (clamp(distance, u_fog_r, u_fog_R) - u_fog_r) / (u_fog_R - u_fog_r); 
	gl_FragColor = alpha * vec4(u_fog_color, 1.0) + (1.0 - alpha) * obj_color;
}