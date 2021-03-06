/*
 * Direct3D11 shader file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "RenderSystem/Direct3D11/spDirect3D11Shader.hpp"

#if defined(SP_COMPILE_WITH_DIRECT3D11)


#include "RenderSystem/Direct3D11/spDirect3D11RenderSystem.hpp"
#include "Platform/spSoftPixelDeviceOS.hpp"


namespace sp
{

extern video::RenderSystem* __spVideoDriver;

namespace video
{


/*
 * Internal members
 */

//extern const c8* const d3dVertexShaderVersions[];
//extern const c8* const d3dPixelShaderVersions[];

const c8* const d3dVertexShaderVersions[] =
{
    "vs_1_0",
    "vs_2_0",
    "vs_2_a",
    "vs_3_0",
    "vs_4_0",
    "vs_4_1",
    "vs_5_0",
    0
};

const c8* const d3dPixelShaderVersions[] =
{
    "ps_1_0",
    "ps_1_1",
    "ps_1_2",
    "ps_1_3",
    "ps_1_4",
    "ps_2_0",
    "ps_2_a",
    "ps_2_b",
    "ps_3_0",
    "ps_4_0",
    "ps_4_1",
    "ps_5_0",
    0
};

const c8* const d3dGeometryShaderVersions[] =
{
    "gs_4_0",
    "gs_4_1",
    "gs_5_0",
    0
};

const c8* const d3dComputeShaderVersions[] =
{
    "cs_4_0",
    "cs_4_1",
    "cs_5_0",
    0
};

const c8* const d3dHullShaderVersions[] =
{
    "hs_5_0",
    0
};

const c8* const d3dDomainShaderVersions[] =
{
    "ds_5_0",
    0
};


/*
 * Direct3D11Shader class
 */

Direct3D11Shader::Direct3D11Shader(ShaderTable* Table, const EShaderTypes Type, const EShaderVersions Version)
    : Shader(Table, Type, Version), DeviceContext_(0), VertexShaderObject_(0), PixelShaderObject_(0),
    GeometryShaderObject_(0), HullShaderObject_(0), DomainShaderObject_(0), ComputeShaderObject_(0),
    InputVertexLayout_(0), ShaderReflection_(0)
{
    Device_         = static_cast<video::Direct3D11RenderSystem*>(__spVideoDriver)->Device_;
    DeviceContext_  = static_cast<video::Direct3D11RenderSystem*>(__spVideoDriver)->DeviceContext_;
    
    if (!Table_)
        Table_ = new Direct3D11ShaderTable();
    
    updateTable();
}
Direct3D11Shader::~Direct3D11Shader()
{
    Direct3D11RenderSystem::releaseObject(VertexShaderObject_);
    Direct3D11RenderSystem::releaseObject(PixelShaderObject_);
    Direct3D11RenderSystem::releaseObject(GeometryShaderObject_);
    Direct3D11RenderSystem::releaseObject(HullShaderObject_);
    Direct3D11RenderSystem::releaseObject(DomainShaderObject_);
    Direct3D11RenderSystem::releaseObject(ComputeShaderObject_);
    
    Direct3D11RenderSystem::releaseObject(InputVertexLayout_);
    
    for (std::vector<ID3D11Buffer*>::iterator it = ConstantBuffers_.begin(); it != ConstantBuffers_.end(); ++it)
        Direct3D11RenderSystem::releaseObject(*it);
}

/* Shader compilation */

bool Direct3D11Shader::compile(const std::vector<io::stringc> &ShaderBuffer, const io::stringc &EntryPoint)
{
    bool Result = false;
    
    c8* ProgramBuffer = 0;
    createProgramString(ShaderBuffer, ProgramBuffer);
    
    if (Type_ == SHADER_VERTEX)
        Result = compileHLSL(ProgramBuffer, EntryPoint.c_str(), d3dVertexShaderVersions     [getVersionIndex(HLSL_VERTEX_1_0,   HLSL_VERTEX_5_0     )]);
    else if (Type_ == SHADER_PIXEL)
        Result = compileHLSL(ProgramBuffer, EntryPoint.c_str(), d3dPixelShaderVersions      [getVersionIndex(HLSL_PIXEL_1_0,    HLSL_PIXEL_5_0      )]);
    else if (Type_ == SHADER_GEOMETRY)
        Result = compileHLSL(ProgramBuffer, EntryPoint.c_str(), d3dGeometryShaderVersions   [getVersionIndex(HLSL_GEOMETRY_4_0, HLSL_GEOMETRY_5_0   )]);
    else if (Type_ == SHADER_HULL)
        Result = compileHLSL(ProgramBuffer, EntryPoint.c_str(), d3dHullShaderVersions       [getVersionIndex(HLSL_HULL_5_0,     HLSL_HULL_5_0       )]);
    else if (Type_ == SHADER_DOMAIN)
        Result = compileHLSL(ProgramBuffer, EntryPoint.c_str(), d3dDomainShaderVersions     [getVersionIndex(HLSL_DOMAIN_5_0,   HLSL_DOMAIN_5_0     )]);
    else if (Type_ == SHADER_COMPUTE)
        Result = compileHLSL(ProgramBuffer, EntryPoint.c_str(), d3dComputeShaderVersions    [getVersionIndex(HLSL_COMPUTE_4_0,  HLSL_COMPUTE_5_0    )]);
    
    if (Result)
        createConstantBuffers();
    
    MemoryManager::deleteBuffer(ProgramBuffer);
    
    return CompiledSuccessfully_ = Result;
}

