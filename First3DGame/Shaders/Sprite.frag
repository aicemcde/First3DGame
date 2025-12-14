#version 330

in vec2 fragTexCoord;
in vec3 fragColor;

out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
	vec4 textureColor = texture(uTexture, fragTexCoord);

	vec3 averageRGB = (textureColor.rgb + fragColor) * 0.5;

	outColor = vec4(averageRGB, textureColor.a);
}