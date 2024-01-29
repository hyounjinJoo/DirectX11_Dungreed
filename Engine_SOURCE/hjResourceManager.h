#pragma once
#include "hjEngine.h"
#include "hjResource.h"

namespace hj
{
// ��� �� �ݵ�� ;(semi-colon)�� �ٿ��� ����� ��.
#define SHADER_NEW() std::make_shared<Shader>()
#define SHADER_INSERT(shaderKey, sharedPtrShader) ResourceManager::Insert<Shader>(WIDE(shaderKey), sharedPtrShader)
#define SHADER_FIND(shaderKey) ResourceManager::Find<Shader>(WIDE(shaderKey))

#define TEX_FIND(texKey) ResourceManager::Find<Texture>(WIDE(texKey))

#define MTRL_NEW() std::make_shared<Material>()
#define MTRL_INSERT(materialKey, sharedPtrMTRL) ResourceManager::Insert<Material>(WIDE(materialKey), sharedPtrMTRL)
#define MTRL_FIND_STR(materialKeyStr) ResourceManager::Find<Material>(WIDE(materialKeyStr))
#define MTRL_FIND_WSTR(materialKeyWStr) ResourceManager::Find<Material>(materialKeyWStr)

#define MESH_NEW() std::make_shared<Mesh>()
#define MESH_INSERT(meshKey, sharedPtrMesh) ResourceManager::Insert<Mesh>(WIDE(meshKey), sharedPtrMesh)
#define MESH_FIND_STR(meshKeyStr) ResourceManager::Find<Mesh>(WIDE(meshKeyStr))
#define MESH_FIND_WSTR(meshKeyWStr) ResourceManager::Find<Mesh>(meshKeyWStr)

	class ResourceManager
	{
	public:
		template <typename T>
		static std::shared_ptr<T> Find(const std::wstring& key)
		{
			std::map<std::wstring, std::shared_ptr<Resource>>::iterator iter = mResources.find(key);

			// �̹� ������ Ű������ �ٸ� ���ҽ��� ���� ��ϵǾ� �־���.
			if (iter != mResources.end())
			{
				return std::dynamic_pointer_cast<T>(iter->second);
			}

			return nullptr;
		}

		template <typename T>
		static std::shared_ptr<T> Load(const std::wstring& key, const std::wstring& path)
		{
			// Ű������ Ž��
			std::shared_ptr<T> resource = ResourceManager::Find<T>(key);
			if (nullptr != resource)
			{
				// �ش�Ű�� �̹� �ε��Ȱ� ������ �ش� ���ҽ��� ��ȯ
				return resource;
			}

			// �ش� Ű�� �ε��� ���ҽ��� ����.
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
		ResourceManager() = delete;
		~ResourceManager() = delete;
	
	private:
		static std::map<std::wstring, std::shared_ptr<Resource>> mResources;
	};

	
}