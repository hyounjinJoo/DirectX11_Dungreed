#include "hjActor.h"
#include "hjRoomBase.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjXmlParser.h"
#include "hjAnimator.h"

namespace hj
{
	Actor::Actor()
		: mOwnerRoom(nullptr)
		, mOwnerActor(nullptr)
	{
	}

	Actor::Actor(const Actor& actor)
		: GameObject(actor)
		, mOwnerRoom(nullptr)
		, mOwnerActor(actor.mOwnerActor)
	{

	}

	Actor::~Actor()
	{
		if (mOwnerRoom)
		{
			mOwnerRoom = nullptr;
		}

		SafeDeleteSprites();
	}

	hj::GameObject* Actor::Clone()
	{
		return new Actor(*this);
	}

	void Actor::Death()
	{
		if (mOwnerActor)
		{
			mOwnerActor = nullptr;
		}

		SafeDeleteSprites();

		if (mOwnerRoom)
		{
			mOwnerRoom->DelistFromManagedGameObjects(this);
		}

		GameObject::Death();
	}

	void Actor::SetOwnerRoom(RoomBase* room)
	{
		if (!room)
		{
			return;
		}

		// 소유된 룸이 존재하면 이 Actor를 Room에서 제거하고 소속된 룸을 변경해야 한다.
		if (mOwnerRoom)
		{
			mOwnerRoom->DelistFromManagedGameObjects(this);
		}

		room->AddObjectToManagedGameObjects(this);
		mOwnerRoom = room;
	}

	RoomBase* Actor::GetOwnerRoom()
	{
		RoomBase* result = nullptr;
		if (mOwnerRoom)
		{
			result = mOwnerRoom;
		}

		return result;
	}

	void Actor::SetNameAndCreateSpriteRenderer(const std::wstring& name, const std::wstring& mtrlName, const std::wstring& meshName)
	{
		SetName(name);

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND_WSTR(mtrlName);
		std::shared_ptr<Mesh> mesh = MESH_FIND_WSTR(meshName);

		assert(material && mesh);

		sr->SetMaterial(material);
		sr->SetMesh(mesh);
	}

	std::shared_ptr<Texture> Actor::CheckHasMaterialAndTexture(const eTextureSlot& slot)
	{
		SpriteRenderer* sr = GetComponent<SpriteRenderer>();
		assert(sr);

		std::shared_ptr<Material> material = sr->GetMaterial();
		assert(material);

		std::shared_ptr<Texture> texture = material->GetTexture(slot);
		assert(texture);

		return texture;
	}

