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
			 * @brief Sets the visibility flag for the sub-inspector of type T.
			 *
			 * Assigns a new visibility state for the sub-inspector identified by type T. If no visibility entry exists for T,
			 * one is created in the internal visibility map.
			 *
			 * @tparam T The type of the sub-inspector.
			 * @param visible True to make the sub-inspector visible; false to hide it.
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
			 * @brief Associates a Material pointer with the sub-inspector of type T.
			 *
			 * Stores the given Material pointer in the sub-inspector data map, using the type index of T as the key.
			 *
			 * @tparam T The type of sub-inspector for which the data is being set.
			 * @param data Pointer to the Material data to associate with the sub-inspector.
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
			 * @brief Retrieves the visibility flag for a sub-inspector of type T.
			 *
			 * This method checks for the visibility state associated with the sub-inspector type T.
			 * If a visibility flag has been previously set, it returns the stored value;
			 * otherwise, it returns false.
			 *
			 * @tparam T The sub-inspector type.
			 * @return bool True if the sub-inspector is marked as visible, false otherwise.
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
			 * @brief Returns a modifiable reference to the visibility flag for a sub-inspector of type T.
			 *
			 * Accesses the internal visibility map keyed by the type index of T. If no entry exists,
			 * one is created and default-initialized to false.
			 *
			 * @tparam T The type of the sub-inspector.
			 * @return bool& Reference to the visibility flag associated with the sub-inspector.
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
			 * @brief Retrieves the data associated with a sub-inspector of type T.
			 *
			 * This templated method searches for data keyed by the type index of T in the inspector's
			 * data map. If data for the specified sub-inspector type exists, it returns the associated
			 * pointer wrapped in a std::variant; otherwise, it returns a std::variant containing std::monostate.
			 *
			 * @tparam T The sub-inspector type whose data is being retrieved.
			 * @return A std::variant holding either a pointer to components::Material if data exists, or
			 *         std::monostate if no data is associated.
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
