//// InspectorWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie Giacomel
//  Date:        23/11/2024
//  Description: Header file for the inspector window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"
#include "core/scene/SceneManager.hpp"

namespace nexo::editor {

	class MaterialInspector {
		public:
			void setup();
			void show(int selectedEntity);

		private:
			std::shared_ptr<renderer::Framebuffer> m_framebuffer = nullptr;
			int m_ecsEntity = -1;
	};


    class InspectorWindow final : public ADocumentWindow {
	    public:
	        InspectorWindow();
	        ~InspectorWindow() override;

	        void setup() override;
	        void shutdown() override;

	        void show() override;
	        void update() override;

	    private:
	        std::unordered_map<std::type_index, int (*)(ecs::Entity)> m_componentShowFunctions;

			std::shared_ptr<MaterialInspector> m_materialInspector = nullptr;

			static void showSceneProperties(scene::SceneId sceneId);
	        void showEntityProperties(ecs::Entity entity);
    };
};
