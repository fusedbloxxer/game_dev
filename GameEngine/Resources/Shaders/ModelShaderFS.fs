precision highp float;

varying vec2 v_uv;
varying vec4 v_pos;

uniform vec3 u_camera;
uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fog_color;

uniform sampler2D u_texture_0;

void main()
{
	vec4 obj_color = texture2D(u_texture_0, v_uv);

	if (obj_color.a < 0.1)
	{
		discard;
	}

	float distance  = distance(vec4(u_camera, 1.0), obj_color);
	float alpha = (clamp(distance, u_fog_r, u_fog_R) - u_fog_r) / (u_fog_R - u_fog_r); 
	gl_FragColor = alpha * vec4(u_fog_color, 1.0) + (1.0 - alpha) * obj_color;	
}