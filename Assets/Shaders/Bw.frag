varying vec4 v_vColour;
uniform sampler2D gm_BaseTexture;

void main()
{
    vec4 Color = texture2D( gm_BaseTexture, gl_TexCoord[0].xy );
    vec3 lum = vec3(0.299, 0.587, 0.114);
    float bw = dot( Color.rgb, lum);//black and white result
    gl_FragColor = vec4( bw * vec3( 1.0, 0.8, 0.4), Color.a);
}