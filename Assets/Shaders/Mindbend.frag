uniform float time;
uniform vec2 resolution;
uniform sampler2D tex0;

void main(void) {
	  vec2 tc = gl_TexCoord[0].xy;
	  vec2 p = -1.0 + 2.0 * tc;
	  float len = length(p);
	  vec2 uv = tc + (p/len)*cos(len*12.0-time*4.0)*0.03;
	  vec3 col = texture2D(tex0,uv).xyz;
	  gl_FragColor = vec4(col,1.0); 
}