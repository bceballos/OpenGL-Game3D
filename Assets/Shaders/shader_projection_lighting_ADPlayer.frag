#version 440 core
out vec4 vertColour;	//output colour of vertex
in vec2 textureCoordinate; //tex coords from vertex shader
in vec3 normals;
in vec3 fragmentPosition;
in vec3 lightColour;
in vec3 lightPosition;

in vec3 lightColorTwo;
in vec3 lightPositionTwo;

uniform sampler2D aTex;		//uniform holding texture info from main programme



void main()
{
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
	
	//combine this with the light colour
	//set the diffuse coeff from material
	vec3 objectDiffuseReflectionCoeff = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse = (diffuseStrength * objectDiffuseReflectionCoeff) * lightColour;
	
	
	
	
	
	//ambient component Two
	//********************************
	//set the ambient coeff from material
	float lightAmbientStrengthTwo = 0.3f;
	vec3 objectAmbientReflectionCoeffTwo = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambientTwo = (lightAmbientStrengthTwo * objectAmbientReflectionCoeffTwo) * lightColorTwo;
	
	//diffuse component
	//********************************
	//calculate the light direction from the light position and the fragment position
    vec3 lightDirectionTwo = normalize(lightPositionTwo - fragmentPosition);
	
	//determine the dot product of normal direction and light direction
	float diffuseStrengthTwo = max(dot(nNormal, lightDirectionTwo), 0.0f);
	
	//combine this with the light colour
	//set the diffuse coeff from material
	vec3 objectDiffuseReflectionCoeffTwo = vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuseTwo = (diffuseStrengthTwo * objectDiffuseReflectionCoeffTwo) * lightColorTwo;
	
	vec4 textureColour = texture(aTex, textureCoordinate);
	
	//apply no lighting, ambient and diffuse components with colour contributed by texture
	//vertColour = (vec4((lightColour), 1.0) * textureColour);
	//vertColour = (vec4((ambient),1.0) * textureColour);
	vertColour = (vec4((ambient+diffuse+ambientTwo+diffuseTwo),1.0) * textureColour);
	
	
}