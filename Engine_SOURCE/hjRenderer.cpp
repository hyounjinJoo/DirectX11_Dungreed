
#include "hjRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjSceneManager.h"

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
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"RectMesh", mesh);
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
		mesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"DebugRectMesh", mesh);
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
		mesh = std::make_shared<Mesh>();
		Resources::Insert<Mesh>(L"CircleMesh", mesh);
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
		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"RectShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Sprite Shader
		shader = Resources::Find<Shader>(L"SpriteShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// UI Shader
		shader = Resources::Find<Shader>(L"UIShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 3
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Debug Shader
		shader = Resources::Find<Shader>(L"DebugShader");
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
		shader = Resources::Find<Shader>(L"GridShader");
		GetDevice()->CreateInputLayout(arrLayoutDesc, 2
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());

		// Fade Shader
		shader = Resources::Find<Shader>(L"FadeShader");
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
		constantBuffers[(UINT)eCBType::Transform] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[(UINT)eCBType::Transform]->Create(sizeof(TransformCB));

		constantBuffers[(UINT)eCBType::Material] = new ConstantBuffer(eCBType::Material);
		constantBuffers[(UINT)eCBType::Material]->Create(sizeof(MaterialCB));

		constantBuffers[(UINT)eCBType::Grid] = new ConstantBuffer(eCBType::Grid);
		constantBuffers[(UINT)eCBType::Grid]->Create(sizeof(GridCB));
	}

	void LoadShader()
	{
		// Default
		std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "main");

		Resources::Insert<Shader>(L"RectShader", shader);

		// Sprite
		shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"SpriteVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"SpritePS.hlsl", "main");

		Resources::Insert<Shader>(L"SpriteShader", shader);

		// UI
		shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"UserInterfaceVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"UserInterfacePS.hlsl", "main");

		Resources::Insert<Shader>(L"UIShader", shader);

		// Grid
		shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"GridVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"GridPS.hlsl", "main");

		Resources::Insert<Shader>(L"GridShader", shader);

		// Fade
		shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"FadeVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"FadePS.hlsl", "main");

		Resources::Insert<Shader>(L"FadeShader", shader);

		// Debug
		shader = std::make_shared<Shader>();
		shader->Create(eShaderStage::VS, L"DebugVS.hlsl", "main");
		shader->Create(eShaderStage::PS, L"DebugPS.hlsl", "main");
		shader->SetRSState(eRSType::SolidNone);
		shader->SetDSState(eDSType::NoWrite);
		shader->SetBSState(eBSType::AlphaBlend);
		shader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		Resources::Insert<Shader>(L"DebugShader", shader);
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
#define SUB_FOLDER_CLEAR(subNumber) subFolderPath##subNumber = WIDE("");
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
			#pragma region 00_Adventurer
			SUB_FOLDER(1, "00_Adventurer");
				#pragma region 00_Idle
				SUB_FOLDER(2, "00_Idle");

				LOAD_TEX("Tex_Character_Player_Adventurer_Idle_0", "CharIdle0.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Idle_1", "CharIdle1.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Idle_2", "CharIdle2.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Idle_3", "CharIdle3.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Idle_4", "CharIdle4.png");
				#pragma endregion
				#pragma region 01_Jump
				SUB_FOLDER(2, "01_Jump");

				LOAD_TEX("Tex_Character_Player_Adventurer_Jump_0", "CharJump.png");
				#pragma endregion
				#pragma region 02_Run
				SUB_FOLDER(2, "02_Run");

				LOAD_TEX("Tex_Character_Player_Adventurer_Run_0", "CharRun0.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Run_1", "CharRun1.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Run_2", "CharRun2.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Run_3", "CharRun3.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Run_4", "CharRun4.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Run_5", "CharRun5.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Run_6", "CharRun6.png");
				LOAD_TEX("Tex_Character_Player_Adventurer_Run_7", "CharRun7.png");
				#pragma endregion
				#pragma region 03_Die
				SUB_FOLDER(2, "03_Die");

				LOAD_TEX("Tex_Character_Player_Adventurer_Die_0", "CharDie.png");
				#pragma endregion
			SUB_FOLDER_ALLCLEAR();
			#pragma endregion
		#pragma endregion
#pragma endregion
#pragma region 01_TitleScene
	#pragma region 00_Layers
		MAIN_FOLDER("01_Scene\\00_TitleScene\\00_Layers");
		
		LOAD_TEX("Tex_Title_Layer_BackCloud", "BackCloud.png");
		LOAD_TEX("Tex_Title_Layer_FrondCloud", "FrontCloud.png");
		LOAD_TEX("Tex_Title_Layer_Sky_Day", "Sky_Day.png");
		LOAD_TEX("Tex_Title_Layer_Sky_Night", "Sky_Night.png");
	#pragma endregion
	#pragma region 01_Object
		MAIN_FOLDER("01_Scene\\00_TitleScene\\01_Object");
		#pragma region Cloud
			SUB_FOLDER(1, "Cloud");
			LOAD_TEX("Tex_Title_Object_MidCloud", "MidCloud0.png");
		#pragma endregion
		#pragma region Bird
			SUB_FOLDER(1, "Bird");
			
			LOAD_TEX("Tex_Title_Object_Bird_0", "Bird0.png");
			LOAD_TEX("Tex_Title_Object_Bird_1", "Bird1.png");
			LOAD_TEX("Tex_Title_Object_Bird_2", "Bird2.png");
			LOAD_TEX("Tex_Title_Object_Bird_3", "Bird3.png");
			LOAD_TEX("Tex_Title_Object_Bird_4", "Bird4.png");
			LOAD_TEX("Tex_Title_Object_Bird_5", "Bird5.png");
			LOAD_TEX("Tex_Title_Object_Bird_6", "Bird6.png");
			LOAD_TEX("Tex_Title_Object_Bird_7", "Bird7.png");
		#pragma endregion
	#pragma endregion
	#pragma region 02_UI
		MAIN_FOLDER("01_Scene\\00_TitleScene\\02_UI");
			#pragma region Logo
				SUB_FOLDER(1, "Logo");
				LOAD_TEX("Tex_Title_UI_MainLogo", "MainLogo.png");
			#pragma endregion
			#pragma region Text
				SUB_FOLDER(1, "Text");
				#pragma region 00_Play_KOR
					SUB_FOLDER(2, "00_Play_KOR");
					LOAD_TEX("Tex_Title_UI_Text_Play_On", "PlayOn_Kor.png");
					LOAD_TEX("Tex_Title_UI_Text_Play_Off", "PlayOff_Kor.png");
			#pragma endregion
				#pragma region 01_Option_KOR
					SUB_FOLDER(2, "01_Option_KOR");
					LOAD_TEX("Tex_Title_UI_Text_Option_On", "OptionOn_Kor.png");
					LOAD_TEX("Tex_Title_UI_Text_Option_Off", "OptionOff_Kor.png");
			#pragma endregion
				#pragma region 02_Exit_KOR
					SUB_FOLDER(2, "02_Exit_KOR");
					LOAD_TEX("Tex_Title_UI_Text_Exit_On", "ExitOn_Kor.png");
					LOAD_TEX("Tex_Title_UI_Text_Exit_Off", "ExitOff_Kor.png");
				#pragma endregion
				SUB_FOLDER_CLEAR(2);
				SUB_FOLDER_CLEAR(1);
			#pragma endregion
			#pragma region Slot
				SUB_FOLDER(1, "Slot");
				#pragma region Base
					SUB_FOLDER(2, "Base");

					LOAD_TEX("Tex_Title_UI_Slot_Base_NoneSelected", "SlotBase.png");
					LOAD_TEX("Tex_Title_UI_Slot_Base_Selected", "SlotBase_Selected.png");
				#pragma endregion
				#pragma region Delete
					SUB_FOLDER(2, "DeleteButton");

					LOAD_TEX("Tex_Title_UI_Slot_DeleteButton", "SlotDeleteButton.png");
					LOAD_TEX("Tex_Title_UI_Slot_DeleteButton_White", "SlotDeleteButtonWhite.png");
				#pragma endregion
				#pragma region Text
					SUB_FOLDER(2, "Text");

					LOAD_TEX("Tex_Title_UI_Slot_Text_SlotText1", "SlotText1_EN.png");
					LOAD_TEX("Tex_Title_UI_Slot_Text_SlotText2", "SlotText2_EN.png");
					LOAD_TEX("Tex_Title_UI_Slot_Text_SlotText3", "SlotText3_EN.png");
				#pragma endregion
				SUB_FOLDER_ALLCLEAR();
			#pragma endregion
	#pragma endregion
#pragma endregion

		FOLDER_ALLCLEAR();

		LOAD_TEX("HPBarTexture", "PlayerLifeBase 1.png");
		LOAD_TEX("Weapon_Legendary_DemonSword_00", "02_Weapon\\04_Legendary\\DemonSword\\DemonSword00.png");

	}

	void LoadMaterial()
	{
		std::shared_ptr<Texture> texture = Resources::Find<Texture>(L"LightSprite");
		std::shared_ptr<Shader> shader = Resources::Find<Shader>(L"RectShader");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(texture);
		Resources::Insert<Material>(L"RectMaterial", material);

		// Sprite
		//std::shared_ptr<Texture> spriteTexture = Resources::Load<Texture>(L"DefaultSprite", L"DefaultSprite.png");
		texture = Resources::Find<Texture>(L"DefaultSprite");
		shader = Resources::Find<Shader>(L"SpriteShader");
		material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert<Material>(L"SpriteMaterial", material);

		// Sprite/Char/Adventurer
		texture = Resources::Find<Texture>(L"Tex_Character_Player_Adventurer_Idle_0");
		shader = Resources::Find<Shader>(L"SpriteShader");
		material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert<Material>(L"Sprite_Char_Adventurer", material);

		// Sprite/Weapon/Legendary/DemonSword
		texture = Resources::Find<Texture>(L"Weapon_Legendary_DemonSword_00");
		shader = Resources::Find<Shader>(L"SpriteShader");
		material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert<Material>(L"Sprite_Weapon_Legendary_DemonSword", material);

		// HPBar
		texture = Resources::Find<Texture>(L"HPBarTexture");
		shader = Resources::Find<Shader>(L"UIShader");
		material = std::make_shared<Material>();
		material->SetShader(shader);
		material->SetTexture(texture);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert<Material>(L"UIMaterial", material);

		// Grid
		shader = Resources::Find<Shader>(L"GridShader");
		material = std::make_shared<Material>();
		material->SetShader(shader);
		shader->SetRSState(eRSType::SolidNone);
		shader->SetDSState(eDSType::NoWrite);
		shader->SetBSState(eBSType::AlphaBlend);
		Resources::Insert<Material>(L"GridMaterial", material);

		// Fade
		shader = Resources::Find<Shader>(L"FadeShader");
		material = std::make_shared<Material>();
		material->SetShader(shader);
		shader->SetBSState(eBSType::AlphaBlend);
		material->SetRenderingMode(eRenderingMode::Transparent);
		Resources::Insert<Material>(L"FadeMaterial", material);

		// Debug
		shader = Resources::Find<Shader>(L"DebugShader");
		material = std::make_shared<Material>();
		material->SetRenderingMode(eRenderingMode::Transparent);
		material->SetShader(shader);
		Resources::Insert<Material>(L"DebugMaterial", material);
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
		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		for (Camera* cam : cameras[(UINT)type])
		{
			if (cam == nullptr)
				continue;

			cam->Render();
		}

		cameras[(UINT)type].clear();
	}

	void Release()
	{
		for (size_t i = 0; i < (UINT)eCBType::End; ++i)
		{
			delete constantBuffers[i];
			constantBuffers[i] = nullptr;
		}
	}
}