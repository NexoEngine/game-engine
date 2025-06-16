//// PrimitiveWindow.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzzzzzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzzz           zzzz        zzzz          zzzz
//  zzz    zzzzzzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz     zzzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Marie Giacomel
//  Date:        06/06/2025
//  Description: Header file for the primitive customization window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"
#include "EntityFactory3d.hpp" // Include the header file where Primitives is defined

namespace nexo::editor {

	class PrimitiveWindow final : public ADocumentWindow {
		public:
			using ADocumentWindow::ADocumentWindow;
			void setup() override;

			void show() override;

			// No-op method in this class
        	void shutdown() override;

            // No-op method in this class
         	void update() override;

			/**
			 * @brief Sets the selected primitive type for customization.
			 *
			 * @param primitive The type of primitive to set as selected.
			 */
			void setSelectedPrimitive(const Primitives primitive) {
				m_selectedPrimitive = primitive;
			}

		private:
            /**
			 * @brief Renders the primitive customization window for the selected primitive.
			 *
			 * This method is responsible for rendering the UI elements related to primitive customization.
			 * It checks if the primitive window is visible, sets up the appropriate window flags,
			 * and calls the `renderPrimitiveWindow` method to handle the actual rendering of the primitive.
			 */
			void renderPrimitiveWindow();

			Primitives m_selectedPrimitive = CUBE; // Default primitive type
	};

}
