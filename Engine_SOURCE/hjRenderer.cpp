
#include "hjRenderer.h"
#include "hjResources.h"

namespace hj::renderer
{
	// ���� ������
	Vertex vertexes[4] = {};

	// ����
	Mesh* mesh = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> triangleConstantBuffer = nullptr;

	// ���ؽ� ���̴�
	Microsoft::WRL::ComPtr<ID3DBlob> triangleVSBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> triangleVS = nullptr;

	// �ȼ� ���̴�
	Microsoft::WRL::ComPtr<ID3DBlob> trianglePSBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> trianglePS = nullptr;

	// ��ǲ ���̾ƿ� ( ���� ���� )
	Microsoft::WRL::ComPtr<ID3D11InputLayout> triangleLayout = nullptr;

	void SetUpState()
	{
		// Input Layout ( ���� ���� ���� )
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
			, triangleVSBlob->GetBufferPointer()
			, triangleVSBlob->GetBufferSize()
			, &triangleLayout);
	}

	void LoadBuffer()
	{
		// �޽� ����
		mesh = new Mesh();
		Resources::Insert<Mesh>(L"RectMesh", mesh);

		// ���ؽ� ���� ����
		mesh->CreateVertexBuffer(vertexes, 4);

		// �ε��� ���� ����
		std::vector<UINT> indexes;

		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		
		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		// �ε��� ���� ����
		mesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));

		// ��� ����
		D3D11_BUFFER_DESC csDesc = {};
		csDesc.ByteWidth = sizeof(Vector4);
		csDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		csDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		csDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&csDesc, nullptr, triangleConstantBuffer.GetAddressOf());

		Vector4 pos(0.2f, 0.2f, 0.f, 0.f);
		GetDevice()->BindConstantBuffer(triangleConstantBuffer.Get(), &pos, sizeof(Vector4));
	}

	void LoadShader()
	{
		GetDevice()->CreateShader();
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
	}
}