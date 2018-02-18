#include "color_widget.hpp"

#include <iostream>

#include "geometry.hpp"

color_widget::color_widget()
    : _color_src(rand())
{
}

void color_widget::on_draw(draw_context & dc, selection_context const & sc) const
{
    SDL_Color c =
        { *reinterpret_cast<Uint8 const *>(&_color_src)
        , *(reinterpret_cast<Uint8 const *>(&_color_src) + 1)
        , *(reinterpret_cast<Uint8 const *>(&_color_src) + 2)
        , 0
        };
    dc.set_color(c);
    dc.draw_rect_filled(get_box());
    if (sc.is_selected_widget(this))
    {
        dc.set_color({255, 255, 255, 0});
        dc.draw_rect(get_box());
    }
}

void color_widget::on_mouse_up_event(mouse_up_event const & e)
{
    if (within_rect(e.position, get_box()))
    {
        std::cout << "mouse event" << std::endl;
        recolor();
        mark_dirty();
    }
}

void color_widget::on_key_event(key_event const & e)
{
    std::cout << "key event" << std::endl;
    recolor();
    mark_dirty();
}

void color_widget::on_activate()
{
    recolor();
    mark_dirty();
}

vec color_widget::min_size_hint() const
{
    // TODO how to dimension ?
    int v = get_context_info().font_line_skip();
    return { v, v };
}

void color_widget::recolor()
{
    _color_src = rand();
}

color_widget::~color_widget()
{
}
