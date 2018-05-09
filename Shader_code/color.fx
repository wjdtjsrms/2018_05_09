//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 LightDirection;
    float SpecularPower;
};

cbuffer cbPerFrame
{
    DirectionalLight gDirLight;
};

cbuffer cbPerObject
{
	float4x4 gWorldViewProj; 
    float4x4 gWorldMatrix;
    float4x4 gViewMatrix;
    float4x4 gProjectionMatrix;
    float4x4 gTexTrabsform;
    float4 gCameraPostion;

 
};


struct VertexIn
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;

};

Texture2D ShaderTexture;

SamplerState Sampler
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};


VertexOut VS(VertexIn vin)
{
    VertexOut output;
    float4 worldPosition;

    vin.position.w = 1.0f;
  
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(vin.position, gWorldMatrix);
    output.position = mul(output.position, gViewMatrix);
    output.position = mul(output.position, gProjectionMatrix);
    
	// Store the input color for the pixel shader to use.
    output.tex = vin.tex;



	// Calculate the normal vector against the world matrix only.
    output.normal = mul(vin.normal, (float3x3) gWorldMatrix);

	// Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate the position of the vertex in the world.
    worldPosition = mul(vin.position, gWorldMatrix);
	 
	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world. 
    output.viewDirection = gCameraPostion.xyz - worldPosition.xyz;
	  
	// Normalize the viewing direction vector. 
    output.viewDirection = normalize(output.viewDirection);

    return output;
}

float4 PS(VertexOut pin) : SV_Target
{
    float3 lightDir; 
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular; 


    float4 texColor;

    lightDir = -gDirLight.LightDirection;

    texColor = ShaderTexture.Sample(Sampler, pin.tex);

    lightIntensity = saturate(dot(pin.normal, lightDir));
    color = gDirLight.Ambient;

    specular=float4(0.0f,0.0f,0.0f,0.0f);

    if (lightIntensity > 0.0f)
    {
        color += (gDirLight.Diffuse * lightIntensity);

        color = saturate(color);

        reflection = normalize(2 * lightIntensity * pin.normal - lightDir);
        specular = pow(saturate(dot(reflection, pin.viewDirection)), gDirLight.SpecularPower);

    }
    color = color * texColor;

    color = saturate(color + specular); // clamp 0.0~1.0

    return color;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
