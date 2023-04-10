
#include "hjRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjSceneManager.h"
#include "hjApplication.h"
#include "hjPaintShader.h"

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
		std::shared_ptr<Mesh> mesh = MESH_NEW();
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
#pragma endregion
#pragma region Structured Buffer
		lightsBuffer = new StructuredBuffer();
		lightsBuffer->Create(sizeof(LightAttribute), 128, eSRVType::None, nullptr);
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
#pragma endregion

#pragma region 00_Character
		#pragma region 00_Player
		MAIN_FOLDER("00_Character\\00_Player");
		LOAD_TEX("00_Player", "00_Player.png");
		SUB_FOLDER_ALLCLEAR();
		#pragma endregion
#pragma endregion
#pragma region 01_TitleScene
	#pragma region 00_Layers
			MAIN_FOLDER("01_Scene\\00_TitleScene");
			LOAD_TEX("TitleScene_01", "TitleScene_01.png");
			LOAD_TEX("TitleScene_02", "TitleScene_02.png");

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float height = static_cast<float>(winRect.bottom - winRect.top);

		MAIN_FOLDER("01_Scene\\00_TitleScene\\00_Layers");
		std::shared_ptr<Texture> texture = nullptr;
		texture = LOAD_TEX("Tex_Title_Layer_BackCloud", "BackCloud.png");
		Vector2 texSize = TEX_FIND("Tex_Title_Layer_BackCloud")->GetTexSize();
		float layerRatio = height / texSize.y;
		texSize *= layerRatio;
		texture->SetTexSize(texSize);

		texture = LOAD_TEX("Tex_Title_Layer_FrontCloud", "FrontCloud.png");
		texSize = texture->GetTexSize() * layerRatio;
		texture->SetTexSize(texSize);
	#pragma endregion

	#pragma region 01_Object
		MAIN_FOLDER("01_Scene\\00_TitleScene\\01_Object");
		#pragma region Cloud
			SUB_FOLDER(1, "Cloud");
			texture = LOAD_TEX("Tex_Title_Object_MidCloud", "MidCloud0.png");
			texSize = texture->GetTexSize() * layerRatio;
			texture->SetTexSize(texSize);
		#pragma endregion
		#pragma region Bird
			SUB_FOLDER(1, "Bird");
			
			texture = LOAD_TEX("Tex_Title_Object_Bird_0", "Bird0.png");
			texSize = texture->GetTexSize() * layerRatio;
			texture->SetTexSize(texSize);
			texture = LOAD_TEX("Tex_Title_Object_Bird_1", "Bird1.png");
			texSize = texture->GetTexSize() * layerRatio;
			texture->SetTexSize(texSize);
			texture = LOAD_TEX("Tex_Title_Object_Bird_2", "Bird2.png");
			texSize = texture->GetTexSize() * layerRatio;
			texture->SetTexSize(texSize);
			texture = LOAD_TEX("Tex_Title_Object_Bird_3", "Bird3.png");
			texSize = texture->GetTexSize() * layerRatio;
			texture->SetTexSize(texSize);
			texture = LOAD_TEX("Tex_Title_Object_Bird_4", "Bird4.png");
			texSize = texture->GetTexSize() * layerRatio;
			texture->SetTexSize(texSize);
			texture = LOAD_TEX("Tex_Title_Object_Bird_5", "Bird5.png");
			texSize = texture->GetTexSize() * layerRatio;
			texture->SetTexSize(texSize);
			texture = LOAD_TEX("Tex_Title_Object_Bird_6", "Bird6.png");
			texSize = texture->GetTexSize() * layerRatio;
			texture->SetTexSize(texSize);
			texture = LOAD_TEX("Tex_Title_Object_Bird_7", "Bird7.png");
			texSize = texture->GetTexSize() * layerRatio;
			texture->SetTexSize(texSize);
		#pragma endregion
	#pragma endregion
	#pragma region 02_UI
		MAIN_FOLDER("01_Scene\\00_TitleScene\\02_UI");
			#pragma region Logo
				SUB_FOLDER(1, "Logo");
				texture = LOAD_TEX("Tex_Title_UI_MainLogo", "MainLogo.png");
				texSize = texture->GetTexSize() * layerRatio;
				texture->SetTexSize(texSize);
			#pragma endregion
			#pragma region Text
				SUB_FOLDER(1, "Text");
				#pragma region 00_Play_KOR
					SUB_FOLDER(2, "00_Play_KOR");
					texture = LOAD_TEX("Tex_Title_UI_Text_Play_On", "PlayOn_Kor.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
					texture = LOAD_TEX("Tex_Title_UI_Text_Play_Off", "PlayOff_Kor.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
			#pragma endregion
				#pragma region 01_Option_KOR
					SUB_FOLDER(2, "01_Option_KOR");
					texture = LOAD_TEX("Tex_Title_UI_Text_Option_On", "OptionOn_Kor.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
					texture = LOAD_TEX("Tex_Title_UI_Text_Option_Off", "OptionOff_Kor.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
			#pragma endregion
				#pragma region 02_Exit_KOR
					SUB_FOLDER(2, "02_Exit_KOR");
					texture = LOAD_TEX("Tex_Title_UI_Text_Exit_On", "ExitOn_Kor.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
					texture = LOAD_TEX("Tex_Title_UI_Text_Exit_Off", "ExitOff_Kor.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
				#pragma endregion
				SUB_FOLDER_CLEAR(2);
				SUB_FOLDER_CLEAR(1);
			#pragma endregion
			#pragma region Slot
				SUB_FOLDER(1, "Slot");
				#pragma region Base
					SUB_FOLDER(2, "Base");

					texture = LOAD_TEX("Tex_Title_UI_Slot_Base_NoneSelected", "SlotBase.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
					texture = LOAD_TEX("Tex_Title_UI_Slot_Base_Selected", "SlotBase_Selected.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
				#pragma endregion
				#pragma region Delete
					SUB_FOLDER(2, "DeleteButton");

					texture = LOAD_TEX("Tex_Title_UI_Slot_DeleteButton", "SlotDeleteButton.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
					texture = LOAD_TEX("Tex_Title_UI_Slot_DeleteButton_White", "SlotDeleteButtonWhite.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
				#pragma endregion
				#pragma region Text
					SUB_FOLDER(2, "Text");

					texture = LOAD_TEX("Tex_Title_UI_Slot_Text_SlotText1", "SlotText1_EN.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
					texture = LOAD_TEX("Tex_Title_UI_Slot_Text_SlotText2", "SlotText2_EN.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
					texture = LOAD_TEX("Tex_Title_UI_Slot_Text_SlotText3", "SlotText3_EN.png");
					texSize = texture->GetTexSize() * layerRatio;
					texture->SetTexSize(texSize);
				#pragma endregion
				SUB_FOLDER_ALLCLEAR();
			#pragma endregion
	#pragma endregion
#pragma endregion

		FOLDER_ALLCLEAR();

		LOAD_TEX("HPBarTexture", "PlayerLifeBase 1.png");
		LOAD_TEX("Weapon_Legendary_DemonSword_00", "02_Weapon\\04_Legendary\\DemonSword\\DemonSword00.png");
		//LOAD_TEX("Tex_Title_Layer_Sky_Day", "Sky_Day.png");
		//LOAD_TEX("Tex_Title_Layer_Sky_Night", "Sky_Night.png");

		//Compute Test
		LOAD_TEX("NoiseTex01", "Noise_01.png");
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
		material->SetTexture(texture);
		MTRL_INSERT("MTRL_Rect", material);

		// Color Rect
		material = MTRL_NEW();
		shader = SHADER_FIND("Shader_ColorRect");
		material->SetShader(shader);
		material->SetRenderingMode(eRenderingMode::Opaque);
		MTRL_INSERT("MTRL_ColorRect", material);

		// Sprite
		//std::shared_ptr<Texture> spriteTexture = Resources::Load<Texture>(L"DefaultSprite", L"DefaultSprite.png");
		texture = TEX_FIND("DefaultSprite");
		shader = SHADER_FIND("Shader_Sprite");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Sprite", material);

		// Sprite/Char/Adventurer
		texture = TEX_FIND("00_Player");
		shader = SHADER_FIND("Shader_Sprite");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Char_Adventurer", material);

		// Sprite/Weapon/Legendary/DemonSword
		texture = TEX_FIND("Weapon_Legendary_DemonSword_00");
		shader = SHADER_FIND("Shader_Sprite");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Weapon_Legendary_DemonSword", material);

		// HPBar
		texture = TEX_FIND("HPBarTexture");
		shader = SHADER_FIND("Shader_UI");
		material = MTRL_NEW();
		material->SetShader(shader);
		material->SetTexture(texture);
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
		
		texture = TEX_FIND("Tex_Title_Layer_BackCloud");
		material = MTRL_NEW();
		material->SetTexture(texture);
		material->SetShader(shader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Title_Layer_BackCloud", material);
		
		texture = TEX_FIND("Tex_Title_Layer_FrontCloud");
		material = MTRL_NEW();
		material->SetTexture(texture);
		material->SetShader(shader);
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Title_Layer_FrontCloud", material);

		texture = TEX_FIND("TitleScene_01");
		material = MTRL_NEW();
		material->SetTexture(texture);
		material->SetShader(SHADER_FIND("Shader_Sprite"));
		material->SetRenderingMode(eRenderingMode::Transparent);
		MTRL_INSERT("MTRL_Title_Object_Bird", material);

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

	void Initialize()
	{
		LoadMesh();
		LoadShader();
		SetUpState();
		LoadBuffer();
		LoadTexture();
		LoadMaterial();
	}

	void Render()
	{
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
		lightsBuffer->Bind(eShaderStage::VS, 13);
		lightsBuffer->Bind(eShaderStage::PS, 13);

		// Constant Buffer Binding
		LightCB trCb = {};
		trCb.numberOfLight = static_cast<UINT>(lights.size());

		ConstantBuffer* cb = constantBuffers[(UINT)eCBType::Light];
		cb->SetData(&trCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}
}