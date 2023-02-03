
#include "hjRenderer.h"
#include "hjResources.h"

namespace hj::renderer
{
	Vertex vertexes[4] = {};

	Mesh* mesh = nullptr;
	ConstantBuffer* constantBuffers[(UINT)eCBType::End] = {};
	Shader* shader = nullptr;

	void SetUpState()
	{
		// Input Layout ( 정점 구조 정보 )
		D3D11_INPUT_ELEMENT_DESC arrLayoutDesc[2] = {};

		arrLayoutDesc[0].AlignedByteOffset = 0;
		arrLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		arrLayoutDesc[0].InputSlot = 0;
		arrLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[0].SemanticName = "POSITION";
		arrLayoutDesc[0].SemanticIndex = 0;

		arrLayoutDesc[1].AlignedByteOffset = 12;
		arrLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		arrLayoutDesc[1].InputSlot = 0;
		arrLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		arrLayoutDesc[1].SemanticName = "COLOR";
		arrLayoutDesc[1].SemanticIndex = 0;

		GetDevice()->CreateInputLayout(arrLayoutDesc, 2
			, shader->GetVSBlobBufferPointer()
			, shader->GetVSBlobBufferSize()
			, shader->GetInputLayoutAddressOf());
	}

	void LoadBuffer()
	{
		// 메시 생성
		mesh = new Mesh();
		Resources::Insert<Mesh>(L"RectMesh", mesh);

		mesh->CreateVertexBuffer(vertexes, 4);

		std::vector<UINT> indexes;
		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		mesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));

		Vector4 pos(0.2f, 0.2f, 0.f, 0.f);
		constantBuffers[(UINT)eCBType::Transform] = new ConstantBuffer();
		constantBuffers[(UINT)eCBType::Transform]->Create(sizeof(Vector4));
		constantBuffers[(UINT)eCBType::Transform]->Bind(&pos);
	}

	void LoadShader()
	{
		shader = new Shader();
		shader->Create(eShaderStage::VS, L"TriangleVS.hlsl", "VS_Test");
		shader->Create(eShaderStage::PS, L"TrianglePS.hlsl", "PS_Test");
	}

	void Initialize()
	{
		// RECT
		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.f);
		vertexes[0].color = Vector4(1.f, 0.f, 0.f, 1.f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.f);
		vertexes[1].color = Vector4(0.f, 1.0f, 0.f, 1.f);

		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.f);
		vertexes[2].color = Vector4(0.f, 0.f, 1.f, 1.f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.f);
		vertexes[3].color = Vector4(1.f, 1.f, 1.f, 1.f);

		LoadShader();
		SetUpState();
		LoadBuffer();
	}

	void Release()
	{
		delete mesh;
		mesh = nullptr;

		delete shader;
		shader = nullptr;

		for (size_t i = 0; i < (UINT)eCBType::End; ++i)
		{
			delete constantBuffers[i];
			constantBuffers[i] = nullptr;
		}
	}
}