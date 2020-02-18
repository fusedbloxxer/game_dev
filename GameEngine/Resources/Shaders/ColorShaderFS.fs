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

// ---- Light Effects -----
uniform vec3 u_c_amb;
uniform float u_r_amb;

uniform float u_kdif;
uniform float u_kspec;

struct LightSource
{
	vec3 direction;

	vec3 diffusion;

	vec3 specular;
	float specular_power;

	vec3 associated_object_pos;
	
	float inner_cutoff;
	float outer_cutoff;
	float type;
};

const int MAX_LIGHT_SOURCES = 8;
uniform LightSource u_lights[MAX_LIGHT_SOURCES];

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
	vec3 comp_diff_final, comp_spec_final;

	for (int i = 0; i < MAX_LIGHT_SOURCES; ++i)
	{
		// Diffusion component
		vec3 light_direction = (u_lights[i].type  == 2.0 || u_lights[i].type == 3.0) ? normalize(v_pos.xyz - u_lights[i].direction) : normalize(u_lights[i].direction);
		vec3 comp_diff = obj_color.xyz * u_lights[i].diffusion * max(dot(v_normal, -light_direction), 0.0);
	
		// Specular component
		vec3 reflected_light = reflect(light_direction, v_normal);
		vec3 comp_spec = u_lights[i].specular * pow(max(dot(reflected_light, v_to_camera), 0.0), u_lights[i].specular_power);

		if (u_lights[i].type == 3.0)
		{
			float theta = dot(light_direction, normalize(u_lights[i].associated_object_pos - u_lights[i].direction));
			float epsilon = u_lights[i].inner_cutoff - u_lights[i].outer_cutoff;
			float intensity = clamp((theta - u_lights[i].outer_cutoff) / epsilon, 0.0, 1.0);

			comp_diff *= intensity;
			comp_spec *= intensity;
		}

		comp_diff_final += comp_diff;
		comp_spec_final += comp_spec;
	}

	// Final component = summing all light components
	obj_color = vec4(u_r_amb * comp_amb + (1.0 - u_r_amb) * (u_kdif  * comp_diff_final + u_kspec * comp_spec_final), 1.0);

	// Apply Fog
	float distance  = distance(vec4(u_camera, 1.0), v_pos);
	float alpha = (clamp(distance, u_fog_r, u_fog_R) - u_fog_r) / (u_fog_R - u_fog_r); 
	gl_FragColor = alpha * vec4(u_fog_color, 1.0) + (1.0 - alpha) * obj_color;	
}