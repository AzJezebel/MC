#version 450 core

// input
in VertexData {
    vec2 texel;
    vec3 normal;
} vertexData;

// uniform
uniform sampler2D atlas;

// output
out vec4 color;

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec4 c)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.f);

	vec3 ambient = light.ambient * c.xyz;
	vec3 diffuse = light.diffuse * diff * c.xyz;

	return (ambient + diffuse);
}

void main()
{
    DirLight dirLight;
    dirLight.direction = vec3(-2.f, -4.f, -1.f);
    dirLight.ambient = vec3(0.0750000030f, 0.072353251f, 0.0614707507f);
    dirLight.diffuse = vec3(0.800000012f, 0.771768034f, 0.655687988f);

    vec4 c = texture(atlas, vertexData.texel);
        
    vec3 result = vec3(0.f);

    vec3 norm = vertexData.normal;

	result += CalcDirLight(dirLight, norm, c);

    color = vec4(result, c.a);
}