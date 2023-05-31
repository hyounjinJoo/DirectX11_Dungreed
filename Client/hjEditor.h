#pragma once
#include "hjGraphics.h"

namespace hj
{

	namespace gui
	{
		class Widget;
	}
	class EditorObject;
	class DebugObject;

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
		std::vector<gui::Widget*> mWidgets;
		std::vector<EditorObject*> mEditorObjects;
		std::vector<DebugObject*> mDebugObjects;
	};
}