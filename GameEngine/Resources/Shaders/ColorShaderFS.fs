precision highp float;

varying vec4 v_pos;
varying vec4 v_Wnorm;
varying vec3 v_color;

uniform vec3 u_camera;
uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fog_color;
uniform float u_is_reflected;

uniform samplerCube u_texture_0;

// ---- Light effects ----
uniform vec3 u_c_amb;
uniform float u_r_amb;

uniform float u_kdif;
uniform float u_kspec;

// ---- Light component 0 ----
uniform vec3 u_c_diff_0;

uniform vec3 u_c_spec_0;
uniform float u_spec_power_0;

uniform vec3 u_light_dir_0;
uniform float u_light_type_0;

uniform vec3 u_assoc_obj_pos_0;
uniform float u_inner_cutoff_0;
uniform float u_outer_cutoff_0;

// ---- Light component 1 ----
uniform vec3 u_c_diff_1;

uniform vec3 u_c_spec_1;
uniform float u_spec_power_1;

uniform vec3 u_light_dir_1;
uniform float u_light_type_1;

uniform vec3 u_assoc_obj_pos_1;
uniform float u_inner_cutoff_1;
uniform float u_outer_cutoff_1;

// ---- Light component 2 ----
uniform vec3 u_c_diff_2;

uniform vec3 u_c_spec_2;
uniform float u_spec_power_2;

uniform vec3 u_light_dir_2;
uniform float u_light_type_2;

uniform vec3 u_assoc_obj_pos_2;
uniform float u_inner_cutoff_2;
uniform float u_outer_cutoff_2;

// ---- Light component 3 ----
uniform vec3 u_c_diff_3;

uniform vec3 u_c_spec_3;
uniform float u_spec_power_3;

uniform vec3 u_light_dir_3;
uniform float u_light_type_3;

uniform vec3 u_assoc_obj_pos_3;
uniform float u_inner_cutoff_3;
uniform float u_outer_cutoff_3;

// ---- Light component 4 ----
uniform vec3 u_c_diff_4;

uniform vec3 u_c_spec_4;
uniform float u_spec_power_4;

uniform vec3 u_light_dir_4;
uniform float u_light_type_4;

uniform vec3 u_assoc_obj_pos_4;
uniform float u_inner_cutoff_4;
uniform float u_outer_cutoff_4;

// ---- Light component 5 ----
uniform vec3 u_c_diff_5;

uniform vec3 u_c_spec_5;
uniform float u_spec_power_5;

uniform vec3 u_light_dir_5;
uniform float u_light_type_5;

uniform vec3 u_assoc_obj_pos_5;
uniform float u_inner_cutoff_5;
uniform float u_outer_cutoff_5;

// ---- Light component 6 ----
uniform vec3 u_c_diff_6;

uniform vec3 u_c_spec_6;
uniform float u_spec_power_6;

uniform vec3 u_light_dir_6;
uniform float u_light_type_6;

uniform vec3 u_assoc_obj_pos_6;
uniform float u_inner_cutoff_6;
uniform float u_outer_cutoff_6;

// ---- Light component 7 ----
uniform vec3 u_c_diff_7;

uniform vec3 u_c_spec_7;
uniform float u_spec_power_7;

uniform vec3 u_light_dir_7;
uniform float u_light_type_7;

uniform vec3 u_assoc_obj_pos_7;
uniform float u_inner_cutoff_7;
uniform float u_outer_cutoff_7;

