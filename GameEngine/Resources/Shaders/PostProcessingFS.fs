precision mediump float;

uniform sampler2D u_texture_0;

varying vec2 v_uv;

void main()
{
   vec4 sample0,
         sample1,
         sample2,
         sample3;

   float step = 0.25 / 100.0;

   sample0 = texture2D(u_texture_0, vec2(v_uv.x - step, v_uv.y - step));
   sample1 = texture2D(u_texture_0, vec2(v_uv.x - step, v_uv.y + step));
   sample2 = texture2D(u_texture_0, vec2(v_uv.x + step, v_uv.y - step));
   sample3 = texture2D(u_texture_0, vec2(v_uv.x + step, v_uv.y + step));

   // v_uv.x += sin(v_uv.y * 4.0 * 2.0 * 3.14159 + 0.25) / 100.0;

   gl_FragColor = texture2D(u_texture_0, vec2(v_uv.x + sin(v_uv.y * 4.0 * 2.0 * 3.14159 + 0.25) / 100.0, v_uv.y)); 
   // (sample0 + sample1 + sample2 + sample3) / 4.0;
}