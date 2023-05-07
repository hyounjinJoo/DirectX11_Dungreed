#pragma once

#include "CommonInclude.h"
#include "hjMath.h"
#include "hjGraphicDevice_DX11.h"

#include "hjMesh.h"
#include "hjShader.h"
#include "hjConstantBuffer.h"
#include "hjCamera.h"
#include "hjLight.h"
#include "hjStructuredBuffer.h"

using namespace hj::math;
using namespace hj::graphics;

namespace hj::renderer
{
	struct Vertex
	{
		Vector4 pos;
		Vector4 color;
		Vector2 uv;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		int iData1 = -1;
		int iData2 = -1;
		int iData3 = -1;
		int iData4 = -1;

		float fData1 = -1.f;
		float fData2 = -1.f;
		float fData3 = -1.f;
		float fData4 = -1.f;

		Vector2 xy1 = {};
		Vector2 xy2 = {};
		Vector2 xy3 = {};
		Vector2 xy4 = {};

		Vector3 xyz1 = {};
		float	xyzPadding1 = -1;
		Vector3 xyz2 = {};
		float	xyzPadding2 = -1;
		Vector3 xyz3 = {};
		float	xyzPadding3 = -1;
		Vector3 xyz4 = {};
		float	xyzPadding4 = -1;

		Vector4 xyzw1 = {};
		Vector4 xyzw2 = {};
		Vector4 xyzw3 = {};
		Vector4 xyzw4 = {};

		Matrix matrix1 = {};
		Matrix matrix2 = {};
		Matrix matrix3 = {};
		Matrix matrix4 = {};
	};

	CBUFFER(GridCB, CBSLOT_GRID)
	{
		Vector4 cameraPosition;
		Vector2 cameraScale;
		Vector2 resolution;
		float	meshScale;

		Vector3 padding;
	};

	CBUFFER(AnimationCB, CBSLOT_ANIMATION)
	{
		// 0 ~ 16
		Vector2 leftTop;
		Vector2 size;

		// 16 ~ 32
		Vector2 offset;
		Vector2 atlasSize;

		// 32 ~ 40
		Vector2 canvasSize;
		// 40 ~ 44
		UINT used;
		// 44 ~ 48
		UINT canvasUsed;
		// 48 ~ 52
		UINT inverse;

		// 52 ~ 56
		UINT type;

		// 56 ~ 64
		Vector2 padding;
	};

	CBUFFER(LightCB, CBSLOT_NUMBEROFLIGHT)
	{
		UINT numberOfLight;
	};

	CBUFFER(GlobalCB, CBSLOT_GLOBAL)
	{
		// 0 ~ 4
		float globalDeltaTime;
		// 4 ~ 8
		float globalAccTime;
		// 8 ~ 16
		Vector2 globalResolution;
		// 16 ~ 24
		Vector2 globalNoiseResolution;
		// 24 ~ 32
		Vector2 padding;
	};

	CBUFFER(ParticleSystemCB, CBSLOT_PARTICLESYSTEM)
	{
		// 0 ~ 48
		Vector4 worldPosition;
		Vector4 startColor;
		Vector4 startSize;

		// 48 ~ 64
		UINT maxParticles;
		UINT simulationSpace;
		float radius;
		float startSpeed;

		// 64 ~ 80
		float startLifeTime;
		float deltaTime;
		float elapsedTime; //누적시간
		int padding;
	}; 
		
	CBUFFER(NoiseCB, CBSLOT_NOISE)
	{
		// 0 ~ 16
		Vector4 noiseSize;
	};

#pragma region extern Variables
	extern Vertex vertexes[4];
	extern Camera* mainCamera;
	extern ConstantBuffer* constantBuffers[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthstencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[];

	extern std::vector<Camera*> cameras[];
	extern std::vector<DebugMesh> debugMeshes;
	extern std::vector<LightAttribute> lights;
	extern StructuredBuffer* lightsBuffer;
#pragma endregion

	void Initialize();
	void Render();
	void Release();

	void PushLightAttribute(LightAttribute lightAttribute);
	void BindLights();
	void BindNoiseTexture();
}