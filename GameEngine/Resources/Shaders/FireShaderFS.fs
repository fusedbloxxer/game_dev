precision highp float;

varying vec2 v_uv;
varying vec4 v_pos;
varying vec4 v_Wnorm;

uniform vec3 u_camera;
uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fog_color;
uniform float u_is_reflected;

uniform float u_time;
uniform float u_disp_max;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform samplerCube u_texture_skybox;

void main()
{
	vec2 disp = texture2D(u_texture_0, vec2(v_uv.x, v_uv.y + u_time)).yz;

	vec2 offset = ((disp * 2.0) - 1.0) * u_disp_max;
	
	vec2 v_uv_displaced = v_uv + offset;
	vec4 obj_color = texture2D(u_texture_1, v_uv_displaced);

	vec4 c_alpha =  texture2D(u_texture_2, v_uv);
	obj_color.w = obj_color.w * c_alpha.y;

	// Discard if transparent
	if (obj_color.a < 0.1)
	{
		discard;
	}

	// Apply reflection
	if (u_is_reflected != 0.0)
	{
		vec3 vec_camera = v_pos.xyz - u_camera;
		vec3 dirReflect = reflect(normalize(vec_camera), normalize(v_Wnorm.xyz));
		obj_color = obj_color * 0.5 + textureCube(u_texture_skybox, dirReflect) * 0.5;
		obj_color.w = 1.0;
	}

	// Apply fog
	float distance  = distance(vec4(u_camera, 1.0), v_pos);
	float alpha = (clamp(distance, u_fog_r, u_fog_R) - u_fog_r) / (u_fog_R - u_fog_r); 
	gl_FragColor = alpha * vec4(u_fog_color, 1.0) + (1.0 - alpha) * obj_color;	
}