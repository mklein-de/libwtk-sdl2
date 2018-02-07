#include <algorithm>

#include "box.hpp"

box::box(orientation o, std::vector<box::child> children, int children_spacing, bool children_homogeneous)
    : _children(children)
    , _children_spacing(children_spacing)
    , _children_homogeneous(children_homogeneous)
    , _o(o)
{
    init_children();
}

void box::apply_layout_to_children()
{
    std::size_t const n = _children.size();

    if (n > 0)
    {
        int num_expand = std::count_if(std::begin(_children), std::end(_children), [](auto const & c){ return c.expand; });

        if (_o == orientation::HORIZONTAL)
        {
            // evenly split box
            int const child_width = get_box().w / n;
            int xoffset = get_box().x;

            for (auto c : _children)
            {
                c.wptr->apply_layout({ xoffset, get_box().y, child_width, get_box().h });
                xoffset += child_width;
            }
        }
        else
        {
            int const child_height = get_box().h / n;
            int yoffset = get_box().y;

            for (auto c : _children)
            {
                c.wptr->apply_layout({ get_box().x, yoffset, get_box().w, child_height });
                yoffset += child_height;
            }
        }

    }
}

widget * box::find_selectable(navigation_type nt)
{
    if (is_orthogonal(nt, _o))
    {
        // get middle
        if (!_children.empty())
            return _children[_children.size() / 2].wptr->find_selectable(nt);
    }
    // TODO add point
    else
    {

        if (is_forward(nt))
        {
            for (auto c : _children)
            {
                auto w = c.wptr->find_selectable(nt);
                if (w != nullptr)
                {
                    return w;
                }
            }
        }
        else
        {
            for (auto it = _children.rbegin(); it != _children.rend(); ++it)
            {
                auto w = it->wptr->find_selectable(nt);
                if (w != nullptr)
                {
                    return w;
                }
            }
        }
    }
    return nullptr;
}

bool box::is_orthogonal(navigation_type nt, orientation o)
{
    return
        ((nt == navigation_type::NEXT_X || nt == navigation_type::PREV_X)
            && o == orientation::VERTICAL
        )
        ||
        ((nt == navigation_type::NEXT_Y || nt == navigation_type::PREV_Y)
            && o == orientation::HORIZONTAL
        );
}

bool box::is_forward(navigation_type nt)
{
    return
        nt == navigation_type::NEXT
        || nt == navigation_type::NEXT_X
        || nt == navigation_type::NEXT_Y;
}

widget * box::navigate_selectable_from_children(navigation_type nt, widget * w, point center)
{
    if (is_orthogonal(nt, _o))//nt == navigation_type::NEXT_Y || nt == navigation_type::PREV_Y)
    {
        return navigate_selectable_parent(nt, center);
    }
    else
    {
        auto const begin = _children.begin();
        auto const end = _children.end();

        // locate widget in container
        auto it = std::find_if(begin, end, [w](child const & c){ return c.wptr.get() == w; });

        // find first selectable widget of all navigationally consecutive widgets
        while (true)
        {
            if (is_forward(nt))
            {
                it++;
            }
            else
            {
                it--;
            }

            if (it == end || it < begin)
            {
                return navigate_selectable_parent(nt, rect_center(get_box()));
            }
            else
            {
                auto result = it->wptr->find_selectable(nt);
                if (result != nullptr)
                    return result;
            }
        }
    }
}

vec box::min_size_hint() const
{
    return { 0, 0 };
}

box::~box()
{
}

std::vector<widget *> box::get_children()
{
    std::vector<widget *> result;
    std::for_each(std::begin(_children), std::end(_children), [&](auto c){ result.push_back(c.wptr.get()); });
    return result;
}

std::vector<widget const *> box::get_children() const
{
    std::vector<widget const *> result;
    std::for_each(std::begin(_children), std::end(_children), [&](auto c){ result.push_back(c.wptr.get()); });
    return result;
}

widget_ptr hbox(std::vector<box::child> children, int children_spacing, bool children_homogeneous)
{
    return std::make_shared<box>(box::orientation::HORIZONTAL, children, children_spacing, children_homogeneous);
}

widget_ptr vbox(std::vector<box::child> children, int children_spacing, bool children_homogeneous)
{
    return std::make_shared<box>(box::orientation::VERTICAL, children, children_spacing, children_homogeneous);
}

