//// LayerStack.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the layer stack class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include <memory>
#include <string>

namespace nexo::layer {
    class Layer;
}

namespace nexo::layer {
    class LayerStack {
        public:
            LayerStack();
            ~LayerStack();

            void pushLayer(const std::shared_ptr<Layer>& layer);
            void pushOverlay(const std::shared_ptr<Layer>& layer);
            void popLayer(const std::shared_ptr<Layer>& layer);
            void popOverlay(const std::shared_ptr<Layer>& layer);

            std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_layers.begin(); };
            std::vector<std::shared_ptr<Layer>>::iterator end() { return m_layers.end(); };

            [[nodiscard]] std::vector<std::shared_ptr<Layer>>::const_iterator begin() const { return m_layers.begin(); }
            [[nodiscard]] std::vector<std::shared_ptr<Layer>>::const_iterator end() const { return m_layers.end(); }

            [[nodiscard]] std::size_t size() const {return m_layers.size();};

            std::shared_ptr<Layer> &operator[](const unsigned int index) { return m_layers[index]; };
            const std::shared_ptr<Layer> &operator[](const unsigned int index) const {return m_layers[index]; };

            std::shared_ptr<Layer> operator[](const std::string &name);
            std::shared_ptr<Layer> operator[](const std::string &name) const;

        private:
            std::vector<std::shared_ptr<Layer>> m_layers;
            std::vector<std::shared_ptr<Layer>>::iterator m_layerInsert;
    };
}
