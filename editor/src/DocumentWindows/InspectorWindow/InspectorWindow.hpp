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

#include <unordered_map>

namespace nexo::editor {

    class InspectorWindow final : public ADocumentWindow {
	    public:
			using ADocumentWindow::ADocumentWindow;
	        ~InspectorWindow() override = default;

			/**
			* @brief Initializes the property handlers for various entity component types.
			*
			* This method populates the internal map that links component type identifiers (obtained via
			* typeid) to their corresponding property display handlers, such as TransformProperty,
			* RenderProperty, and various light and camera properties. These handlers are constructed
			* with the current InspectorWindow instance and later used to display component-specific
			* properties in the inspector UI.
			*/
	        void setup() override;

            /**
             * @brief Registers type-erased properties for all component descriptions in the coordinator.
             *
             * This method should be called after the scripting system has been initialized
             * to ensure all managed components have been registered with their field metadata.
             */
            void registerTypeErasedProperties();

			// No-op method in this class
	        void shutdown() override;

			/**
			* @brief Renders the Inspector window.
			*
			* Opens an ImGui window titled "Inspector" and, on its first display, configures docking by calling
			* firstDockSetup("Inspector"). It retrieves the currently selected entity via the Selector singleton and,
			* if a valid selection exists, displays either scene or entity properties depending on the selection type.
			*/
	        void show() override;

			// No-op method in this class
	        void update() override;

			/**
			 * @brief Sets the visibility flag for the sub-inspector associated with type T.
			 *
			 * This template method updates the internal mapping of sub-inspector visibility,
			 * associating the specified visibility state with the sub-inspector corresponding
			 * to type T.
			 *
			 * @param visible The desired visibility state (true for visible, false for hidden).
			 */
			template<typename T>
			void setSubInspectorVisibility(const bool visible)
			{
			    m_subInspectorVisibility[std::type_index(typeid(T))] = visible;
			}

			/**
			 * @brief Associates material data with a sub-inspector of the specified type.
			 *
			 * Maps the type index of the templated sub-inspector (T) to the provided material data.
			 * If an entry for the given type already exists, it is updated with the new data.
			 *
			 * @tparam T The type of the sub-inspector.
			 * @param data Pointer to the material data to associate with the sub-inspector.
			 */
			template<typename T, typename Data>
			void setSubInspectorData(Data &&data)
			{
			    m_subInspectorData[std::type_index(typeid(T))] = std::forward<Data>(data);
			}

			/**
			 * @brief Retrieves the visibility flag for a specific sub-inspector type.
			 *
			 * This function checks if a visibility flag has been set for the sub-inspector designated by the template parameter T.
			 * If no flag is found, it returns false.
			 *
			 * @tparam T The type of the sub-inspector.
			 * @return true if the sub-inspector is marked as visible; otherwise, false.
			 */
			template<typename T>
			bool getSubInspectorVisibility() const
			{
			    auto it = m_subInspectorVisibility.find(std::type_index(typeid(T)));
			    return (it != m_subInspectorVisibility.end()) ? it->second : false;
			}

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
			template<typename T>
			bool& getSubInspectorVisibility()
			{
			    return m_subInspectorVisibility[std::type_index(typeid(T))];
			}

			/**
			 * @brief Retrieves the material data associated with the specified sub-inspector window type.
			 *
			 * This templated function searches for data in the sub-inspector data map using the type index of WindowType.
			 * If an entry for WindowType exists, it returns the associated pointer to a Data type; otherwise, it returns nullptr
			 *
			 * @tparam WindowType The sub-inspector type used to look up the associated data.
			 * @tparam Data The type of data to retrieve.
			 * @return A pointer to the Data type if found, or nullptr if not found.
			 */
			template<typename WindowType, typename Data>
			std::optional<Data> getSubInspectorData() const
            {
                auto it = m_subInspectorData.find(std::type_index(typeid(WindowType)));
                if (it != m_subInspectorData.end()) {
                    if (auto ptr = std::any_cast<Data>(&it->second)) {
                        return *ptr;
                    } else {
                        LOG(NEXO_ERROR, "Failed to cast sub-inspector data for type {}",
                            typeid(WindowType).name());
                        return std::nullopt;
                    }
                }
                return std::nullopt;
            }
	    private:
			std::unordered_map<ecs::ComponentType, std::shared_ptr<IEntityProperty>> m_entityProperties;

			std::unordered_map<std::type_index, bool> m_subInspectorVisibility;
   			std::unordered_map<std::type_index, std::any> m_subInspectorData;

			/**
			* @brief Displays the scene's properties in the inspector UI.
			*
			* Retrieves the scene corresponding to the provided SceneId and renders UI controls that allow toggling the scene's
			* render and active statuses. The UI is configured with two columns where the "Hide" checkbox inverts the scene's
			* rendering state and the "Pause" checkbox inverts its active state. An icon prefix is removed from the scene's UI handle
			* before display.
			*
			* @param sceneId The identifier of the scene whose properties are to be displayed.
			*/
			static void showSceneProperties(scene::SceneId sceneId);

			/**
			* @brief Renders the UI for the properties of an entity's components.
			*
			* Iterates through all component types associated with the given entity and,
			* for each type that has a registered property handler in the m_entityProperties map,
			* invokes its show() method to display the component's properties in the inspector.
			*
			* @param entity The entity whose component properties are being displayed.
			*/
	        void showEntityProperties(ecs::Entity entity);

			/**
			* @brief Registers a property for a given component type.
			*
			* This function creates a new property instance of type @p Property (which must be derived from AEntityProperty)
			* associated with the given @p Component type. The property is stored in the internal entity properties map,
			* using the type index of @p Component as the key.
			*
			* @tparam Component The type of the component that the property is associated with.
			* @tparam Property The type of the property to register. Must be derived from AEntityProperty.
			*/
			template<typename Component, typename Property>
			requires std::derived_from<Property, AEntityProperty>
			void registerProperty()
			{
			    const auto type = Application::m_coordinator->getComponentType<Component>();
				m_entityProperties[type] = std::make_shared<Property>(*this);
			}

            void registerProperty(const ecs::ComponentType type, std::shared_ptr<IEntityProperty> property)
            {
			    if (!property) {
                    LOG(NEXO_ERROR, "Attempted to register a null property for component type {}", type);
                    return;
                }
                m_entityProperties[type] = std::move(property);
            }

    };
};
