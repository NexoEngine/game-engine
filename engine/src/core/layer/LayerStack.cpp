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
#include "Layer.hpp"
#include "LayerStack.hpp"
#include "Logger.hpp"

#include <algorithm>

namespace nexo::layer {

    LayerStack::LayerStack()
    {
        m_layerInsert = m_layers.begin();
    }

    LayerStack::~LayerStack() = default;

    void LayerStack::pushLayer(const std::shared_ptr<Layer>& layer)
    {
        if (m_layers.empty())
            m_layerInsert = m_layers.emplace(m_layers.begin(), layer);
        else
            // Insert the layer just after the overlays
                m_layerInsert = m_layers.emplace(m_layerInsert, layer);
    }

    void LayerStack::pushOverlay(const std::shared_ptr<Layer>& overlay)
    {
        // Calculate the distance of m_layerInsert from the beginning
        auto offset = std::distance(m_layers.begin(), m_layerInsert);

        m_layers.insert(m_layers.begin(), overlay);

        // Recalculate m_layerInsert because insert could cause reallocation that invalidates the iterator
        m_layerInsert = m_layers.begin() + offset + 1;
    }

    void LayerStack::popLayer(const std::shared_ptr<Layer>& layer)
    {
        if (const auto it = std::ranges::find(m_layers, layer); it != m_layers.end())
        {
            m_layers.erase(it);

            // Adjust m_layerInsert only if the layer was the one pointed to by m_layerInsert
            if (m_layerInsert == it)
                if (m_layerInsert == m_layers.end())
                    m_layerInsert = m_layers.begin();
        }
    }


    void LayerStack::popOverlay(const std::shared_ptr<Layer>& overlay)
    {
        if (const auto it = std::ranges::find(m_layers, overlay); it != m_layers.end())
        {
            // Erase the overlay
            m_layers.erase(it);

            if (it < m_layerInsert) {
                --m_layerInsert;
            }
        }
    }

    std::shared_ptr<Layer> LayerStack::operator[](const std::string& name)
    {
        for (auto& layer : m_layers)
        {
            if (layer->name == name)
                return layer;
        }
        LOG(NEXO_WARN, "LayerStack: Layer {} not found", name);
        return nullptr;
    }

    std::shared_ptr<Layer> LayerStack::operator[](const std::string& name) const
    {
        for (const auto& layer : m_layers)
        {
            if (layer->name == name)
                return layer;
        }
        LOG(NEXO_WARN, "LayerStack: Layer {} not found", name);
        return nullptr;
    }

    std::shared_ptr<Layer> LayerStack::operator[](unsigned int index)
    {
        if (index >= m_layers.size())
        {
            LOG(NEXO_WARN, "LayerStack: Index {} is out of range", index);
            return nullptr;
        }
        return m_layers[index];
    }

    std::shared_ptr<Layer> LayerStack::operator[](unsigned int index) const
    {
        if (index >= m_layers.size())
        {
            LOG(NEXO_WARN, "LayerStack: Index {} is out of range", index);
            return nullptr;
        }
        return m_layers[index];
    }

    std::shared_ptr<Layer> LayerStack::byId(unsigned int id)
    {
        for (const auto& layer : m_layers)
        {
            if (layer->id == id)
                return layer;
        }
        LOG(NEXO_WARN, "LayerStack: Layer {} not found", id);
        return nullptr;
    }

    std::shared_ptr<Layer> LayerStack::byId(unsigned int id) const
    {
        for (const auto& layer : m_layers)
        {
            if (layer->id == id)
                return layer;
        }
        LOG(NEXO_WARN, "LayerStack: Layer {} not found", id);
        return nullptr;
    }
}
