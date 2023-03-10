#pragma once
#include "CommonInclude.h"
#include "hjMath.h"

namespace hj
{
#pragma region InputEnum
	enum class eKeyCode
	{
		//Alphabet
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,

		//Special Key
		ENTER, ESC, LSHIFT, LALT, LCTRL,
		SPACE, LEFT, RIGHT, UP, DOWN,
		LBTN, RBTN,

		//Num Pad
		NUM_0, NUM_1, NUM_2,
		NUM_3, NUM_4, NUM_5,
		NUM_6, NUM_7, NUM_8,
		NUM_9,

		//Num Line
		N_0, N_1, N_2, N_3, N_4, N_5,
		N_6, N_7, N_8, N_9,

		END,
	};
	enum eKeyState
	{
		DOWN,
		PRESSED,
		UP,
		NONE,
	};
#pragma endregion

	class Input
	{
	public:
		struct Key
		{
			eKeyCode	eType;
			eKeyState	eState;
			bool		bPressed;
		};

#pragma region MemberFunction
		static void Initialize();
		static void Update();
		static void ComputeMousePos();
#pragma endregion

#pragma region Getter
		static __forceinline eKeyState GetKeyState(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].eState;
		}

		static __forceinline math::Vector2 GetMousePosition()
		{
			return mMousePosition;
		}

		static __forceinline math::Vector2 GetMouseWorldPosition()
		{
			return mMouseWorldPosition;
		}

		// 누르는 동안 true 반환
		static __forceinline bool GetKeyPressed(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::PRESSED;
		}

		// 눌렀을 때 true 반환
		static __forceinline bool GetKeyDown(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::DOWN;
		}
		
		// 누르다 뗐을 때 true 반환
		static __forceinline bool GetKeyUp(eKeyCode keyCode)
		{
			return mKeys[static_cast<UINT>(keyCode)].eState == eKeyState::UP;
		}
#pragma endregion

#pragma region DeleteInputDefaultConstructor&Destructor
	private:
		Input() = delete;
		~Input() = delete;
#pragma endregion

#pragma region MemberVariable
	private:
		static std::vector<Key>		mKeys;
		static math::Vector2		mMousePosition;
		static math::Vector2		mMouseWorldPosition;
#pragma endregion
	};
}
