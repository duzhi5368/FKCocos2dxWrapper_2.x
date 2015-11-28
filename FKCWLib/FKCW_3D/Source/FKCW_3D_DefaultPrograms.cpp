//-------------------------------------------------------------------------
#include "../Include/FKCW_3D_DefaultPrograms.h"
//-------------------------------------------------------------------------
const char* TexOnlyVert ="								  \n\
	attribute vec4 a_position;							  \n\
	attribute vec2 a_texCoord;							  \n\
	attribute vec4 a_normal;							  \n\
	attribute vec4 a_color;								  \n\
														  \n\
	varying vec2 v_texCoord;							  \n\
	varying vec4 v_fragmentColor;						  \n\
	void main()											  \n\
	{													  \n\
		gl_Position = CC_MVPMatrix * a_position;		  \n\
		v_texCoord = a_texCoord;						  \n\
		v_fragmentColor = a_color;						  \n\
	}";
//-------------------------------------------------------------------------
const char* TexOnlyFrag = "																	\n\
	#ifdef GL_ES																			\n\
		varying mediump vec2 v_texCoord;													\n\
		varying lowp vec4 v_fragmentColor;													\n\
	#else																					\n\
		varying  vec2 v_texCoord;															\n\
		varying  vec4 v_fragmentColor;														\n\
	#endif																					\n\
																							\n\
	uniform  sampler2D CC_Texture0;															\n\
																							\n\
	void main()																				\n\
	{																						\n\
		gl_FragColor =  texture2D(CC_Texture0, v_texCoord)*v_fragmentColor;					\n\
	}";
//-------------------------------------------------------------------------
const char* ClassicLightingVert = "																					 \n\
	//attribute																										 \n\
	attribute vec4 a_position;																						 \n\
	attribute vec4 a_normal;																						 \n\
	attribute vec2 a_texCoord;																						 \n\
	attribute vec4 a_color;																							 \n\
	//uniform																										 \n\
	uniform mat4 modelMat;																							 \n\
	uniform mat4 normalMat;																							 \n\
	uniform vec4 lightPos_world;																					 \n\
	uniform vec4 eyePos_world;																						 \n\
	uniform vec4 diffuseMaterial;																					 \n\
	uniform vec4 diffuseLight;																						 \n\
	uniform vec4 ambientMaterial;																					 \n\
	uniform vec4 ambientLight;																						 \n\
	uniform vec4 specularMaterial;																					 \n\
	uniform vec4 specularLight;																						 \n\
	uniform float shininess;																						 \n\
	//varying (pass to fragment shader)																				 \n\
	varying vec4 v_mainColor;																						 \n\
	varying vec4 v_secondaryColor;																					 \n\
	varying vec2 v_texCoord;																						 \n\
																													 \n\
	void main(void)																									 \n\
	{																												 \n\
		vec4 diffuseML=vec4(vec3(diffuseMaterial)*vec3(diffuseLight),diffuseMaterial.a);							 \n\
		vec4 ambientML=ambientMaterial*ambientLight;																 \n\
		vec4 specularML=specularMaterial*specularLight;																 \n\
		//----get normal in world space																				 \n\
		//if scaling is uniformed, we can use modelMat instead of normalMat											 \n\
		//if no scaling, we need not normalization																	 \n\
		vec4 norm_world = vec4(normalize(vec3(normalMat*a_normal)),0.0);											 \n\
		//----get pos in world space																				 \n\
		vec4 pos_world = modelMat*a_position;																		 \n\
		//----lightPos already in world space																		 \n\
		//----calculate diffuse color																				 \n\
		vec4 posToLight=vec4(normalize(vec3(lightPos_world-pos_world)),0.0);										 \n\
		float normDotPosToLight = max(0.0, dot(norm_world, posToLight));											 \n\
		vec4 diffuseColor= vec4(normDotPosToLight*vec3(diffuseML),diffuseML.a);										 \n\
		//----calculate ambient color																				 \n\
		vec4 ambientColor=vec4(vec3(ambientML),1.0);																 \n\
		//----calculate specular color																				 \n\
		vec4 posToEye=vec4(normalize(vec3(eyePos_world-pos_world)),0.0);											 \n\
		vec4 halfVector=(posToLight+posToEye)*0.5;																	 \n\
		float normDotHalfVector=max(0.0,dot(norm_world, halfVector));												 \n\
		float pf=normDotHalfVector==0.0?0.0:pow(normDotHalfVector,shininess);										 \n\
		vec4 specularColor= vec4(pf*vec3(specularML),1.0);															 \n\
		//----set varying																							 \n\
		//the final alpha is equal to diffuseColor.a																 \n\
		//bug fix: change vec3(ambientColor)+vec3(diffuseColor)*vec3(a_color) to 									 \n\
		// (vec3(ambientColor)+vec3(diffuseColor))*vec3(a_color)													 \n\
		v_mainColor = vec4((vec3(ambientColor)+vec3(diffuseColor))*vec3(a_color),diffuseColor.a*a_color.a);			 \n\
		float secondaryColorAlpha=pf;																				 \n\
		v_secondaryColor=vec4(vec3(specularColor),secondaryColorAlpha);												 \n\
		v_texCoord = a_texCoord;																					 \n\
		gl_Position = CC_MVPMatrix * a_position;																	 \n\
	}";
