#ifndef LIBWTK_SDL2_LIST_VIEW_HPP
#define LIBWTK_SDL2_LIST_VIEW_HPP

#include <vector>
#include <functional>
#include <optional>

#include "selectable.hpp"

// TODO change to vector_view ?
struct list_view : selectable
{

    list_view(std::vector<std::string> const & values, std::size_t position, std::function<void(std::size_t)> activate_callback);
    ~list_view() override;

    void on_draw(draw_context & dc, selection_context const & sc) const override;
    void on_mouse_up_event(mouse_up_event const & e) override;
    void on_mouse_down_event(mouse_down_event const & e) override;
    void on_key_event(key_event const & e) override;
    void on_activate() override;

    void apply_layout_to_children() override;

    vec min_size_hint() const override;
    vec nat_size_inc_hint() const override;

    // list_view interface

    // Set the position of the first entry to be shown.
    void set_position(std::size_t position);
    void set_list(std::vector<std::string> const & values, std::size_t position = 0);

    std::size_t get_visible_entries() const;

    void scroll_up(std::size_t amount);
    void scroll_down(std::size_t amount);
    void scroll_up();
    void scroll_down();


    private:

    std::optional<point> _opt_pressed_point;

    std::size_t _position;

    std::size_t _selected_position;

    std::size_t _highlight_position;

    std::size_t _x_shift;

    // TODO more columns and other types
    std::reference_wrapper<std::vector<std::string> const> _values;

    std::function<void(std::size_t)> _activate_callback;

    int _row_height;
    int _visible_entries;
};

#endif

