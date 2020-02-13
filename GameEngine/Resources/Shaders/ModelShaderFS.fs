precision highp float;

varying vec2 v_uv;
varying vec4 v_pos;
varying vec4 v_Wnorm;

uniform vec3 u_camera;

// ---- Fog effect ----
uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fog_color;

// ---- Reflections ----
uniform float u_is_reflected;

// ---- Light effects ----
uniform vec3 u_c_amb;
uniform float u_r_amb;

uniform vec3 u_c_spec_0;
uniform float u_spec_power_0;

uniform vec3 u_c_diff_0;

uniform vec3 u_light_dir_0;

// ---- Skybox ----
uniform sampler2D u_texture_0;
uniform samplerCube u_texture_skybox;

void main()
{
	vec4 obj_color = texture2D(u_texture_0, v_uv);

	// Discard if transparent
	if (obj_color.a < 0.1)
	{
		discard;
	}

	// Apply reflection
	if (u_is_reflected != 0.0)
	{
		vec3 vec_camera = v_pos.xyz - u_camera;
		vec3 dirReflect = reflect(normalize(-vec_camera), normalize(v_Wnorm.xyz));
		obj_color = obj_color * 0.5 + textureCube(u_texture_skybox, dirReflect) * 0.5;
		obj_color.w = 1.0;
	}

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
	float distance  = distance(vec4(u_camera, 1.0), obj_color);
	float alpha = (clamp(distance, u_fog_r, u_fog_R) - u_fog_r) / (u_fog_R - u_fog_r); 
	gl_FragColor = alpha * vec4(u_fog_color, 1.0) + (1.0 - alpha) * obj_color;	
}