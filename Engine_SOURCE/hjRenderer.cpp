#include "pch.h"

#include "hjRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjSceneManager.h"
#include "hjApplication.h"
#include "hjPaintShader.h"
#include "hjMap.h"
#include "hjParticleShader.h"

extern hj::Application application;
namespace hj::renderer
{
	Vertex vertexes[4] = {};
	ConstantBuffer* constantBuffers[(UINT)eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthstencilStates[(UINT)eDSType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBSType::End] = {};
	
	Camera* mainCamera = nullptr;
	std::vector<Camera*> cameras[(UINT)eSceneType::End];
	std::vector<DebugMesh> debugMeshes;
	std::vector<LightAttribute> lights;
	StructuredBuffer* lightsBuffer = nullptr;

	void LoadMesh()
	{
#pragma region Point Mesh
		Vertex v = {};
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		MESH_INSERT("Mesh_Point", mesh);
		mesh->CreateVertexBuffer(&v, 1);
		UINT pointIndex = 0;
		mesh->CreateIndexBuffer(&pointIndex, 1);
#pragma endregion
#pragma region RECT Mesh
		vertexes[0].pos = Vector4(-0.5f, 0.5f, 0.f, 1.f);
		vertexes[0].color = Vector4(0.f, 1.f, 0.f, 1.f);
		vertexes[0].uv = Vector2(0.f, 0.f);

		vertexes[1].pos = Vector4(0.5f, 0.5f, 0.f, 1.f);
		vertexes[1].color = Vector4(1.f, 1.0f, 1.f, 1.f);
		vertexes[1].uv = Vector2(1.f, 0.f);

		vertexes[2].pos = Vector4(0.5f, -0.5f, 0.f, 1.f);
		vertexes[2].color = Vector4(1.f, 0.f, 0.f, 1.f);
		vertexes[2].uv = Vector2(1.f, 1.f);

		vertexes[3].pos = Vector4(-0.5f, -0.5f, 0.f, 1.f);
		vertexes[3].color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexes[3].uv = Vector2(0.f, 1.f);

		// Create Mesh
		mesh = MESH_NEW();
		MESH_INSERT("Mesh_Rect", mesh);
		mesh->CreateVertexBuffer(vertexes, 4);

		std::vector<UINT> indexes;
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);
		indexes.push_back(0);
		mesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));
#pragma endregion
#pragma region Debug RECT Mesh
		for (size_t index = 0; index < 4; ++index)
		{
			vertexes[index].pos.z = -0.00001f;
		}
		mesh = MESH_NEW();
		MESH_INSERT("Mesh_DebugRect", mesh);
		mesh->CreateVertexBuffer(vertexes, 4);

		indexes.clear();
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		indexes.push_back(3);
		indexes.push_back(0);
		mesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));
#pragma endregion
#pragma region Circle Mesh
		std::vector<Vertex> circleVtxes;
		Vertex center = {};
		center.pos = Vector4(0.f, 0.f, -0.00001f, 1.f);
		center.color = Vector4(0.f, 1.f, 0.f, 1.f);
		center.uv = Vector2::Zero;

		circleVtxes.push_back(center);

		int iSlice = 80;
		float fRadius = 0.5f;
		float fTheta = XM_2PI / static_cast<float>(iSlice);

		Vertex vtx = {};
		for (size_t i = 0; i < iSlice; ++i)
		{
			vtx.pos = Vector4
			(
				fRadius * cosf(fTheta * static_cast<float>(i))
				, fRadius * sinf(fTheta * static_cast<float>(i))
				, -0.00001f, 1.f
			);
			vtx.color = center.color;

			circleVtxes.push_back(vtx);

			vtx = {};
		}

		indexes.clear();
		for (size_t i = 0; i < iSlice - 2; ++i)
		{
			indexes.push_back(static_cast<UINT>(i) + 1);
		}
		indexes.push_back(1);
		
		// Create Mesh
		mesh = MESH_NEW();
		MESH_INSERT("Mesh_Circle", mesh);
		mesh->CreateVertexBuffer(circleVtxes.data(), static_cast<UINT>(circleVtxes.size()));
		mesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));
