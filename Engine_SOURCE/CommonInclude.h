#pragma once
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <type_traits>
#include <Windows.h>

#define arraysize(a) (sizeof(a) / sizeof(a[0]))
#define WIDE(x) L##x
#define ENUM_TO_STRING(member) \
    ([]() -> std::string { \
        std::string name = #member; \
        size_t pos = name.find("::"); \
        if (pos != std::string::npos) { \
            name.erase(0, pos + 2); \
        } \
        return name; \
    })()

#ifdef _DEBUG
	#include <chrono>
	#include <thread>
	#define DEBUG_PRINT_INTERVAL_MS 500 // 0.5 second interval
	#define DEBUG_PRINT(fmt, ...) \
		do { \
		    static auto lastPrintTime = std::chrono::system_clock::now(); \
		    auto currentTime = std::chrono::system_clock::now(); \
		    if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastPrintTime).count() >= DEBUG_PRINT_INTERVAL_MS) { \
		        char Text[256]; \
		        sprintf_s(Text, fmt "\n", __VA_ARGS__); \
		        OutputDebugStringA(Text); \
		        lastPrintTime = currentTime; \
		    } \
		    std::this_thread::yield(); \
		} while (0)

#define MSG_BOX(title, text) do { \
        WCHAR wtext[MAX_PATH]; \
        WCHAR wtitle[MAX_PATH]; \
        MultiByteToWideChar(CP_UTF8, 0, text, -1, wtext, MAX_PATH); \
        MultiByteToWideChar(CP_UTF8, 0, title, -1, wtitle, MAX_PATH); \
		WCHAR wmsg[MAX_PATH*2]; \
        wsprintf(wmsg, L"%s\n\nFile: %S\nLine: %d", wtext, __FILE__, __LINE__); \
        MessageBox(NULL, wmsg, wtitle, MB_OK); \
    } while (0)
#else
#define DEBUG_PRINT(fmt, ...) do {} while (0)
#define MSG_BOX(title, text) ((void)0)
#endif

// Enable enum flags:
// https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html

template<typename E>
struct enable_bitmask_operators
{
	static constexpr bool enable = false;
};

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type operator|(E lhs, E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	return static_cast<E>
		(
			static_cast<underlying>(lhs) | static_cast<underlying>(rhs)
			);
}

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E&>::type operator|=(E& lhs, E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	lhs = static_cast<E>
		(
			static_cast<underlying>(lhs) | static_cast<underlying>(rhs)
			);

	return lhs;
}

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type operator&(E lhs, E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	return static_cast<E>
		(
			static_cast<underlying>(lhs) & static_cast<underlying>(rhs)
			);
}

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E&>::type operator&=(E& lhs, E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	lhs = static_cast<E>
		(
			static_cast<underlying>(lhs) & static_cast<underlying>(rhs)
			);

	return lhs;
}

template<typename E>
constexpr typename std::enable_if<enable_bitmask_operators<E>::enable, E>::type operator~(E rhs)
{
	typedef typename std::underlying_type<E>::type underlying;
	rhs = static_cast<E>
		(
			~static_cast<underlying>(rhs)
			);

	return rhs;
}

template<typename E>
constexpr bool has_flag(E lhs, E rhs)
{
	return (lhs & rhs) == rhs;
}

#define	PI	3.14159f

static float DegreeToRadian(float Degree)
{
	// (각도 * PI) / 180.f를 이용해서 Radian 값으로 변환
	return Degree * PI / 180.f;
}

static float RadianToDegree(float Radian)
{
	// (라디안 * 180.f) / PI를 이용해서 Radian 값으로 변환
	return Radian * 180.f / PI;
}

static std::wstring StringToWideString(const std::string& str)
{
	int numChars = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	if (numChars == 0) 
	{
		return L"";
	}
	
	std::wstring wideStr(numChars, 0);
	
	if (!MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wideStr[0], numChars)) 
	{
		return L"";
	}
	
	return wideStr;
}

static std::string WideStringToString(const std::wstring& str)
{
	int numChars = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
	if (numChars == 0)
	{
		return "";
	}

	std::string narrowStr(numChars, 0);

	if (!WideCharToMultiByte(CP_UTF8, 0, str.c_str(), -1, &narrowStr[0], numChars, NULL, NULL))
	{
		return "";
	}

	return narrowStr;
}