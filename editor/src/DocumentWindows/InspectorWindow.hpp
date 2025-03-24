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
#include "DocumentWindows/EntityProperties/AEntityProperty.hpp"
#include "core/scene/SceneManager.hpp"

#include <variant>
#include <unordered_map>

namespace nexo::editor {

    class InspectorWindow final : public ADocumentWindow {
	    public:
			using ADocumentWindow::ADocumentWindow;
	        ~InspectorWindow() override;

	        void setup() override;
	        void shutdown() override;

	        void show() override;
	        void update() override;

			/**
			* @brief Sets the visibility flag for the sub-inspector identified by type T.
			*
			* @tparam T The sub-inspector type.
			* @param visible True if the sub-inspector should be visible, false otherwise.
			*/
			template<typename T>
			/**
			 * @brief Sets the visibility flag for the sub-inspector associated with type T.
			 *
			 * This template method updates the internal mapping of sub-inspector visibility,
			 * associating the specified visibility state with the sub-inspector corresponding
			 * to type T.
			 *
			 * @param visible The desired visibility state (true for visible, false for hidden).
			 */
			void setSubInspectorVisibility(bool visible)
			{
			    m_subInspectorVisibility[std::type_index(typeid(T))] = visible;
			}

			/**
			* @brief Sets the associated data for the sub-inspector identified by type T.
			*
			* @tparam T The sub-inspector type.
			* @param data Pointer to a components::Material instance, or nullptr.
			*/
			template<typename T>
			/**
			 * @brief Associates material data with a sub-inspector of the specified type.
			 *
			 * Maps the type index of the templated sub-inspector (T) to the provided material data. 
			 * If an entry for the given type already exists, it is updated with the new data.
			 *
			 * @tparam T The type of the sub-inspector.
			 * @param data Pointer to the material data to associate with the sub-inspector.
			 */
			void setSubInspectorData(components::Material *data)
			{
			    m_subInspectorData[std::type_index(typeid(T))] = data;
			}

			/**
			* @brief Gets the visibility flag for the sub-inspector identified by type T.
			*
			* @tparam T The sub-inspector type.
			* @return bool True if the sub-inspector is marked as visible; false if not found.
			*/
			template<typename T>
			/**
			 * @brief Retrieves the visibility flag for a specific sub-inspector type.
			 *
			 * This function checks if a visibility flag has been set for the sub-inspector designated by the template parameter T.
			 * If no flag is found, it returns false.
			 *
			 * @tparam T The type of the sub-inspector.
			 * @return true if the sub-inspector is marked as visible; otherwise, false.
			 */
			bool getSubInspectorVisibility() const
			{
			    auto it = m_subInspectorVisibility.find(std::type_index(typeid(T)));
			    return (it != m_subInspectorVisibility.end()) ? it->second : false;
			}

			/**
			* @brief Gets a modifiable reference to the visibility flag for the sub-inspector of type T.
			*
			* This function returns a reference to the boolean stored in m_subInspectorVisibility
			* associated with the sub-inspector type T. If the entry does not exist, it is default-initialized
			* (to false) and inserted into the map.
			*
			* @tparam T The type representing the sub-inspector.
			* @return bool& A modifiable reference to the visibility flag.
			*/
			template<typename T>
			/**
			 * @brief Retrieves a modifiable reference to the visibility flag for a sub-inspector.
			 *
			 * This template method returns a reference to the boolean flag representing the visibility
			 * of the sub-inspector associated with type T. If the flag does not exist, it is default-
			 * initialized (typically to false) and inserted.
			 *
			 * @tparam T The sub-inspector type.
			 * @return bool& A modifiable reference to the visibility flag for the specified sub-inspector.
			 */
			bool& getSubInspectorVisibility()
			{
			    return m_subInspectorVisibility[std::type_index(typeid(T))];
			}

			/**
			* @brief Gets the associated data for the sub-inspector identified by type T.
			*
			* The returned variant will either contain a pointer to a components::Material or std::monostate
			* if no data was set.
			*
			* @tparam T The sub-inspector type.
			* @return std::variant<std::monostate, components::Material*> The stored data.
			*/
			template<typename T>
			/**
			 * @brief Retrieves the material data associated with the specified sub-inspector type.
			 *
			 * This templated function searches for data in the sub-inspector data map using the type index of T.
			 * If an entry for T exists, it returns the associated pointer to a material; otherwise, it returns a variant
			 * containing std::monostate to indicate that no data is set.
			 *
			 * @tparam T The sub-inspector type used to look up the associated data.
			 * @return std::variant<std::monostate, components::Material*> A variant holding a pointer to components::Material if set,
			 *         or std::monostate if no data is available.
			 */
			std::variant<std::monostate, components::Material*> getSubInspectorData() const
			{
			    auto it = m_subInspectorData.find(std::type_index(typeid(T)));
			    return (it != m_subInspectorData.end()) ? it->second : std::variant<std::monostate, components::Material*>{std::monostate{}};
			}
	    private:
			std::unordered_map<std::type_index, std::shared_ptr<IEntityProperty>> m_entityProperties;

			std::unordered_map<std::type_index, bool> m_subInspectorVisibility;
   			std::unordered_map<std::type_index, std::variant<std::monostate, components::Material *>> m_subInspectorData;

			void showSceneProperties(scene::SceneId sceneId) const;
	        void showEntityProperties(ecs::Entity entity);
    };
};
