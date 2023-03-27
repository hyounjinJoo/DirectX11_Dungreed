#pragma once
#include "hjComputeShader.h"
#include "hjTexture.h"

namespace hj::graphics
{
    class PaintShader :
        public ComputeShader
    {
    public:
        PaintShader();
        virtual ~PaintShader();

        virtual void Binds() override;
        virtual void Clear() override;

        void SetTarget(std::shared_ptr<Texture> texture) { mTarget = texture; }
        void SetNoiseTexture(std::shared_ptr<Texture> texture) { mNoiseTexture = texture; }
    private:
        std::shared_ptr<Texture> mTarget;
        std::shared_ptr<Texture> mNoiseTexture;
        float mThresHold;
        float mCurrentTime;
    };
}