// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430
struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

uniform MaterialInfo Material;
//lighting color, a uniform
uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float Shininess;//for material specular, also called specular strength
out vec4 vFragColor;



in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 UV;
in vec3 viewDir;
in vec3 halfwayDir;

void main(void)
{ 
    // Dot product gives us diffuse intensity (cos theta)
    float diff = max(0.0, dot(normalize(vVaryingNormal),
					normalize(vVaryingLightDir)));

    // Multiply intensity by diffuse color(Kd), force alpha to 1.0
    //diff*diffuse results in difuse color, and Kd is the objectColor
    vFragColor = diffuseColor*(diff*vec4(Material.Kd,1));

    // Add in ambient light
    vFragColor += ambientColor*vec4(Material.Ka,1);


    // Specular Light
    vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),
								normalize(vVaryingNormal)));//反射角
    //float spec = max(0.0, dot(normalize(viewDir), vReflection));
    float spec = max(0.0, dot(normalize(vVaryingNormal), halfwayDir));
    if(diff != 0) {
		spec = pow(spec, Shininess);
		vFragColor += specularColor*(spec*vec4(Material.Ks,1));
    }
}
	
    