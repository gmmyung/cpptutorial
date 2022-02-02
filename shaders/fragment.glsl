#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientLightColor;
uniform vec3 viewPos;
uniform vec3 increment;
void main()
{
   float specularStrength = 1.0f;
   vec3 lightDirection = normalize(lightPos - FragPos);
   vec3 viewDirection = normalize(viewPos - FragPos);
   vec3 reflectDirection = reflect(-lightDirection, Normal);  
   float lightDistance = length(lightPos - FragPos);
   vec3 ambient = 0.2 * ambientLightColor;
   float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 128);
   vec3 specular = specularStrength * spec * lightColor;  

   /*
   if(increment){

   }
   */
   
   vec3 diffuse = max(dot(Normal, lightDirection), 0.0) * lightColor / pow(lightDistance, 2) * 10.0;
   FragColor = vec4(ourColor, 1) * texture(texture1, (vec2(0,1)-TexCoord)) * vec4(ambient + diffuse + specular, 1.0);
}