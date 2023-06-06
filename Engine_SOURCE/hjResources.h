#pragma once
#include "hjEngine.h"
#include "hjResource.h"

namespace hj
{
// 사용 시 반드시 ;(semi-colon)을 붙여서 사용할 것.
#define SHADER_NEW() std::make_shared<Shader>()
#define SHADER_INSERT(shaderKey, sharedPtrShader) Resources::Insert<Shader>(WIDE(shaderKey), sharedPtrShader)
#define SHADER_FIND(shaderKey) Resources::Find<Shader>(WIDE(shaderKey))

#define TEX_FIND(texKey) Resources::Find<Texture>(WIDE(texKey))

#define MTRL_NEW() std::make_shared<Material>()
#define MTRL_INSERT(materialKey, sharedPtrMTRL) Resources::Insert<Material>(WIDE(materialKey), sharedPtrMTRL)
#define MTRL_FIND_STR(materialKeyStr) Resources::Find<Material>(WIDE(materialKeyStr))
#define MTRL_FIND_WSTR(materialKeyWStr) Resources::Find<Material>(materialKeyWStr)

#define MESH_NEW() std::make_shared<Mesh>()
#define MESH_INSERT(meshKey, sharedPtrMesh) Resources::Insert<Mesh>(WIDE(meshKey), sharedPtrMesh)
#define MESH_FIND_STR(meshKeyStr) Resources::Find<Mesh>(WIDE(meshKeyStr))
#define MESH_FIND_WSTR(meshKeyWStr) Resources::Find<Mesh>(meshKeyWStr)

	class Resources
	{
	public:
		template <typename T>
		static std::shared_ptr<T> Find(const std::wstring& key)
		{
			std::map<std::wstring, std::shared_ptr<Resource>>::iterator iter = mResources.find(key);

			// 이미 동일한 키값으로 다른 리소스가 먼저 등록되어 있었다.
			if (iter != mResources.end())
			{
				return std::dynamic_pointer_cast<T>(iter->second);
			}

			return nullptr;
		}

		template <typename T>
		static std::shared_ptr<T> Load(const std::wstring& key, const std::wstring& path)
		{
			// 키값으로 탐색
			std::shared_ptr<T> resource = Resources::Find<T>(key);
			if (nullptr != resource)
			{
				// 해당키로 이미 로딩된게 있으면 해당 리소스를 반환
				return resource;
			}

			// 해당 키로 로딩된 리소스가 없다.
			resource = std::make_shared<T>();
			if (FAILED(resource->Load(path)))
			{
				MessageBox(nullptr, L"Image Load Failed!", L"Error", MB_OK);
				return nullptr;
			}

			resource->SetKey(key);
			resource->SetPath(path);
			mResources.insert(std::make_pair(key, std::dynamic_pointer_cast<Resource>(resource)));

			return resource;
		}

		template <typename T>
		static void Insert(const std::wstring& key, std::shared_ptr<T> resource)
		{
			mResources.insert(std::make_pair(key, std::dynamic_pointer_cast<Resource>(resource)));
		}

		static void deleteTest()
		{
			int a = 0;
		}

	private:
		Resources() = delete;
		~Resources() = delete;
	
	private:
		static std::map<std::wstring, std::shared_ptr<Resource>> mResources;
	};

	
}