//-------------------------------------------------------------------------
const char* ClassicLightingFrag = "																\n\
	#ifdef GL_ES																				\n\
		varying lowp vec4 v_mainColor;															\n\
		varying lowp vec4 v_secondaryColor;														\n\
		varying mediump vec2 v_texCoord;														\n\
	#else																						\n\
		varying  vec4 v_mainColor;																\n\
		varying  vec4 v_secondaryColor;															\n\
		varying  vec2 v_texCoord;																\n\
	#endif																						\n\
																								\n\
	//sampler uniform																			\n\
	uniform sampler2D CC_Texutre0;																\n\
																								\n\
	void main(void) {																			\n\
		gl_FragColor = v_mainColor * texture2D(CC_Texutre0, v_texCoord)+v_secondaryColor;		\n\
	}";
//-------------------------------------------------------------------------
FKCW_3D_Program* GetProgramTexOnly()
{
	string programName="TexOnly";
	FKCW_3D_Program* program=(FKCW_3D_Program*)CCShaderCache::sharedShaderCache()->programForKey(programName.c_str());
	if(program==NULL)
	{
		program=new FKCW_3D_Program();
		program->autorelease();
		program->init(TexOnlyVert, TexOnlyFrag);
		program->setName(programName);
		//add to shader cache
		CCShaderCache::sharedShaderCache()->addProgram(program, program->getName().c_str());
	}
	return program;
}
//-------------------------------------------------------------------------
FKCW_3D_Program* GetProgramClassicLighting()
{
	string programName="ClassicLighting";
	FKCW_3D_Program* program=(FKCW_3D_Program*)CCShaderCache::sharedShaderCache()->programForKey(programName.c_str());
	if(program==NULL)
	{
		program=new FKCW_3D_Program();
		program->autorelease();
		program->init(ClassicLightingVert, ClassicLightingFrag);
		program->setName(programName);
		//attach uniforms
		program->attachUniform("modelMat");
		program->attachUniform("normalMat");
		program->attachUniform("lightPos_world");
		program->attachUniform("eyePos_world");
		program->attachUniform("diffuseMaterial");
		program->attachUniform("diffuseLight");
		program->attachUniform("ambientMaterial");
		program->attachUniform("ambientLight");
		program->attachUniform("specularMaterial");
		program->attachUniform("specularLight");
		program->attachUniform("shininess");
		//add to shader cache
		CCShaderCache::sharedShaderCache()->addProgram(program, program->getName().c_str());
	}
	return program;
}
//-------------------------------------------------------------------------