/* Set the constant buffer */

bool Direct3D11Shader::setConstantBuffer(const io::stringc &Name, const void* Buffer)
{
    ID3D11Buffer* ConstantBuffer = ConstantBufferNames_[Name.str()];
    
    if (ConstantBuffer)
    {
        DeviceContext_->UpdateSubresource(ConstantBuffer, 0, 0, Buffer, 0, 0);
        return true;
    }
    
    return false;
}

bool Direct3D11Shader::setConstantBuffer(u32 Number, const void* Buffer)
{
    if (Number < ConstantBuffers_.size())
    {
        DeviceContext_->UpdateSubresource(ConstantBuffers_[Number], 0, 0, Buffer, 0, 0);
        return true;
    }
    
    return false;
}

u32 Direct3D11Shader::getConstantCount() const
{
    return 0; // !TODO!
}

std::vector<io::stringc> Direct3D11Shader::getConstantList() const
{
    return std::vector<io::stringc>(); // !TODO!
}


/*
 * ======= Private: =======
 */

bool Direct3D11Shader::compileHLSL(const c8* ProgramBuffer, const c8* EntryPoint, const c8* TargetName)
{
    if (!ProgramBuffer)
        return false;
    
    /* Temporary variables */
    ID3DBlob* Buffer = 0;
    ID3DBlob* Errors = 0;
    
    UINT CompileFlags = D3D10_SHADER_OPTIMIZATION_LEVEL3;
    
    /* Get the shader name */
    io::stringc ShaderName;
    
    switch (Type_)
    {
        case SHADER_VERTEX:
            ShaderName = "vertex"; break;
        case SHADER_PIXEL:
            ShaderName = "pixel"; break;
        case SHADER_GEOMETRY:
            ShaderName = "geometry"; break;
        case SHADER_HULL:
            ShaderName = "hull"; break;
        case SHADER_DOMAIN:
            ShaderName = "domain"; break;
        case SHADER_COMPUTE:
            ShaderName = "compute"; break;
    }
    
    /* Compile the shader */
    HRESULT Result = D3DCompile(
        ProgramBuffer,          /* Shader source */
        strlen(ProgramBuffer),  /* Source length */
        ProgramBuffer,          /* Source name */
        0,                      /* Shader macros */
        0,                      /* Include file handeing */
        EntryPoint,             /* Entry point (shader's main function) */
        TargetName,             /* Target name (shader's version) */
        CompileFlags,           /* Compile flags (flags1) */
        0,                      /* Effect flags (flags2) */
        &Buffer,                /* Compiled output shader code */
        &Errors                 /* Error messages */
    );
    
    /* Check for errors */
    if (Result)
    {
        io::Log::message("Direct3D11 HLSL " + ShaderName + " shader compilation failed:", io::LOG_WARNING);
        
        if (Errors)
        {
            io::Log::message((c8*)Errors->GetBufferPointer(), io::LOG_WARNING);
            Errors->Release();
        }
        
        if (Buffer)
            Buffer->Release();
        
        return false;
    }
    
    if (!Buffer)
        return false;
    
    /* Create the vertex shader */
    switch (Type_)
    {
        case SHADER_VERTEX:
            Result = Device_->CreateVertexShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), 0, &VertexShaderObject_); break;
        case SHADER_PIXEL:
            Result = Device_->CreatePixelShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), 0, &PixelShaderObject_); break;
        case SHADER_GEOMETRY:
            Result = Device_->CreateGeometryShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), 0, &GeometryShaderObject_); break;
        case SHADER_HULL:
            Result = Device_->CreateHullShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), 0, &HullShaderObject_); break;
        case SHADER_DOMAIN:
            Result = Device_->CreateDomainShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), 0, &DomainShaderObject_); break;
        case SHADER_COMPUTE:
            Result = Device_->CreateComputeShader(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), 0, &ComputeShaderObject_); break;
    }
    
    if (Result)
    {
        io::Log::error("Could not create HLSL " + ShaderName + " shader");
        Buffer->Release();
        return false;
    }
    
    if (Type_ == SHADER_VERTEX)
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC>* InputDesc = static_cast<std::vector<D3D11_INPUT_ELEMENT_DESC>*>(
            static_cast<Direct3D11ShaderTable*>(Table_)->VertexFormat_->InputLayout_
        );
        
        /* Create the vertex layout */
        Result = Device_->CreateInputLayout(
            //static_cast<video::Direct3D11RenderSystem*>(__spVideoDriver)->VertexLayout3D_, 13,
            &(*InputDesc)[0], InputDesc->size(),
            Buffer->GetBufferPointer(), Buffer->GetBufferSize(), &InputVertexLayout_
        );
        
        if (Result)
        {
            io::Log::error("Could not create vertex input layout");
            Buffer->Release();
            return false;
        }
    }
    
    /* Get the shader reflection */
    if (D3DReflect(Buffer->GetBufferPointer(), Buffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&ShaderReflection_))
    {
        io::Log::error("Could not get shader reflection");
        Buffer->Release();
        return false;
    }
    
    Buffer->Release();
    
    return true;
}

