//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************
struct MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float4 specularColor;
    float specularPower;

};

struct CamaraBuffer
{
    float4 cameraPosition;
};




cbuffer StandardMatrix
{
    MatrixBuffer gMatrix;
};

cbuffer DirectionalLight
{
    LightBuffer gLight;
};

cbuffer StandardMatrixCamera
{
    CamaraBuffer gCamera;
	
};


struct VertexIn
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0; //save teture position
	float3 normal: NORMAL;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

Texture2D shaderTexture[2];
SamplerState SampleType;


VertexOut VS(VertexIn vin)
{
        
    VertexOut output;
    float4 worldPosition;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    vin.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(vin.position, gMatrix.worldMatrix);
    output.position = mul(output.position, gMatrix.viewMatrix);
    output.position = mul(output.position, gMatrix.projectionMatrix);
    
	// Store the input color for the pixel shader to use.
    output.tex = vin.tex;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(vin.normal, (float3x3) gMatrix.worldMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(vin.position, gMatrix.worldMatrix);
	 
	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world. 
    output.viewDirection = gCamera.cameraPosition.xyz - worldPosition.xyz;
	  
	// Normalize the viewing direction vector. 
    output.viewDirection = normalize(output.viewDirection);

    return output;
}

float4 PS(VertexOut pin) : SV_Target
{

    float4 textureColor;
    float4 textureColor2;
    float4 blendColor;
    

    float3 lightDir; //direction
    float lightIntensity; //power
    float4 color;
    float3 reflection; //color
    float4 specular; //vector

    textureColor = shaderTexture[0].Sample(SampleType, pin.tex);
    textureColor2 = shaderTexture[1].Sample(SampleType, pin.tex);

    blendColor = textureColor2 * textureColor * 1.0f;
	//blendColor= textureColor2*0.5f + textureColor *0.5f;

    blendColor = saturate(blendColor);

	// Invert the light direction for calculations.
    lightDir = -gLight.lightDirection;

	// Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(pin.normal, lightDir)); //value clamp 0~1

	 // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    color = gLight.ambientColor;

    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    if (lightIntensity > 0.0f)
    {
        color += (gLight.diffuseColor * lightIntensity);

        color = saturate(color);

        reflection = normalize(2 * lightIntensity * pin.normal - lightDir);
        specular = pow(saturate(dot(reflection, pin.viewDirection)), gLight.specularPower) * gLight.specularColor;

    }

    color = saturate(color);

	 // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
	 // blendColor is texture color
    color = color * blendColor;

    color = saturate(color + specular); // clamp 0.0~1.0

    float4 test;
    test = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return test;
    
}

technique11 LightTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}

