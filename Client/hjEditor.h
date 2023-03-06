#pragma once
#include "hjDebugObject.h"
#include "hjWidget.h"
#include "hjEditorObject.h"
#include "hjGraphics.h"

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

		void DebugRender(graphics::DebugMesh& mesh);

	private:
		std::vector<Widget*> mWidgets;
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;
	};
}