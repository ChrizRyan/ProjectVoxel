#version 450 core



in vec2 position;  
out vec4 FragColor;

layout(std430, binding = 0) buffer VoxelBuffer {

    int voxel[];
};

uniform mat4 u_projectionMatrix; // The projection matrix
uniform mat4 u_viewMatrix;       // The view matrix
uniform vec2 u_resolution;
uniform vec3 u_dimensions;


struct Voxel {
    vec4 color;
    vec3 normal;
    vec3 hitLocation;
    vec3 coord;
};

vec3 cubeNormals[6] =
{
    vec3(1, 0, 0),
    vec3(0, 1, 0),
    vec3(0, 0, 1),
    vec3(-1, 0, 0),
    vec3(0, -1, 0),
    vec3(0, 0, -1)
};

vec2 rayBoxIntersection(vec3 rayOrig, vec3 rayDir, vec3 boxMin, vec3 boxMax)
{
    vec3 tMin = (boxMin - rayOrig) / rayDir;
    vec3 tMax = (boxMax - rayOrig) / rayDir;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);

    return vec2(tNear, tFar);
}

vec3 returnCoords(vec3 position)
{
    return floor(position);
}

vec4 decodeColor(int c)
{
    float red = float(c >> 16 & 255) / 255.0F;
    float blue = float(c >> 8 & 255) / 255.0F;
    float green = float(c & 255) / 255.0F;
    float alpha = float(c >> 24 & 255) / 255.0F;

    return vec4(red, blue, green, alpha);
}

Voxel getVoxel(vec3 coords, vec3 hitPosition, vec3 ray)
{
    Voxel currentVoxel;
    currentVoxel.color = vec4(0,0,0,0);
    currentVoxel.hitLocation = hitPosition;
    currentVoxel.coord = coords;

    float index = coords.z + (coords.y * (u_dimensions.z)) + (coords.x * (u_dimensions.y * u_dimensions.z));
    int pixel = voxel[int(index)];

    if (pixel == 0)
        return currentVoxel;

    vec3 outOfScope = max(vec3(-1.0f, -1.0f, -1.0f), (u_dimensions - max(coords + 1, u_dimensions)) + min(coords, 0)) + 1;


    vec3 cellLocation = (coords + 0.5f);
    vec3 vecToHit = hitPosition - cellLocation;
    vec3 absVecToHit = abs(vecToHit);
    float maxValue = max(abs(vecToHit.x), max(abs(vecToHit.y), abs(vecToHit.z)));

    float normalIndex = float(absVecToHit.x >= absVecToHit.y && absVecToHit.x >= absVecToHit.z) + float(absVecToHit.y > absVecToHit.x && absVecToHit.y >= absVecToHit.z) * 2 + float(absVecToHit.z > absVecToHit.x && absVecToHit.z > absVecToHit.y) * 3;
    normalIndex = (normalIndex - 1) + float(normalIndex == 1 && (vecToHit.x < 0)) * 3 + float(normalIndex == 2 && (vecToHit.y < 0)) * 3 + float(normalIndex == 3 && (vecToHit.z < 0)) * 3;

    vec3 normal = cubeNormals[int(normalIndex)];

    currentVoxel.color = decodeColor(pixel) * outOfScope.x * outOfScope.y * outOfScope.z;
    currentVoxel.normal = normal;

    return currentVoxel;
}

Voxel traceByIndex(vec3 rayDir, vec3 rayOrgin, vec3 startingCoord, vec2 raybounds)
{
    Voxel currentVoxel;
    currentVoxel.color = vec4(0,0,0,0);
    currentVoxel.normal = vec3(0,0,0);
    currentVoxel.hitLocation = rayOrgin;

    float dstTravelled = raybounds.x;
    vec3 pointAlongRay = rayOrgin + rayDir * (raybounds.x);
    vec3 currentCoords = startingCoord;

    vec3 directions = sign(rayDir);
    vec3 index = floor(vec3((directions.x + 1) * 0.5f, (directions.y + 1) * 0.5f, (directions.z + 1) * 0.5f));

    float maxDst = (raybounds.y - raybounds.x);

    float maxSteps = 3 * ceil(maxDst);
    for (float dst = 1; dst < maxSteps; ++dst)
    {
        vec3 nextCoord = currentCoords + index;

        float minDst = maxDst;
        vec3 dist = abs((nextCoord - pointAlongRay) / rayDir);

        vec3 nextIndex = vec3(0,0,0);
        if (dist.x > 0 && dist.x < dist.y && dist.x < dist.z)
        {
            nextIndex = vec3(1,0,0);
        }
        else if (dist.y > 0 && dist.y < dist.x && dist.y < dist.z)
        {
            nextIndex = vec3(0, 1, 0);
        }
        else if (dist.z > 0 && dist.z < dist.x && dist.z < dist.y)
        {
            nextIndex = vec3(0, 0, 1);
        }

        dstTravelled += nextIndex.x * dist.x + nextIndex.y * dist.y + nextIndex.z * dist.z;
        dstTravelled += 0.00001f;

        if (dstTravelled >= raybounds.y)
        {
            return currentVoxel;
        }

        pointAlongRay = rayOrgin + rayDir * (dstTravelled);
        currentCoords = returnCoords(pointAlongRay);

        Voxel voxel = getVoxel(currentCoords, pointAlongRay, rayDir);

        if (voxel.color.a > 0.01)
        {
            return voxel;
        }
    }

    return currentVoxel;
}

