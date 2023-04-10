#pragma once
#include "hjComponent.h"
#include "hjGraphics.h"

namespace hj
{
	using namespace math;

    class Light :
        public Component
    {
    public:
        Light();
        virtual ~Light();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;

        graphics::LightAttribute GetLightAttribute() { return mAttribute; }
        
        void SetDiffuse(Vector4 diffuse) { mAttribute.diffuse = diffuse; }
		void SetSpecular(Vector4 specular) { mAttribute.specular = specular; }
		void SetAmbient(Vector4 ambient) { mAttribute.ambient = ambient; }
		void SetEmissive(Vector4 emissive) { mAttribute.emissive = emissive; }
		void SetPosition(Vector4 position) { mAttribute.position = position; }
		void SetDirection(Vector4 direction) { mAttribute.direction = direction; }
		void SetType(eLightType type) { mAttribute.type = type; }
		void SetRadius(float radius) { mAttribute.radius = radius; }
		void SetAngle(float angle) { mAttribute.angle = angle; }

		const Vector4& GetDiffuse() { return mAttribute.diffuse; }
		const Vector4& GetSpecular() { return mAttribute.specular; }
		const Vector4& GetAmbient() { return mAttribute.ambient; }
		const Vector4& GetEmissive() { return mAttribute.emissive; }
		const Vector4& GetPosition() { return mAttribute.position; }
		const Vector4& GetDirection() { return mAttribute.direction; }
		eLightType GetType() { return static_cast<eLightType>(mAttribute.type); }
		float GetRadius() { return mAttribute.radius; }
		float GetAngle() { return mAttribute.angle; }

    private:
        graphics::LightAttribute mAttribute;
    };
}