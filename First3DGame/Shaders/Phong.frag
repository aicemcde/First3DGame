#version 330

struct DirectionalLight
{
	vec3 mDirection;
	vec3 mDiffuseColor;
	vec3 mSpecColor;
};

struct PointLight
{
	vec3 mWorldPos;
	vec3 mDiffuseColor;
	vec3 mSpecColor;
	float mConstant;
	float mLinear;
	float mQuadratic;
};

in vec2 fragTexCoord;
//–@ü
in vec3 fragNormal;
//À•W
in vec3 fragWorldPos;

out vec4 outColor;

uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform float uSpecPower;
uniform DirectionalLight uDirLight;
uniform PointLight uPointLight;
uniform sampler2D uTexture;

vec3 CalcPointLight(PointLight light, vec3 N, vec3 fragPos, vec3 V)
{
	vec3 L = normalize(light.mWorldPos - fragPos);
	vec3 R = normalize(reflect(-L, N));

	float distance = length(light.mWorldPos - fragPos);
	float attenuation = 1.0 / (light.mConstant + light.mLinear * distance + light.mQuadratic * (distance * distance));

	float NdotL = dot(N, L);
	if (NdotL > 0.0)
	{
		vec3 Diffuse = light.mDiffuseColor * NdotL * attenuation;
		vec3 Specular = light.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower) * attenuation;
		return (Diffuse + Specular);
	}
	return vec3(0.0);
}

void main()
{
	vec3 N = normalize(fragNormal);
	vec3 L = normalize(-uDirLight.mDirection);
	vec3 V = normalize(uCameraPos - fragWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	Phong += CalcPointLight(uPointLight, N, fragWorldPos, V);

	outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}