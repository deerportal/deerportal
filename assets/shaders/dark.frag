uniform sampler2D source;

void main()
{
        vec4 src = texture2D(source, gl_TexCoord[0].xy);
        // https://en.wikipedia.org/wiki/Relative_luminance
        float luminance = src.r * 0.2126 + src.g * 0.7152 + src.b * 0.0722;
        src *= clamp(luminance - 0.5, 0.0, 1.0) * 2;
        gl_FragColor = src;
}