vec3 lightingEquation(vec3 lightPosition,
                    vec3 lightColor,     
                    vec3 diffuseColor, 
                    vec3 specularColor,
                    float specularExponent,
                    vec3 eyeCoords, // Eye coordinates for the point.
                    vec3 N, // Normal vector to the surface.
                    vec3 V  // Direction to viewer.
                    ) 
{

    vec3 L, R; // Light direction and reflected light direction.
    //if ( lightPosition.w == 0.0 ) { // directional light
        L = normalize( lightPosition.xyz );
    //}
    // else { // point light
    //     //L = normalize( lightPosition.xyz/lightPosition.w - eyeCoords );
    //     L = normalize(lightPosition.xyz - eyeCoords);
    // }

    float atten = 1.0;  
    vec3 reflection = atten * lightColor * diffuseColor * max(0.0,dot(N, L));
    return reflection;

    R = -reflect(L,N);
    if (dot(R,V) > 0.0) { // ray is reflected toward the the viewer
        float factor = pow(dot(R,V),specularExponent);
        reflection += factor * specularColor * lightColor;
    }
    

    return reflection;
}

vec3 getCameraPosition(mat4 viewMatrix) 
{
    mat4 inverseViewMatrix = inverse(viewMatrix);
    return vec3(inverseViewMatrix[3]); // Assuming column-major, last column is translation
}

void main()
{
    FragColor = vec4(0.05,0.05,0.15,1);//default color

    vec2 ndc = gl_FragCoord.xy/u_resolution*2.0 - 1.0;
    vec4 clipSpacePoint = vec4(ndc.x, ndc.y, 1.0, 1.0);
    vec4 viewSpacePoint = inverse(u_projectionMatrix) * clipSpacePoint;
    viewSpacePoint.xyz /= viewSpacePoint.w;

    vec3 cameraPos = getCameraPosition(u_viewMatrix);
    vec4 worldSpacePoint = inverse(u_viewMatrix) * viewSpacePoint;
    vec3 rayDir = normalize(worldSpacePoint.xyz - cameraPos)/2;

 

    vec2 raybounds = rayBoxIntersection(cameraPos, rayDir, vec3(0,0,0), u_dimensions); 

    Voxel currentVoxel;
    currentVoxel.color = vec4(0,0,0,0);
    currentVoxel.normal = vec3(0,0,0);

    if (raybounds.x < raybounds.y)
    {
        
        vec3 hitLocation = cameraPos + rayDir * (raybounds.x);
        vec3 dstToGrid = hitLocation - u_dimensions;

        vec3 currentCoords = floor(hitLocation);

        currentVoxel = getVoxel(currentCoords, hitLocation, rayDir);

        if (currentVoxel.color.a == 0)
        {
            currentVoxel = traceByIndex(rayDir, cameraPos, currentCoords, raybounds);
        }


        //if (currentVoxel.color.a != 0)
        {
            vec3 finalColor = vec3(0.1, 0.1, 0.15); // Ambient light
            //for (int i = 0; i < 4; i++)
            //{
                //if (lightDataColor[i].a != 0)
                //{
                    float lenghtOfRay = 100;;
                    vec4 lightPosition = vec4(5,100,5,0);//lightDataPos[i];
                    vec3 L, R; // Light direction and reflected light direction.
                    //if (lightPosition.w == 0.0) { // directional light
                        L = normalize(lightPosition.xyz);

                    // }
                    // else { // point light
                    //     vec3 vecToLight = lightPosition.xyz / lightPosition.w - currentVoxel.hitLocation;
                    //     lenghtOfRay = length(vecToLight); //(vecToLight.x * vecToLight.x) + (vecToLight.y * vecToLight.y) + (vecToLight.z * vecToLight.z); //length(vecToLight);
                    //     L = normalize(vecToLight);
                    // }

                    if (dot(currentVoxel.normal, L) > 0)
                    {
                        Voxel shadowVoxel = traceByIndex(L, currentVoxel.hitLocation, currentVoxel.coord, vec2(0, lenghtOfRay));

                        if (shadowVoxel.color.a == 0)
                        {
                            finalColor += lightingEquation(lightPosition.xyz, vec3(0.7,0.7,0.7), currentVoxel.color.xyz, vec3(1, 1, 1), 4, currentVoxel.hitLocation, currentVoxel.normal, -rayDir);
                        }
                    }
                //}
            //}

            FragColor = vec4(finalColor, currentVoxel.color.a);
        }
    }
}  