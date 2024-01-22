#ifndef KOTERM_SCREEN_COLORCACHE_H
#define KOTERM_SCREEN_COLORCACHE_H

#include "koterm/screen/Color.h"
#include <algorithm>
#include <cassert>
#include <vector>
namespace koterm::screen {

/**
 * @brief ColorCache class for caching colors.
 * @details The color cache class lacks a remove method because it assumes a higher likelihood of reusing colors than
 * never using them again.
 */
class ColorCache {
public:
    using id_t = std::size_t;

    id_t add(const Color& color) {
        auto it = std::find(m_colors.begin(), m_colors.end(), color);
        if (it != m_colors.end()) {
            return std::distance(m_colors.begin(), it);
        } else {
            m_colors.push_back(color);
            return m_colors.size() - 1;
        }
    }

    id_t add(Color&& color) {
        auto it = std::find(m_colors.begin(), m_colors.end(), color);
        if (it != m_colors.end()) {
            return std::distance(m_colors.begin(), it);
        } else {
            m_colors.push_back(color);
            return m_colors.size() - 1;
        }
    }

    [[nodiscard]] const Color& get(id_t id) const {
        assert(exists(id));
        return m_colors.at(id);
    }

    [[nodiscard]] bool exists(id_t id) const { return id < m_colors.size(); }

    static const Color& get_color(id_t id) { return s_instance.get(id); }
    static id_t add_color(const Color& color) { return s_instance.add(color); }
    static id_t add_color(Color&& color) { return s_instance.add(std::move(color)); }

private:
    ColorCache() = default;

    static ColorCache s_instance;

    std::vector<Color> m_colors;
};

}

#endif