#pragma endregion
	}

	void SetUpState()
	{
#pragma region Input Layout
		D3D11_INPUT_ELEMENT_DESC* arrLayoutDesc = new D3D11_INPUT_ELEMENT_DESC[3];
		memset(arrLayoutDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC) * 3);
		arrLayoutDesc[0].AlignedByteOffset = 0;
		arrLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[0].InputSlot = 0;
		arrLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[0].SemanticName = "POSITION";
		arrLayoutDesc[0].SemanticIndex = 0;

		arrLayoutDesc[1].AlignedByteOffset = 16;
		arrLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[1].InputSlot = 0;
		arrLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[1].SemanticName = "COLOR";
		arrLayoutDesc[1].SemanticIndex = 0;

		arrLayoutDesc[2].AlignedByteOffset = 32;
		arrLayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayoutDesc[2].InputSlot = 0;
		arrLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[2].SemanticName = "TEXCOORD";
		arrLayoutDesc[2].SemanticIndex = 0;

		// RECT Shader
		std::shared_ptr<Shader> shader = SHADER_FIND("Shader_Rect");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Color RECT Shader
		shader = SHADER_FIND("Shader_ColorRect");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Sprite Shader
		shader = SHADER_FIND("Shader_Sprite");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Shift Shader
		shader = SHADER_FIND("Shader_Shift");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// UI Shader
		shader = SHADER_FIND("Shader_UI");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Debug Shader
		shader = SHADER_FIND("Shader_Debug");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// TileMap Shader
		shader = SHADER_FIND("Shader_TileMap");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Particle Shader
		shader = SHADER_FIND("Shader_Particle");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		delete[] arrLayoutDesc;

		arrLayoutDesc = new D3D11_INPUT_ELEMENT_DESC[2];
		memset(arrLayoutDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC) * 2);

		arrLayoutDesc[0].AlignedByteOffset = 0;
		arrLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[0].InputSlot = 0;
		arrLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[0].SemanticName = "POSITION";
		arrLayoutDesc[0].SemanticIndex = 0;
				
		arrLayoutDesc[1].AlignedByteOffset = 16;
		arrLayoutDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		arrLayoutDesc[1].InputSlot = 0;
		arrLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[1].SemanticName = "TEXCOORD";
		arrLayoutDesc[1].SemanticIndex = 0;

		// Grid Shader
		shader = SHADER_FIND("Shader_Grid");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 2
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Fade Shader
		shader = SHADER_FIND("Shader_Fade");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 2
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());				

		delete[] arrLayoutDesc;