void Direct3D11Shader::createProgramString(const std::vector<io::stringc> &ShaderBuffer, c8* &ProgramBuffer)
{
    /* Create a single line string */
    io::stringc ProgramString;
    
    for (std::vector<io::stringc>::const_iterator it = ShaderBuffer.begin(); it != ShaderBuffer.end(); ++it)
        ProgramString += *it + "\n";
    
    /* Create the program buffer and copy the data */
    ProgramBuffer = new c8[ProgramString.size() + 1];
    
    memcpy(ProgramBuffer, ProgramString.c_str(), ProgramString.size());
    
    ProgramBuffer[ProgramString.size()] = 0;
}

bool Direct3D11Shader::createConstantBuffers()
{
    if (!ShaderReflection_)
        return false;
    
    /* Temporary variables */
    ID3D11ShaderReflectionConstantBuffer* ReflectionBuffer = 0;
    
    D3D11_SHADER_DESC ShaderDesc;
    D3D11_SHADER_BUFFER_DESC ShaderBufferDesc;
    D3D11_BUFFER_DESC BufferDesc;
    
    ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
    
    /* Release and clear old constant buffers */
    for (std::vector<ID3D11Buffer*>::iterator it = ConstantBuffers_.begin(); it != ConstantBuffers_.end(); ++it)
        Direct3D11RenderSystem::releaseObject(*it);
    ConstantBuffers_.clear();
    
    /* Examine each shader constant buffer */
    ShaderReflection_->GetDesc(&ShaderDesc);
    
    /* Create each constant buffer */
    for (u32 i = 0, j = 0; i < ShaderDesc.ConstantBuffers; ++i)
    {
        /* Get shader buffer description */
        ReflectionBuffer = ShaderReflection_->GetConstantBufferByIndex(i);
        
        if (!ReflectionBuffer || ReflectionBuffer->GetDesc(&ShaderBufferDesc))
        {
            io::Log::error("Could not get constant buffer #" + io::stringc(i));
            return false;
        }
        
        if (ShaderBufferDesc.Type != D3D11_CT_CBUFFER)
            continue;
        
        ConstantBuffers_.resize(j + 1);
        
        /* Create the constant buffer */
        BufferDesc.Usage            = D3D11_USAGE_DEFAULT;
        BufferDesc.ByteWidth        = ShaderBufferDesc.Size;
        BufferDesc.BindFlags        = D3D11_BIND_CONSTANT_BUFFER;
        BufferDesc.CPUAccessFlags   = 0;
        
        if (Device_->CreateBuffer(&BufferDesc, 0, &ConstantBuffers_[j]))
        {
            io::Log::error("Could not create shader constant buffer #" + io::stringc(i) +
            " (\"" + io::stringc(ShaderBufferDesc.Name) + "\")");
            return false;
        }
        
        /* Store the constant buffer in the shader buffer name map */
        ConstantBufferNames_[std::string(ShaderBufferDesc.Name)] = ConstantBuffers_[j];
        
        ++j;
    }
    
    return true;
}


} // /namespace scene

} // /namespace sp


#endif



// ================================================================================
