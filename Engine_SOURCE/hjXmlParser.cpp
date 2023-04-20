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

	float XmlParser::GetFloatAttribute(const std::wstring& attibuteFloatName)
	{
		float result = 0.f;
		result = std::stof(mXml->GetAttrib(attibuteFloatName).c_str());

		return result;
	}

	int XmlParser::GetIntAttribute(const std::wstring& attibuteIntName)
	{
		int result = 0;
		result = std::stoi(mXml->GetAttrib(attibuteIntName).c_str());

		return result;
	}

}