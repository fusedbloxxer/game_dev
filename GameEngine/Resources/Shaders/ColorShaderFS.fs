precision highp float;

varying vec4 v_pos;
varying vec3 v_color;

uniform vec3 u_camera;
uniform float u_fog_r;
uniform float u_fog_R;
uniform vec3 u_fog_color;

void main()
{
	vec4 obj_color = vec4(v_color, 1.0);
	float distance  = distance(vec4(u_camera, 1.0), obj_color);
	float alpha = (clamp(distance, u_fog_r, u_fog_R) - u_fog_r) / (u_fog_R - u_fog_r); 
	gl_FragColor = alpha * vec4(u_fog_color, 1.0) + (1.0 - alpha) * obj_color;	
}