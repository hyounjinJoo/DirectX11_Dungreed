#include "hjXmlParser.h"
namespace hj
{

	XmlParser::XmlParser()
	{
		mXml = new CMarkup;
	}

	XmlParser::~XmlParser()
	{
		if (mXml)
		{
			delete mXml;
			mXml = nullptr;
		}
	}

	bool XmlParser::LoadFile(const std::wstring& name)
	{
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
		std::wstring fullPath = parentPath.wstring() + L"\\..\\Resources\\MetaData\\" + name;

		return mXml->Load(fullPath.c_str());
	}

	bool XmlParser::FindElem(const std::wstring& elementName)
	{
		MCD_CSTR targetElementName = elementName.c_str();
		return mXml->FindElem(targetElementName);
	}

	bool XmlParser::HasAttribute(const std::wstring& attributeName)
	{
		MCD_CSTR targetAttributeName = attributeName.c_str();
		std::wstring attributeValue = mXml->GetAttrib(targetAttributeName);
		
		bool result = false;
		if (!attributeValue.empty())
			result = true;

		return result;
	}

	size_t XmlParser::CheckAttributeIncludeWstr(const std::wstring& attributeName, const std::wstring findWstr)
	{
		// 해당 어트리뷰트가 없을 때
		if (!HasAttribute(attributeName))
			return std::wstring::npos;

		std::wstring targetWstr = GetWstringAttribute(attributeName);
		size_t pos = targetWstr.find(findWstr);

		return pos;
	}

	float XmlParser::GetFloatAttribute(const std::wstring& attibuteFloatName)
	{
		float result = 0.f;
		if (!HasAttribute(attibuteFloatName))
			return result;

		result = std::stof(mXml->GetAttrib(attibuteFloatName).c_str());

		return result;
	}

	int XmlParser::GetIntAttribute(const std::wstring& attibuteIntName)
	{
		int result = 0;
		if (!HasAttribute(attibuteFloatName))
			return result;

		result = std::stoi(mXml->GetAttrib(attibuteIntName).c_str());

		return result;
	}

}