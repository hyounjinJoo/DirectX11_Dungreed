#include "hjTileMap.h"
#include "hjResources.h"
#include "hjGameObject.h"

namespace hj
{
	TileMap::TileMap()
		: BaseRenderer(eComponentType::TileMap)
		, mRowCount(0)
		, mColCount(0)
		, mTileCountX(0)
		, mTileCountY(0)
		, mbBufferUpdated(false)
		, mBuffer(nullptr)
	{
		SetName(WIDE("TileMap Comp"));

		std::shared_ptr<Material> material = MTRL_FIND("test");
		if (material)
		{
			SetMaterial(material);
			std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
			if (mesh)
			{
				SetMesh(mesh);
			}
		}

		mTileData.resize(static_cast<size_t>(mTileCountX * mTileCountY));
		mBuffer = new StructuredBuffer;
	}

	TileMap::~TileMap()
	{
		if (nullptr != mBuffer)
			delete mBuffer;

		mBuffer = nullptr;
	}

	void TileMap::Initialize()
	{
		BaseRenderer::Initialize();
	}
	
	void TileMap::Update()
	{
		BaseRenderer::Update();
	}
	
	void TileMap::FixedUpdate()
	{
		BaseRenderer::FixedUpdate();
	}

	void TileMap::UpdateData()
	{
		if (nullptr == mAtlasTexture)
			return;
		
		GetMaterial()->SetData(eGPUParam::Int_1, &mTileCountX);
		GetMaterial()->SetData(eGPUParam::Int_2, &mTileCountY);
		GetMaterial()->SetData(eGPUParam::Vector2_1, &mSliceUV);
		
		// 모든 타일 데이터(m_vecTileData) 를 구조화버퍼를 통해 t16 레지스터로 바인딩
		if (false == mbBufferUpdated)
		{
			mBuffer->SetData(mTileData.data(), mTileCountX * mTileCountY);
			mbBufferUpdated = true;
		}
		
		mBuffer->BindSRV(eShaderStage::PS, 16);
	}

	void TileMap::Render()
	{
		if (nullptr == GetMesh() || nullptr == GetMaterial() || nullptr == mAtlasTexture)
			return;

		UpdateData();

		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
		GetMesh()->BindBuffer();
		GetMaterial()->Bind();
		GetMesh()->Render();

		GetMaterial()->Clear();
	}

	void TileMap::SetTileMapCount(UINT countX, UINT countY)
	{
		mTileCountX = countX;
		mTileCountY = countY;

		ClearTileData();
	}
	
	void TileMap::SetTileData(int tileIdx, int imgIdx)
	{
		if (nullptr == mAtlasTexture)
		{
			return;
		}

		mTileData[tileIdx].mImgIdx = imgIdx;

		// 아틀라스에서 타일의 행, 렬 개수 구하기
		mColCount = (UINT)mAtlasTexture->GetWidth() / (UINT)mSlicePixel.x;
		mRowCount = (UINT)mAtlasTexture->GetHeight() / (UINT)mSlicePixel.y;

		int row = mTileData[tileIdx].mImgIdx / mColCount;
		int col = mTileData[tileIdx].mImgIdx % mColCount;

		mTileData[tileIdx].mStartUV = Vector2(mSliceUV.x * col, mSliceUV.y * row);

		mbBufferUpdated = false;
	}
	
	void TileMap::ClearTileData()
	{
		std::vector<TileData> tileData;
		mTileData.swap(tileData);

		mTileData.resize(static_cast<size_t>(mTileCountX * mTileCountY));

		// 구조화 버퍼도 크기에 대응한다.
		size_t bufferSize = static_cast<size_t>(mTileCountX * mTileCountY) * sizeof(TileData);

		if (mBuffer->GetTotalSize() < bufferSize)
		{
			//UINT size, UINT stride, eSRVType type, void* data
			mBuffer->Create(sizeof(TileData), mTileCountX * mTileCountY, eSRVType::SRV, nullptr, true);
		}

		mbBufferUpdated = false;
	}

	void TileMap::SetAllTileData(int imgIdx)
	{
		size_t Size = mTileData.size();
		for (int i = 0; i < Size; ++i)
		{
			SetTileData(i, imgIdx);
		}
	}

	void TileMap::SetAllTileData(const std::vector<TileData>& data)
	{
		ClearTileData();

		mTileData.resize(static_cast<size_t>(mTileCountX * mTileCountY));
		std::copy(data.begin(), data.end(), mTileData.begin());

		// 구조화 버퍼도 크기에 대응한다.
		size_t bufferSize = static_cast<size_t>(mTileCountX * mTileCountY) * sizeof(TileData);

		if (mBuffer->GetTotalSize() < bufferSize)
		{
			//UINT size, UINT stride, eSRVType type, void* data
			mBuffer->Create(sizeof(TileData), mTileCountX * mTileCountY, eSRVType::SRV, nullptr, true);
		}

		mbBufferUpdated = false;
	}
}