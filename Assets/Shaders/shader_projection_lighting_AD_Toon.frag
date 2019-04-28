#version 440 core
out vec4 vertColour;	//output colour of vertex
in vec2 textureCoordinate; //tex coords from vertex shader
in vec3 normals;
in vec3 fragmentPosition;
in vec3 lightColour;
in vec3 lightPosition;

uniform sampler2D aTex;		//uniform holding texture info from main programme
uniform float uTime;

float random (vec2 input) {
    return fract(sin(dot(input.xy, vec2(12.9898,78.233)))* 43758.5453123);
}


void main()
{
	vec3 toon;

	//ambient component
	//********************************
	//set the ambient coeff from material
	float lightAmbientStrength = 0.3f;
	vec3 objectAmbientReflectionCoeff = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient = (lightAmbientStrength * objectAmbientReflectionCoeff) * lightColour;

	//diffuse component
	//********************************
	//normalise normal vectors (reset them as unit vectors)
	vec3 nNormal = normalize(normals);
	//calculate the light direction from the light position and the fragment position
    vec3 lightDirection = normalize(lightPosition - fragmentPosition);

	//determine the dot product of normal direction and light direction
	float diffuseStrength = max(dot(nNormal, lightDirection), 0.0f);

		 if (diffuseStrength > 0.95) {toon = vec3(1.0, 1.0, 1.0);}
else if (diffuseStrength > 0.75) {toon = vec3(0.8, 0.8, 0.8);}
else if (diffuseStrength > 0.50) {toon = vec3(0.6, 0.6, 0.6);}
else if (diffuseStrength > 0.25) {toon = vec3(0.4, 0.4, 0.4);}
else 							 {toon = vec3(0.2, 0.2, 0.2);}

	//combine this with the light colour
	//set the diffuse coeff from material
	vec3 objectDiffuseReflectionCoeff = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse = (diffuseStrength * objectDiffuseReflectionCoeff) * (lightColour * toon);

  float random1 = random(vec2(uTime, uTime));
	float random2 = random(vec2(uTime + 420, uTime - 69));
	float random3 = random(vec2(uTime - 64, uTime + 128));

	vec4 textureColour = texture(aTex, textureCoordinate);

	textureColour.x *= random1;
	textureColour.y *= random2;
	textureColour.z *= random3;

	//apply no lighting, ambient and diffuse components with colour contributed by texture
	//vertColour = (vec4((lightColour), 1.0) * textureColour);
	//vertColour = (vec4((ambient),1.0) * textureColour);
	vertColour = (vec4((ambient+diffuse),1.0) * textureColour);


}
