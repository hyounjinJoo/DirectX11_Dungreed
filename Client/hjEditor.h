#pragma once
#include "..\Editor_SOURCE\hjDebugObject.h"
#include "..\Editor_SOURCE\hjWidget.h"
#include "..\Editor_SOURCE\hjEditorObject.h"

namespace hj
{
	using namespace gui;
	class Editor
	{
	public:
		void Initialize();
		void Run();

		void Update();
		void FixedUpdate();
		void Render();
		void Release();

		void DebugRender();

	private:
		std::vector<Widget> mWidgets;
		std::vector<EditorObject> mEditorObjects;
		std::vector<DebugObject> mDebugObjects;
	};
}