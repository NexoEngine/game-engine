//// LayerStack.cpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the layer stack class
//
///////////////////////////////////////////////////////////////////////////////
#include "LayerStack.hpp"

#include <algorithm>

namespace nexo::layer {
    LayerStack::LayerStack() {
        m_layerInsert = m_layers.begin();
    }

    LayerStack::~LayerStack()
    = default;

    void LayerStack::pushLayer(const std::shared_ptr<layer::Layer> &layer) {
        m_layers.emplace_back(layer);
    }

    void LayerStack::pushOverlay(const std::shared_ptr<layer::Layer> &overlay) {
        m_layerInsert = m_layers.emplace(m_layerInsert, overlay);
    }

    void LayerStack::popLayer(const std::shared_ptr<layer::Layer> &layer) {
        if (const auto it = std::find(m_layers.begin(), m_layers.end(), layer); it != m_layers.end())
            m_layers.erase(it);
    }

    void LayerStack::popOverlay(const std::shared_ptr<layer::Layer> &overlay) {
        if (const auto it = std::find(m_layers.begin(), m_layers.end(), overlay); it != m_layers.end()) {
            m_layers.erase(it);
            --m_layerInsert;
        }
    }
}