void main()
{
	vec4 obj_color = vec4(v_color, 1.0);
	
	// Apply reflection
	if (u_is_reflected != 0.0)
	{
		vec3 vec_camera = v_pos.xyz - u_camera;
		vec3 dirReflect = reflect(normalize(-vec_camera), normalize(v_Wnorm.xyz));
		obj_color = textureCube(u_texture_0, dirReflect);
		
	}

	// Apply ambiental light
	vec3 comp_amb = obj_color.xyz * u_c_amb, v_normal = normalize(v_Wnorm.xyz), v_to_camera = normalize(u_camera - v_pos.xyz);
	
	// Diffusion component
	vec3 light_direction_0 = (u_light_type_0 == 2.0 || u_light_type_0 == 3.0) ? normalize(v_pos.xyz - u_light_dir_0) : normalize(u_light_dir_0);
	vec3 comp_diff_0 = obj_color.xyz * u_c_diff_0 * max(dot(v_normal, -light_direction_0), 0.0);
	
	// Specular component
	vec3 reflected_light_0 = reflect(light_direction_0, v_normal);
	vec3 comp_spec_0 = u_c_spec_0 * pow(max(dot(reflected_light_0, v_to_camera), 0.0), u_spec_power_0);

	if (u_light_type_0 == 3.0)
	{
		float theta = dot(light_direction_0, normalize(u_assoc_obj_pos_0 - u_light_dir_0));
		float epsilon = u_inner_cutoff_0 - u_outer_cutoff_0;
		float intensity = clamp((theta - u_outer_cutoff_0) / epsilon, 0.0, 1.0);

		comp_diff_0 *= intensity;
		comp_spec_0 *= intensity;
	}

	// Diffusion component
	vec3 light_direction_1 = (u_light_type_1 == 2.0 || u_light_type_1 == 3.0) ? normalize(v_pos.xyz - u_light_dir_1) : normalize(u_light_dir_1);
	vec3 comp_diff_1 = obj_color.xyz * u_c_diff_1 * max(dot(v_normal, -light_direction_1), 0.0);
	
	// Specular component
	vec3 reflected_light_1 = reflect(light_direction_1, v_normal);
	vec3 comp_spec_1 = u_c_spec_1 * pow(max(dot(reflected_light_1, v_to_camera), 0.0), u_spec_power_1);

	if (u_light_type_1 == 3.0)
	{
		float theta = dot(light_direction_1, normalize(u_assoc_obj_pos_1 - u_light_dir_1));
		float epsilon = u_inner_cutoff_1 - u_outer_cutoff_1;
		float intensity = clamp((theta - u_outer_cutoff_1) / epsilon, 0.0, 1.0);

		comp_diff_1 *= intensity;
		comp_spec_1 *= intensity;
	}

	// Diffusion component
	vec3 light_direction_2 = (u_light_type_2 == 2.0 || u_light_type_2 == 3.0) ? normalize(v_pos.xyz - u_light_dir_2) : normalize(u_light_dir_2);
	vec3 comp_diff_2 = obj_color.xyz * u_c_diff_2 * max(dot(v_normal, -light_direction_2), 0.0);
	
	// Specular component
	vec3 reflected_light_2 = reflect(light_direction_2, v_normal);
	vec3 comp_spec_2 = u_c_spec_2 * pow(max(dot(reflected_light_2, v_to_camera), 0.0), u_spec_power_2);

	if (u_light_type_2 == 3.0)
	{
		float theta = dot(light_direction_2, normalize(u_assoc_obj_pos_2 - u_light_dir_2));
		float epsilon = u_inner_cutoff_2 - u_outer_cutoff_2;
		float intensity = clamp((theta - u_outer_cutoff_2) / epsilon, 0.0, 1.0);

		comp_diff_2 *= intensity;
		comp_spec_2 *= intensity;
	}

	// Diffusion component
	vec3 light_direction_3 = (u_light_type_3 == 2.0 || u_light_type_3 == 3.0) ? normalize(v_pos.xyz - u_light_dir_3) : normalize(u_light_dir_3);
	vec3 comp_diff_3 = obj_color.xyz * u_c_diff_3 * max(dot(v_normal, -light_direction_3), 0.0);
	
	// Specular component
	vec3 reflected_light_3 = reflect(light_direction_3, v_normal);
	vec3 comp_spec_3 = u_c_spec_3 * pow(max(dot(reflected_light_3, v_to_camera), 0.0), u_spec_power_3);

	if (u_light_type_3 == 3.0)
	{
		float theta = dot(light_direction_3, normalize(u_assoc_obj_pos_3 - u_light_dir_3));
		float epsilon = u_inner_cutoff_3 - u_outer_cutoff_3;
		float intensity = clamp((theta - u_outer_cutoff_3) / epsilon, 0.0, 1.0);

		comp_diff_3 *= intensity;
		comp_spec_3 *= intensity;
	}

	// Diffusion component
	vec3 light_direction_4 = (u_light_type_4 == 2.0 || u_light_type_4 == 3.0) ? normalize(v_pos.xyz - u_light_dir_4) : normalize(u_light_dir_4);
	vec3 comp_diff_4 = obj_color.xyz * u_c_diff_4 * max(dot(v_normal, -light_direction_4), 0.0);
	
	// Specular component
	vec3 reflected_light_4 = reflect(light_direction_4, v_normal);
	vec3 comp_spec_4 = u_c_spec_4 * pow(max(dot(reflected_light_4, v_to_camera), 0.0), u_spec_power_4);

	if (u_light_type_4 == 3.0)
	{
		float theta = dot(light_direction_4, normalize(u_assoc_obj_pos_4 - u_light_dir_4));
		float epsilon = u_inner_cutoff_4 - u_outer_cutoff_4;
		float intensity = clamp((theta - u_outer_cutoff_4) / epsilon, 0.0, 1.0);

		comp_diff_4 *= intensity;
		comp_spec_4 *= intensity;
	}

	// Diffusion component
	vec3 light_direction_5 = (u_light_type_5 == 2.0 || u_light_type_5 == 3.0) ? normalize(v_pos.xyz - u_light_dir_5) : normalize(u_light_dir_5);
	vec3 comp_diff_5 = obj_color.xyz * u_c_diff_5 * max(dot(v_normal, -light_direction_5), 0.0);
	
	// Specular component
	vec3 reflected_light_5 = reflect(light_direction_5, v_normal);
	vec3 comp_spec_5 = u_c_spec_5 * pow(max(dot(reflected_light_5, v_to_camera), 0.0), u_spec_power_5);

	if (u_light_type_5 == 3.0)
	{
		float theta = dot(light_direction_5, normalize(u_assoc_obj_pos_5 - u_light_dir_5));
		float epsilon = u_inner_cutoff_5 - u_outer_cutoff_5;
		float intensity = clamp((theta - u_outer_cutoff_5) / epsilon, 0.0, 1.0);

		comp_diff_5 *= intensity;
		comp_spec_5 *= intensity;
	}

	// Diffusion component
	vec3 light_direction_6 = (u_light_type_6 == 2.0 || u_light_type_6 == 3.0) ? normalize(v_pos.xyz - u_light_dir_6) : normalize(u_light_dir_6);
	vec3 comp_diff_6 = obj_color.xyz * u_c_diff_6 * max(dot(v_normal, -light_direction_6), 0.0);
	
	// Specular component
	vec3 reflected_light_6 = reflect(light_direction_6, v_normal);
	vec3 comp_spec_6 = u_c_spec_6 * pow(max(dot(reflected_light_6, v_to_camera), 0.0), u_spec_power_6);

	if (u_light_type_6 == 3.0)
	{
		float theta = dot(light_direction_6, normalize(u_assoc_obj_pos_6 - u_light_dir_6));
		float epsilon = u_inner_cutoff_6 - u_outer_cutoff_6;
		float intensity = clamp((theta - u_outer_cutoff_6) / epsilon, 0.0, 1.0);

		comp_diff_6 *= intensity;
		comp_spec_6 *= intensity;
	}

	// Diffusion component
	vec3 light_direction_7 = (u_light_type_7 == 2.0 || u_light_type_7 == 3.0) ? normalize(v_pos.xyz - u_light_dir_7) : normalize(u_light_dir_7);
	vec3 comp_diff_7 = obj_color.xyz * u_c_diff_7 * max(dot(v_normal, -light_direction_7), 0.0);
	
	// Specular component
	vec3 reflected_light_7 = reflect(light_direction_7, v_normal);
	vec3 comp_spec_7 = u_c_spec_7 * pow(max(dot(reflected_light_7, v_to_camera), 0.0), u_spec_power_7);

	if (u_light_type_7 == 3.0)
	{
		float theta = dot(light_direction_7, normalize(u_assoc_obj_pos_7 - u_light_dir_7));
		float epsilon = u_inner_cutoff_7 - u_outer_cutoff_7;
		float intensity = clamp((theta - u_outer_cutoff_7) / epsilon, 0.0, 1.0);

		comp_diff_7 *= intensity;
		comp_spec_7 *= intensity;
	}

	// Final component = summing all light components
	obj_color = vec4(u_r_amb * comp_amb + (1.0 - u_r_amb) * (u_kdif  * (comp_diff_0 + comp_diff_1 + comp_diff_2 + comp_diff_3 + comp_diff_4 + comp_diff_5 + comp_diff_6 + comp_diff_7) + 
															 u_kspec * (comp_spec_0 + comp_spec_1 + comp_spec_2 + comp_spec_3 + comp_spec_4 + comp_spec_5 + comp_spec_6 + comp_spec_7)), 1.0);

	// Apply Fog
	float distance  = distance(vec4(u_camera, 1.0), v_pos);
	float alpha = (clamp(distance, u_fog_r, u_fog_R) - u_fog_r) / (u_fog_R - u_fog_r); 
	gl_FragColor = alpha * vec4(u_fog_color, 1.0) + (1.0 - alpha) * obj_color;	
}