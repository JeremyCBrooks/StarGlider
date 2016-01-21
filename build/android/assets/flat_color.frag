varying highp vec2 TexCoord0;
varying lowp vec4 ColorVec;
uniform lowp sampler2D tex0;
uniform highp vec2 TexOffset;

void main(void)
{
	gl_FragColor = ColorVec*texture2D(tex0,TexCoord0.st + TexOffset);
}