	void Actor::LoadAnimInfoFromFile(const eFrameAddMethod& addMethod, const eTextureSlot& slot, const std::wstring& metaDataFilePath,
									const std::wstring& searchWstr, const std::wstring& createdAnimNameWstr,
									float frameDuration, bool bUseCount, int startCount, bool bReversePlay)
	{
		std::shared_ptr<Texture> texture = CheckHasMaterialAndTexture(slot);
		Vector2 atlasTexSize = texture->GetTexSize();

		XmlParser* parser = new XmlParser;
		bool parseResult = parser->LoadFile(metaDataFilePath);

		if (!parseResult)
		{
			delete parser;
			return;
		}

		parseResult = parser->FindElem(WIDE("TextureAtlas"));
		parseResult = parser->IntoElem();

		std::wstring targetSpriteNameWstr;
		int count = startCount;

		Vector2 leftTop = Vector2::Zero;
		Vector2 size = Vector2::One;
		Vector2 offset = Vector2::Zero;
		Vector2 originSize = Vector2::Zero;
		Vector2 trimmedSize = Vector2::One;
		Vector2 trimmedOffset = Vector2::Zero;

		size_t checkStringParseValue = std::wstring::npos;
		{
			CREATE_ANIM(anim, frame, atlasTexSize, frameDuration);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				if (bUseCount)
				{
					targetSpriteNameWstr = searchWstr + std::to_wstring(count);
				}
				else
				{
					targetSpriteNameWstr = searchWstr;
				}

				checkStringParseValue = parser->CheckAttributeIncludeWstr(WIDE("n"), targetSpriteNameWstr);
				if (std::wstring::npos != checkStringParseValue)
				{
					leftTop.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
					leftTop.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
					size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
					size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
					offset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("pX")));
					offset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("pY")));
					trimmedSize.x = static_cast<float>(parser->GetFloatAttribute(WIDE("tW")));
					trimmedSize.y = static_cast<float>(parser->GetFloatAttribute(WIDE("tH")));
					trimmedOffset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("tOX")));
					trimmedOffset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("tOY")));

					switch (addMethod)
					{
					case hj::eFrameAddMethod::FRAME_ADD:
						FRAME_ADD(frame, leftTop.x, leftTop.y, size.x, size.y, anim);
						break;
					case hj::eFrameAddMethod::FRAME_ADD_OFFSETX:
						FRAME_ADD_OFFSETX(frame, leftTop.x, leftTop.y, size.x, size.y, offset.x, anim);
						break;
					case hj::eFrameAddMethod::FRAME_ADD_OFFSET:
						FRAME_ADD_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y, offset.x, offset.y, anim);
						break;
					case hj::eFrameAddMethod::FRAME_ADD_OFFSET_TRIM_OFFSET:
						FRAME_ADD_OFFSET_TRIM_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y
							, offset.x, offset.y, trimmedSize.x, trimmedSize.y
							, trimmedOffset.x, trimmedOffset.y, anim);
						break;
					case hj::eFrameAddMethod::End:
					default:
						assert(false);
						break;
					}

					++count;
				}
			}

			assert(anim.size() > 0);

			mCanvasSize = canvasSize;
			SpritesInfo spritesInfo = {};
			
			spritesInfo.spritesName = createdAnimNameWstr;
			
			spritesInfo.spriteSheet.resize(anim.size());
			std::copy(anim.begin(), anim.end(), spritesInfo.spriteSheet.begin());

			spritesInfo.bIsReversePlay = bReversePlay;

			mSprites.push_back(spritesInfo);
		}

		delete parser;
	}

	void Actor::CreateAnimation(const eTextureSlot& slot, bool bScaleSetCanvasSize)
	{
		Animator* animator = GetComponent<Animator>();
		assert(animator);

		std::shared_ptr<Texture> texture = CheckHasMaterialAndTexture(slot);

		for (const auto& sprite : mSprites)
		{
			animator->Create(sprite.spritesName, texture, sprite.spriteSheet, mCanvasSize, sprite.bIsReversePlay);
		}

		if (bScaleSetCanvasSize)
		{
			SetScaleXY(mCanvasSize);
			GetTransform()->FixedUpdate();
		}

		SafeDeleteSprites();
	}
	
	void Actor::CalcOffsetAuto(const std::wstring& spriteSheetName)
	{
		for (SpritesInfo iter : mSprites)
		{
			if (iter.spritesName == spriteSheetName)
			{
				for (auto& spriteFrame : iter.spriteSheet)
				{
					if (spriteFrame.size.x != mCanvasSize.x)
					{
						spriteFrame.offset.x = (mCanvasSize.x - spriteFrame.size.x) / 2.f - mCanvasSize.x * 0.13f;
					}
					else
					{
						spriteFrame.offset.x = 0.f;
					}
					
					if (spriteFrame.size.y != mCanvasSize.y)
					{
						spriteFrame.offset.y = -(mCanvasSize.y - spriteFrame.size.y) / 2.f;
					}
					else
					{
						spriteFrame.offset.y = 0.f;
					}
				}
			}
		}
	}

	void Actor::CalcOffsetAutoX(const std::wstring& spriteSheetName)
	{
		for (SpritesInfo iter : mSprites)
		{
			if (iter.spritesName == spriteSheetName)
			{
				for (auto& spriteFrame : iter.spriteSheet)
				{
					if (spriteFrame.size.x != mCanvasSize.x)
					{
						spriteFrame.offset.x = (mCanvasSize.x - spriteFrame.size.x) / 2.f - mCanvasSize.x * 0.13f;
					}
					else
					{
						spriteFrame.offset.x = 0.f;
					}
				}
			}
		}
	}

	void Actor::PauseAnimation()
	{
		Animator* animator = GetComponent<Animator>();
		if (animator)
		{
			Animation* animation = animator->GetCurrentAnimation();
			if (animation)
			{
				animation->SetPause(true);
			}
		}
	}

	Actor* Actor::GetOwnerActor()
	{
		if (mOwnerActor)
		{
			return mOwnerActor;
		}

		return nullptr;
	}

	void Actor::SafeDeleteSprites()
	{
		for (auto& spritesHasName : mSprites)
		{
			spritesHasName.spriteSheet.clear();
		}
		mSprites.clear();
	}

}