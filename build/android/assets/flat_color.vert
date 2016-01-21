attribute vec3 Position;
attribute vec2 InTexCoord0;
attribute vec4 ColorIn;

uniform mat4 Proj;
uniform mat4 Model;

varying highp vec2 TexCoord0;
varying lowp vec4 ColorVec;

void main(void)
{
	gl_Position = Proj * Model * vec4(Position, 1.0);

	TexCoord0 = InTexCoord0;
	ColorVec = ColorIn;
}