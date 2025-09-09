//// TypeErasedProperty.cpp ///////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Guillaume HEIN
//  Date:        25/06/2025
//  Description: Implementation file for the type erased property class
//               used to display and edit entity properties
//               for C# defined components
//
///////////////////////////////////////////////////////////////////////////////

#include "TypeErasedProperty.hpp"

#include "ImNexo/Components.hpp"
#include "ImNexo/Elements.hpp"

namespace nexo::editor {

    void showField(const ecs::Field& field, void *data)
    {
        switch (field.type) {
            case ecs::FieldType::Bool:
                static_assert(sizeof(bool) == 1 && "Size of bool must be 1 byte");
                ImGui::Checkbox(field.name.c_str(), static_cast<bool *>(data));
                break;
            case ecs::FieldType::Int8:
                ImGui::InputScalar(field.name.c_str(), ImGuiDataType_S8, data);
                break;
            case ecs::FieldType::Int16:
                ImGui::InputScalar(field.name.c_str(), ImGuiDataType_S16, data);
                break;
            case ecs::FieldType::Int32:
                ImGui::InputScalar(field.name.c_str(), ImGuiDataType_S32, data);
                break;
            case ecs::FieldType::Int64:
                ImGui::InputScalar(field.name.c_str(), ImGuiDataType_S64, data);
                break;
            case ecs::FieldType::UInt8:
                ImGui::InputScalar(field.name.c_str(), ImGuiDataType_U8, data);
                break;;
            case ecs::FieldType::UInt16:
                ImGui::InputScalar(field.name.c_str(), ImGuiDataType_U16, data);
                break;
            case ecs::FieldType::UInt32:
                ImGui::InputScalar(field.name.c_str(), ImGuiDataType_U32, data);
                break;
            case ecs::FieldType::UInt64:
                ImGui::InputScalar(field.name.c_str(), ImGuiDataType_U64, data);
                break;
            case ecs::FieldType::Float:
                ImGui::InputFloat(field.name.c_str(), static_cast<float *>(data));
                break;
            case ecs::FieldType::Double:
                ImGui::InputDouble(field.name.c_str(), static_cast<double *>(data));
                break;

            // Widgets
            case ecs::FieldType::Vector3:
                if (ImGui::BeginTable("InspectorTransformTable", 4,
                    ImGuiTableFlags_SizingStretchProp))
                {
                    // Only the first column has a fixed width
                    ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                    ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                    ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                    ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                    ImNexo::RowDragFloat3(field.name.c_str(), "X", "Y", "Z", static_cast<float *>(data));

                    ImGui::EndTable();
                }
                break;
            case ecs::FieldType::Vector4:
                ImGui::Text("Cannot edit Vector4 for now"); // TODO: Implement Vector4 editing
                break;
            default: return;
        }
    }

    void TypeErasedProperty::show(const ecs::Entity entity)
    {
        if (!m_description) {
            ImGui::Text("No component description available for type %d", m_componentType);
            return;
        }

        const auto& coordinator = Application::m_coordinator;

        const auto componentData = static_cast<uint8_t *>(coordinator->tryGetComponentById(m_componentType, entity));
        if (!componentData) {
            ImGui::Text("Entity %d does not have component type %d", entity, m_componentType);
            return;
        }

        if (ImNexo::Header(std::format("##{}", m_description->name), m_description->name + " Component"))
        {
            for (const auto& field : m_description->fields) {
                // Move to pointer to next field data
                const auto currentComponentData = componentData + field.offset;
                // Show the field in the UI
                showField(field, currentComponentData);
            }

            ImGui::TreePop();
        }


    }


}
