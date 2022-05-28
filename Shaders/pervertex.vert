#version 120

uniform mat4 modelToCameraMatrix;
uniform mat4 cameraToClipMatrix;
uniform mat4 modelToWorldMatrix;
uniform mat4 modelToClipMatrix;

uniform int active_lights_n; // Number of active lights (< MG_MAX_LIGHT)
uniform vec3 scene_ambient;  // rgb

uniform struct light_t {
	vec4 position;    // Camera space
	vec3 diffuse;     // rgb
	vec3 specular;    // rgb
	vec3 attenuation; // (constant, lineal, quadratic)
	vec3 spotDir;     // Camera space
	float cosCutOff;  // cutOff cosine
	float exponent;
} theLights[4];     // MG_MAX_LIGHTS

uniform struct material_t {
	vec3  diffuse;
	vec3  specular;
	float alpha;
	float shininess;
} theMaterial;

attribute vec3 v_position; // Model space
attribute vec3 v_normal;   // Model space
attribute vec2 v_texCoord;

varying vec4 f_color; //son variables de salida para el vertex shader 
varying vec2 f_texCoord;


void main() {
	vec4 normal_vertice = normalize(modelToCameraMatrix*vec3(v_normal,0));
	vec4 vertice_en_camara =  modelToCameraMatrix * vec4(v_position,1);
	vec3 color = vec3(scene_ambient);
	vec3 color_difuso = vec3(0);
	vec3 color_especular = vec3(0);
	vec4 v = vec4(0,0,0,1);
	v-=vertice_en_camara;
	
	for(int i =0; i< active_lights_n; i++){
		vec4 pos_luz = theLights[i].position;
		if (pos_luz.w == 0.0){
			pos_luz = normalize(-pos_luz);
			direccional(pos_luz.xyz,normal_vertice.xyz,v.xyz,color_difuso,color_especular,theLights[i].diffuse,theLights[i].specular, theMaterial.shininess );
		}else {
			vec3 I = pos_luz.xyz-vertice_en_camara.xyz;
			float dist = length(I)
			I/= dist;
			if(theLights[i].cosCutOff>0){//para el foco
				float cos_theta_s = dot(-I,theLights[i].spotDir);
				//asumimos que si el coseno es mayor que el del cutoff es porque es mayor que cero
				if(cos_theta_s>theLights[i].cosCutOff ){
					float cspot= pow(cos_theta_s,theLights[i].exponent);
					foco(I,normal_vertice.xyz,v.xyz,color_difuso,color_especular,theLights[i].diffuse,theLights[i].specular, cspot,theMaterial.shininess)
				}
			}else{//para la luz
				float atenuacion = 1.0/(theLights[i].attenuation.x + theLights[i].attenuation.y * dist + theLights[i].attenuation.z * pow(dist,2));
				posicional(I,normal_vertice.xyz,v.xyz,color_difuso,color_especular,theLights[i].diffuse,theLights[i].specular, atenuacion,theMaterial.shininess);
			}
		}
	}
	color_difuso*=theMaterial.diffuse;
	color_especular*=theMaterial.specular;
	color+=color_difuso + color_especular;

	f_texCoord = v_texCoord;
	//lo ultimo de to 
	gl_Position = modelToClipMatrix * vec4(v_position, 1);
}

float lambert_factor(vec3 n, vec3 l) return dot(n,l);

vec3 reflejo(in float lambert,in vec3 n, in vec3 l) return 2*lambert*n - l;

float especular_factor(vec3 r, vec3 v , float shininess){
	float vista_reflejo = dot(r,v);
	return (vista_reflejo> 0.0)? pow(vista_reflejo,shininess): 0.0;
}

void direccional(in vec3 l,in vec3 n, in vec3 v ,
 				inout color_difuso, inout color_especular, 
				in vec3 diffuse, in vec3 specular, in float shininess){
	float lambert = lambert_factor(n,l);
	color_difuso+= lambert_factor*diffuse;
	vec3 r = reflejo(lambert,n,l);
	//calcular el vector v o recibirlo de fuera IDGAF
	
	color_especular+= specular*(especular_factor(r,v,shininess)*lambert);
}

void posicional(in vec3 l,in vec3 n, in vec3 v ,
 				inout color_difuso, inout color_especular, in vec3 diffuse, 
				in vec3 specular,in float atenuacion, in float shininess){
	float lambert = lambert_factor(n,l);
	color_difuso+= lambert_factor*diffuse*atenuacion;
	vec3 r = reflejo(lambert,n,l);
	color_especular+= specular*(especular_factor(r,v,shininess)*lambert)*atenuacion;
}

void foco(in vec3 l,in vec3 n, in vec3 v ,
 				inout color_difuso, inout color_especular, in vec3 diffuse, 
				in vec3 specular,in float cspot, in float shininess){
	float lambert = lambert_factor(n,l);
	color_difuso+= lambert_factor*diffuse*cspot;
	vec3 r = reflejo(lambert,n,l);
	color_especular+= specular*(especular_factor(r,v,shininess)*lambert)*cspot;
	
}




