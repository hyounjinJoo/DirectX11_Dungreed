#include "pch.h"
#include "hjBaseRenderer.h"

namespace hj
{
	BaseRenderer::BaseRenderer(eComponentType type)
		: Component(type)
	{
	}

	BaseRenderer::BaseRenderer(const BaseRenderer& baseRenderer)
		: Component(baseRenderer.GetOrder())
	{
		if (baseRenderer.mMesh)
		{
			mMesh = baseRenderer.mMesh;
		}

		if (baseRenderer.mMaterial)
		{
			mMaterial = baseRenderer.mMaterial;
		}
	}

	BaseRenderer::~BaseRenderer()
	{
	}
	void BaseRenderer::Initialize()
	{
	}
	void BaseRenderer::Update()
	{
	}
	void BaseRenderer::FixedUpdate()
	{
	}
	void BaseRenderer::Render()
	{
	}

	hj::Component* BaseRenderer::Clone() const
	{
		return new BaseRenderer(*this);
	}

}