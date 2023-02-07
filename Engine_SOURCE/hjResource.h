#pragma once
#include "hjEntity.h"

using namespace hj::enums;
namespace hj
{
	class Resource : public Entity
	{
	public:
		Resource(eResourceType type);
		virtual ~Resource();

		virtual HRESULT Load(const std::wstring& path) = 0;

		const std::wstring& GetKey() { return mKey; }
		const std::wstring& GetPath() { return mPath; }

		void SetKey(const std::wstring& key) { mKey = key; }
		void SetPath(const std::wstring& path) { mPath = path; }

	private:
		std::wstring mKey;
		std::wstring mPath;
		eResourceType mType;
	};
}