#pragma endregion
#pragma region Sampler State
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		//D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5;
		//D3D11_FILTER_MIN_LINEAR_MAG_MIP_LINEAR = 0x10;

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;	
		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, samplerStates[(UINT)eSamplerType::Point].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		GetDevice()->CreateSamplerState
		(
			&samplerDesc
			, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf()
		);

		GetDevice()->BindsSamplers((UINT)eSamplerType::Point
			, 1, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Linear
			, 1, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf());

		GetDevice()->BindsSamplers((UINT)eSamplerType::Anisotropic
			, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
#pragma endregion
#pragma region Rasterizer State
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

		GetDevice()->CreateRasterizerState(&rsDesc
			, rasterizerStates[(UINT)eRSType::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

		GetDevice()->CreateRasterizerState(&rsDesc
			, rasterizerStates[(UINT)eRSType::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc
			, rasterizerStates[(UINT)eRSType::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc
			, rasterizerStates[(UINT)eRSType::WireframeNone].GetAddressOf());
#pragma endregion
#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, depthstencilStates[(UINT)eDSType::LessEqual].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, depthstencilStates[(UINT)eDSType::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, depthstencilStates[(UINT)eDSType::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc
			, depthstencilStates[(UINT)eDSType::None].GetAddressOf());
#pragma endregion
#pragma region Blend State
		blendStates[(UINT)eBSType::Default] = nullptr;

		D3D11_BLEND_DESC bsDesc = {};
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&bsDesc, blendStates[(UINT)eBSType::AlphaBlend].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&bsDesc, blendStates[(UINT)eBSType::OneOne].GetAddressOf());
#pragma endregion
	}

	void LoadBuffer()
	{
#pragma region Constant Buffer
		constantBuffers[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));

		constantBuffers[(UINT)eCBType::Grid] = new ConstantBuffer(eCBType::Grid);
		constantBuffers[(UINT)eCBType::Grid]->Create(sizeof(GridCB));

		constantBuffers[(UINT)eCBType::Animation] = new ConstantBuffer(eCBType::Animation);
		constantBuffers[(UINT)eCBType::Animation]->Create(sizeof(AnimationCB));

		constantBuffers[(UINT)eCBType::Light] = new ConstantBuffer(eCBType::Light);
		constantBuffers[(UINT)eCBType::Light]->Create(sizeof(LightCB));

		constantBuffers[(UINT)eCBType::Global] = new ConstantBuffer(eCBType::Global);
		constantBuffers[(UINT)eCBType::Global]->Create(sizeof(GlobalCB));

		constantBuffers[(UINT)eCBType::ParticleSystem] = new ConstantBuffer(eCBType::ParticleSystem);
		constantBuffers[(UINT)eCBType::ParticleSystem]->Create(sizeof(ParticleSystemCB));

		constantBuffers[(UINT)eCBType::Noise] = new ConstantBuffer(eCBType::Noise);
		constantBuffers[(UINT)eCBType::Noise]->Create(sizeof(NoiseCB));

#pragma endregion
#pragma region Structured Buffer
		lightsBuffer = new StructuredBuffer();
		lightsBuffer->Create(sizeof(LightAttribute), 128, eSRVType::SRV, nullptr, true);
#pragma endregion
	}

	void LoadShader()
	{
		// Default
		std::shared_ptr<Shader> shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");

		SHADER_INSERT("Shader_Rect", shader);

		// Color RECT Shader
		shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"ColorPS.hlsl", "main");

		SHADER_INSERT("Shader_ColorRect", shader);

		// Sprite
		shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"AnimationPS.hlsl", "main");
		shader->SetRSState(eRSType::SolidNone);
		SHADER_INSERT("Shader_Sprite", shader);

		// Shift
		shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"ShiftPS.hlsl", "main");

		SHADER_INSERT("Shader_Shift", shader);

		// UI
		shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"UserInterfaceVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"UserInterfacePS.hlsl", "main");

		SHADER_INSERT("Shader_UI", shader);

		// Grid
		shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");

		SHADER_INSERT("Shader_Grid", shader);

		// Fade
		shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"FadeVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"FadePS.hlsl", "main");

		SHADER_INSERT("Shader_Fade", shader);

		// Debug
		shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");
		shader->SetRSState(eRSType::SolidNone);
		shader->SetDSState(eDSType::NoWrite);
		shader->SetBSState(eBSType::AlphaBlend);
		shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		SHADER_INSERT("Shader_Debug", shader);

		// Paint Shader(Compute Shader)
		std::shared_ptr<PaintShader> paintShader = std::make_shared<PaintShader>();
		paintShader->Create(L"PaintCS.hlsl", "main");
		Resources::Insert<PaintShader>(WIDE("Shader_Paint"), paintShader);

		// TileMap Shader
		shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TileMapPS.hlsl", "main");
		shader->SetRSState(eRSType::SolidNone);
		shader->SetDSState(eDSType::LessEqual);
		shader->SetBSState(eBSType::AlphaBlend);

		SHADER_INSERT("Shader_TileMap", shader);

		// ParticleSystem Shader
		shader = SHADER_NEW();
		shader->Create(eShaderStage::VS, L"ParticleVS.hlsl", "main");
		shader->Create(eShaderStage::GS, L"ParticleGS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"ParticlePS.hlsl", "main");
		shader->SetRSState(eRSType::SolidNone);
		shader->SetDSState(eDSType::NoWrite);
		shader->SetBSState(eBSType::AlphaBlend);
		shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		SHADER_INSERT("Shader_Particle", shader);
		
		std::shared_ptr<ParticleShader> particleCS = std::make_shared<ParticleShader>();
		particleCS->Create(L"ParticleCS.hlsl", "main");
		Resources::Insert<ParticleShader>(L"Shader_ParticleCS", particleCS);
	}


	void LoadTexture()
	{
		std::wstring mainFolderPath = WIDE("");
		std::wstring subFolderPath1 = WIDE("");
		std::wstring subFolderPath2 = WIDE("");

// must type ;(semicolon) where Macro call end.
#define LOAD_TEX(texName, fileName) \
			Resources::Load<Texture>(WIDE(texName), \
			(mainFolderPath.empty() ? L"" : mainFolderPath + WIDE("\\")) \
			+(subFolderPath1.empty() ? L"" : subFolderPath1 + WIDE("\\"))\
			+(subFolderPath2.empty() ? L"" : subFolderPath2 + WIDE("\\"))\
			+ WIDE(fileName))

#define MAIN_FOLDER(folderPath) mainFolderPath = WIDE(folderPath)
#define MAIN_FOLDER_CLEAR() mainFolderPath = WIDE("")
#define SUB_FOLDER(subNumber, folderPath) subFolderPath##subNumber = WIDE(folderPath)
#define SUB_FOLDER_CLEAR(subNumber) subFolderPath##subNumber = WIDE("")
#define SUB_FOLDER_ALLCLEAR() \
			SUB_FOLDER_CLEAR(1); \
			SUB_FOLDER_CLEAR(2)
#define FOLDER_ALLCLEAR() \
			MAIN_FOLDER_CLEAR(); \
			SUB_FOLDER_ALLCLEAR()
			

#pragma region Default
		LOAD_TEX("LightSprite", "Light.png");
		LOAD_TEX("DefaultSprite", "DungeonEat08.png");
		LOAD_TEX("MapTile", "Map5x.png");
#pragma endregion

#pragma region 00_Character
		#pragma region 00_Player
		MAIN_FOLDER("00_Character\\00_Player");
		LOAD_TEX("00_Player", "00_Player.png");
		SUB_FOLDER_ALLCLEAR();
		#pragma endregion

		#pragma region 02_Monster
		MAIN_FOLDER("00_Character\\02_Monster");
			#pragma region Boss
				SUB_FOLDER(1, "01_Boss");
				SUB_FOLDER(2, "00_Skul");
				LOAD_TEX("02_Monster_Boss_Bellial", "Monster_Boss_1_Bellial.png");
				SUB_FOLDER_ALLCLEAR();
			#pragma endregion
		#pragma endregion
#pragma endregion
#pragma region 01_Scene
		MAIN_FOLDER("01_Scene");
	#pragma region 00_TitleScene
			SUB_FOLDER(1, "00_TitleScene");
			LOAD_TEX("TitleScene_01", "TitleScene_01.png");
			LOAD_TEX("TitleScene_02", "TitleScene_02.png");
			LOAD_TEX("TitleScene_03", "TitleScene_03.png");
			SUB_FOLDER_ALLCLEAR();
	#pragma endregion
	#pragma region 01_TownScene
	
	#pragma endregion
	#pragma region 02_DungeonScene
	
	#pragma endregion
	#pragma region 03_EndingScene
	
	#pragma endregion
#pragma endregion
#pragma region 03_UI
		MAIN_FOLDER("03_UI");
		SUB_FOLDER(1, "00_HUD");
		LOAD_TEX("03_UI_00_HUD_RedWarningOnHit", "RedWarningOnHit.png");
		FOLDER_ALLCLEAR();
#pragma endregion
#pragma region _08_FX
		MAIN_FOLDER("08_FX");
		LOAD_TEX("08_FX", "08_FX.png");
		FOLDER_ALLCLEAR();
#pragma endregion

		FOLDER_ALLCLEAR();

		LOAD_TEX("HPBarTexture", "PlayerLifeBase 1.png");
		LOAD_TEX("Weapon_Legendary_DemonSword_00", "02_Weapon\\04_Legendary\\DemonSword\\DemonSword00.png");
		LOAD_TEX("CartoonSmoke", "particle\\CartoonSmoke.png");
		//LOAD_TEX("Tex_Title_Layer_Sky_Day", "Sky_Day.png");
		//LOAD_TEX("Tex_Title_Layer_Sky_Night", "Sky_Night.png");

		//Compute Test
		LOAD_TEX("NoiseTex01", "Noise_01.png");
		LOAD_TEX("noise_01", "noise\\noise_01.png");
		LOAD_TEX("noise_02", "noise\\noise_02.png");
		//LOAD_TEX("noise_03", "noise\\noise_03.png");

		std::shared_ptr<Texture> uavTexture = std::make_shared<Texture>();
		uavTexture->Create(1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE
			| D3D11_BIND_UNORDERED_ACCESS);
		Resources::Insert<Texture>(L"PaintTexture", uavTexture);
	}

	void LoadMaterial()
	{
		// Default
		std::shared_ptr<Texture> texture = TEX_FIND("LightSprite");
		std::shared_ptr<Shader> shader = SHADER_FIND("Shader_Rect");
		std::shared_ptr<Material> material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		MTRL_INSERT("MTRL_Rect", material);

		// Color Rect
		material = MTRL_NEW();
		shader = SHADER_FIND("Shader_ColorRect");
		material->SetShader(shader);
		material->SetRenderingMode(eRenderingMode::Opaque);
		MTRL_INSERT("MTRL_ColorRect", material);

		// Sprite
		texture = TEX_FIND("DefaultSprite");
		shader = SHADER_FIND("Shader_Sprite");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Sprite", material);

		// Sprite/Char/Adventurer
		texture = TEX_FIND("00_Player");
		shader = SHADER_FIND("Shader_Sprite");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Char_Adventurer", material);

		texture = TEX_FIND("02_Monster_Boss_Bellial");
		shader = SHADER_FIND("Shader_Sprite");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Monster_Boss_Bellial", material);

		// Sprite/08_FX/FX
		texture = TEX_FIND("08_FX");
		shader = SHADER_FIND("Shader_Sprite");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_FX", material);

		// Sprite/Weapon/Legendary/DemonSword
		texture = TEX_FIND("Weapon_Legendary_DemonSword_00");
		shader = SHADER_FIND("Shader_Sprite");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Weapon_Legendary_DemonSword", material);

		//HP Red Warning UI
		texture = TEX_FIND("03_UI_00_HUD_RedWarningOnHit");
		shader = SHADER_FIND("Shader_UI");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_UI_HUD_RedWarningOnHit", material);

		// HPBar
		texture = TEX_FIND("HPBarTexture");
		shader = SHADER_FIND("Shader_UI");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_UI", material);

		// Grid
		shader = SHADER_FIND("Shader_Grid");
		material = MTRL_NEW();
		material->SetShader(shader);
		shader->SetRSState(eRSType::SolidNone);
		shader->SetDSState(eDSType::NoWrite);
		shader->SetBSState(eBSType::AlphaBlend);
		MTRL_INSERT("MTRL_Grid", material);

		// Fade
		shader = SHADER_FIND("Shader_Fade");
		material = MTRL_NEW();
		material->SetShader(shader);
		shader->SetBSState(eBSType::AlphaBlend);
		shader->SetDSState(eDSType::None);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Fade", material);

		// Debug
		shader = SHADER_FIND("Shader_Debug");
		material = MTRL_NEW();
		material->SetRenderingMode(eRenderingMode::Transparent);
		material->SetShader(shader);
		MTRL_INSERT("MTRL_Debug", material);
		
#pragma region Mtrl_Title
		shader = SHADER_FIND("Shader_Shift");
		shader->SetBSState(eBSType::AlphaBlend);
		
		texture = TEX_FIND("TitleScene_02");
		material = MTRL_NEW();
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetShader(shader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Title_Layer_Cloud", material);

		material = MTRL_NEW();
		texture = TEX_FIND("TitleScene_01");
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetShader(SHADER_FIND("Shader_Sprite"));
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Title_Object_Bird", material);

		shader = SHADER_FIND("Shader_UI");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Title_UI", material);

		material = MTRL_NEW();
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetShader(SHADER_FIND("Shader_Sprite"));
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Title_Object", material);

		material = MTRL_NEW();
		texture = TEX_FIND("TitleScene_03");
		material->SetShader(shader);
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Title_UI_Text", material);
#pragma endregion
#pragma region
		material = MTRL_NEW();
		texture = TEX_FIND("MapTile");
		material->SetShader(SHADER_FIND("Shader_TileMap"));
		material->SetTexture(eTextureSlot::T0, texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Map_Tile", material);

		material = MTRL_NEW();
		material->SetShader(SHADER_FIND("Shader_Particle"));
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Particle", material);
#pragma endregion
		//texture = TEX_FIND("Tex_Town_Layer_Sky_Day");
		//material = MTRL_NEW();
		//material->SetTexture(texture);
		//material->SetShader(shader);
		//material->SetRenderingMode(eRenderingMode::Transparent);
		//MTRL_INSERT("MTRL_Town_Layer_Sky_Day", material);
		//
		//texture = TEX_FIND("Tex_Town_Layer_Sky_Night");
		//material = MTRL_NEW();
		//material->SetTexture(texture);
		//material->SetShader(shader);
		//material->SetRenderingMode(eRenderingMode::Transparent);
		//MTRL_INSERT("MTRL_Town_Layer_Sky_Night", material);

	}

	void LoadMapData()
	{
		std::shared_ptr<Map> map = std::make_shared<Map>();
		map->Load(WIDE("TileMap\\00_Town.xml")); 
		Resources::Insert<Map>(WIDE("MAP_00_Town"), map);

		map = std::make_shared<Map>();
		map->Load(WIDE("TileMap\\01_BossRoom.xml"));
		Resources::Insert<Map>(WIDE("MAP_01_BossRoom"), map);

		map = std::make_shared<Map>();
		map->Load(WIDE("TileMap\\01_N_LR.xml"));
		Resources::Insert<Map>(WIDE("MAP_01_LR2"), map);

#pragma region Start Room
		{
			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_B.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_B"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_L.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_L"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_LB.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_LB"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_LR.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_LR"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_LRB.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_LRB"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_LT.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_LT"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_LTB.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_LTB"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_LTR.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_LTR"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_LTRB.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_LTRB"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_R.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_R"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_RB.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_RB"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_T.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_T"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_TB.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_TB"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_TR.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_TR"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_TR2.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_TR2"), map);

			map = std::make_shared<Map>();
			map->Load(WIDE("TileMap\\01_S_TRB.xml"));
			Resources::Insert<Map>(WIDE("MAP_01_S_TRB"), map);
		}
#pragma endregion
	}

	void Initialize()
	{
		LoadMesh();
		LoadShader();
		SetUpState();
		LoadBuffer();
		LoadTexture();
		LoadMaterial();
		LoadMapData();
	}

	void Render()
	{
		BindNoiseTexture();
		BindLights();

		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		for (Camera* cam : cameras[(UINT)type])
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		cameras[(UINT)type].clear();
		lights.clear();
	}

	void Release()
	{
		for (size_t i = 0; i < (UINT)eCBType::End; ++i)
		{
			if (constantBuffers[i])
			{
				delete constantBuffers[i];
				constantBuffers[i] = nullptr;
			}
		}

		if (lightsBuffer)
		{
			delete lightsBuffer;
			lightsBuffer = nullptr;
		}
	}

	void PushLightAttribute(LightAttribute lightAttribute)
	{
		lights.push_back(lightAttribute);
	}

	void BindLights()
	{
		// Structured Buffer Binding
		lightsBuffer->SetData(lights.data(), static_cast<UINT>(lights.size()));
		lightsBuffer->BindSRV(eShaderStage::VS, 13);
		lightsBuffer->BindSRV(eShaderStage::PS, 13);

		// Constant Buffer Binding
		LightCB trCb = {};
		trCb.numberOfLight = static_cast<UINT>(lights.size());

		ConstantBuffer* cb = constantBuffers[(UINT)eCBType::Light];
		cb->SetData(&trCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	void BindNoiseTexture()
	{
		std::shared_ptr<Texture> noise = TEX_FIND("noise_01");
		noise->BindShaderResource(eShaderStage::VS, 16);
		noise->BindShaderResource(eShaderStage::HS, 16);
		noise->BindShaderResource(eShaderStage::DS, 16);
		noise->BindShaderResource(eShaderStage::GS, 16);
		noise->BindShaderResource(eShaderStage::PS, 16);
		noise->BindShaderResource(eShaderStage::CS, 16);

		NoiseCB info = {};
		info.noiseSize.x = static_cast<float>(noise->GetWidth());
		info.noiseSize.y = static_cast<float>(noise->GetHeight());

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Noise];
		cb->SetData(&info);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}
}