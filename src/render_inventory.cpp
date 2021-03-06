#include "render_inventory.hpp"

#include <string>
#include <iostream>

#include "init.hpp"
#include "actor_player.hpp"
#include "msg_log.hpp"
#include "render.hpp"
#include "map.hpp"
#include "item.hpp"
#include "text_format.hpp"

namespace
{

void draw_item_symbol(const Item& item, const Pos& p)
{
    const Clr item_clr = item.clr();

    if (config::is_tiles_mode())
    {
        render::draw_tile(item.tile(), Panel::screen, p, item_clr);
    }
    else //Text mode
    {
        render::draw_glyph(item.glyph(), Panel::screen, p, item_clr);
    }
}

void draw_weight_pct(const int Y, const int ITEM_NAME_X, const size_t ITEM_NAME_LEN,
                     const Item& item, const Clr& item_name_clr, const bool IS_SELECTED)
{
    const int WEIGHT_CARRIED_TOT = map::player->inv().total_item_weight();
    const int WEIGHT_PCT         = (item.weight() * 100) / WEIGHT_CARRIED_TOT;

    std::string weight_str  = to_str(WEIGHT_PCT) + "%";
    int         weight_x    = DESCR_X0 - 1 - weight_str.size();

    assert(WEIGHT_PCT >= 0 && WEIGHT_PCT <= 100);

    if (WEIGHT_PCT > 0 && WEIGHT_PCT < 100)
    {
        const Pos weight_pos(weight_x, Y);

        const Clr weight_clr = IS_SELECTED ? clr_white : clr_gray_drk;

        render::draw_text(weight_str, Panel::screen, weight_pos, weight_clr);
    }
    else //"Zero" weight, or 100% of weight - no weight percent should be displayed
    {
        weight_str  = "";
        weight_x    = DESCR_X0 - 1;
    }

    int dots_x = ITEM_NAME_X + ITEM_NAME_LEN;
    int dots_w = weight_x - dots_x;

    if (dots_w <= 0)
    {
        //Item name does not fit, draw a few dots up until the weight percentage
        dots_w              = 3;
        const int DOTS_X1   = weight_x - 1;
        dots_x              = DOTS_X1 - dots_w + 1;
    }

    const std::string   dots_str(dots_w, '.');
    Clr                 dots_clr = IS_SELECTED ? clr_white : item_name_clr;

    if (!IS_SELECTED)
    {
        dots_clr.r /= 2;
        dots_clr.g /= 2;
        dots_clr.b /= 2;
    }

    render::draw_text(dots_str, Panel::screen, Pos(dots_x, Y), dots_clr);
}

void draw_detailed_item_descr(const Item* const item)
{
    std::vector<Str_and_clr> lines;

    if (item)
    {
        const auto base_descr = item->descr();

        if (!base_descr.empty())
        {
            for (const std::string& paragraph : base_descr)
            {
                lines.push_back({paragraph, clr_white_high});
            }
        }

        const bool          IS_PLURAL   = item->nr_items_ > 1 && item->data().is_stackable;
        const std::string   weight_str  = (IS_PLURAL ? "They are " : "It is ") +
                                          item->weight_str() + " to carry.";

        lines.push_back({weight_str, clr_green});

        const int WEIGHT_CARRIED_TOT = map::player->inv().total_item_weight();
        const int WEIGHT_PCT         = (item->weight() * 100) / WEIGHT_CARRIED_TOT;

        if (WEIGHT_PCT > 0 && WEIGHT_PCT < 100)
        {
            const std::string pct_str = "(" + to_str(WEIGHT_PCT) + "% of total carried weight)";
            lines.push_back({pct_str, clr_green});
        }
    }

    //We draw the description box regardless of whether the lines are empty or not,
    //just to clear this area on the screen.
    render::draw_descr_box(lines);
}

} //Namespace

namespace render_inventory
{

void draw_browse_inv(const Menu_browser& browser)
{
    TRACE_FUNC_BEGIN_VERBOSE;

    render::clear_screen();

    const int     BROWSER_Y   = browser.y();
    const auto&   inv         = map::player->inv();
    const size_t  NR_SLOTS    = size_t(Slot_id::END);

    const bool    IS_IN_EQP   = BROWSER_Y < int(NR_SLOTS);
    const size_t  INV_ELEMENT = IS_IN_EQP ? 0 : (size_t(BROWSER_Y) - NR_SLOTS);

    const auto* const item    = IS_IN_EQP ?
                                inv.slots_[BROWSER_Y].item :
                                inv.backpack_[INV_ELEMENT];

    const std::string query_eq_str   = item ? "unequip" : "equip";
    const std::string query_base_str = "[enter] to " + (IS_IN_EQP ? query_eq_str : "apply item");
    const std::string query_drop_str = item ? " [shift+enter] to drop" : "";

    std::string str = query_base_str + query_drop_str + " [space/esc] to exit";

    render::draw_text(str, Panel::screen, Pos(0, 0), clr_white_high);

    Pos p(1, EQP_Y0);

    const Panel panel = Panel::screen;

    for (size_t i = 0; i < NR_SLOTS; ++i)
    {
        const bool          IS_CUR_POS  = IS_IN_EQP && BROWSER_Y == int(i);
        const Inv_slot&     slot        = inv.slots_[i];
        const std::string   slot_name   = slot.name;

        p.x = 1;

        render::draw_text(slot_name, panel, p, IS_CUR_POS ? clr_white_high : clr_menu_drk);

        p.x += 9; //Offset to leave room for slot label

        const auto* const cur_item = slot.item;

        if (cur_item)
        {
            draw_item_symbol(*cur_item, p);
            p.x += 2;

            const Clr clr = IS_CUR_POS ? clr_white_high : cur_item->interface_clr();

            const Item_data_t&  d       = cur_item->data();
            Item_ref_att_inf    att_inf = Item_ref_att_inf::none;

            if (slot.id == Slot_id::wielded || slot.id == Slot_id::wielded_alt)
            {
                //Thrown weapons are forced to show melee info instead
                att_inf = d.main_att_mode == Main_att_mode::thrown ? Item_ref_att_inf::melee :
                          Item_ref_att_inf::wpn_context;
            }
            else if (slot.id == Slot_id::thrown)
            {
                att_inf = Item_ref_att_inf::thrown;
            }

            Item_ref_type ref_type = Item_ref_type::plain;

            if (slot.id == Slot_id::thrown)
            {
                ref_type = Item_ref_type::plural;
            }

            std::string item_name = cur_item->name(ref_type, Item_ref_inf::yes, att_inf);

            text_format::first_to_upper(item_name);

            render::draw_text(item_name, panel, p, clr);

            draw_weight_pct(p.y, p.x, item_name.size(), *cur_item, clr, IS_CUR_POS);
        }
        else //No item in this slot
        {
            p.x += 2;
            render::draw_text("<empty>", panel, p, IS_CUR_POS ? clr_white_high : clr_menu_drk);
        }

        ++p.y;
    }

    const size_t  NR_INV_ITEMS  = inv.backpack_.size();

    size_t inv_top_idx = 0;

    if (!IS_IN_EQP && NR_INV_ITEMS > 0)
    {
        auto is_browser_pos_on_scr = [&](const bool IS_FIRST_SCR)
        {
            const int MORE_LABEL_H = IS_FIRST_SCR ? 1 : 2;
            return int(INV_ELEMENT) < (int(inv_top_idx + INV_H) - MORE_LABEL_H);
        };

        if (int(NR_INV_ITEMS) > INV_H && !is_browser_pos_on_scr(true))
        {
            inv_top_idx = INV_H - 1;

            while (true)
            {
                //Check if this is the bottom screen
                if (int(NR_INV_ITEMS - inv_top_idx) + 1 <= INV_H)
                {
                    break;
                }

                //Check if current browser pos is currently on screen
                if (is_browser_pos_on_scr(false))
                {
                    break;
                }

                inv_top_idx += INV_H - 2;
            }
        }
    }

    const int INV_X0  = 1;

    p = Pos(INV_X0, INV_Y0);

    const int INV_ITEM_NAME_X = INV_X0 + 2;

    for (size_t i = inv_top_idx; i < NR_INV_ITEMS; ++i)
    {
        const bool IS_CUR_POS = !IS_IN_EQP && INV_ELEMENT == i;
        Item* const cur_item   = inv.backpack_[i];

        const Clr clr = IS_CUR_POS ? clr_white_high : cur_item->interface_clr();

        if (i == inv_top_idx && inv_top_idx > 0)
        {
            p.x = INV_ITEM_NAME_X;
            render::draw_text("(more)", panel, p, clr_black, clr_gray);
            ++p.y;
        }

        p.x = INV_X0;

        draw_item_symbol(*cur_item, p);

        p.x = INV_ITEM_NAME_X;

        std::string item_name = cur_item->name(Item_ref_type::plural, Item_ref_inf::yes,
                                               Item_ref_att_inf::wpn_context);

        text_format::first_to_upper(item_name);

        render::draw_text(item_name, panel, p, clr);

        draw_weight_pct(p.y, INV_ITEM_NAME_X, item_name.size(), *cur_item, clr, IS_CUR_POS);

        ++p.y;

        if (p.y == INV_Y1 && ((i + 1) < (NR_INV_ITEMS - 1)))
        {
            render::draw_text("(more)", panel, p, clr_black, clr_gray);
            break;
        }
    }

    const Rect eqp_rect(0, EQP_Y0 - 1, DESCR_X0 - 1, EQP_Y1 + 1);
    const Rect inv_rect(0, INV_Y0 - 1, DESCR_X0 - 1, INV_Y1 + 1);

    render::draw_popup_box(eqp_rect, panel, clr_popup_box, false);
    render::draw_popup_box(inv_rect, panel, clr_popup_box, false);

    if (config::is_tiles_mode())
    {
        render::draw_tile(Tile_id::popup_ver_r, panel, inv_rect.p0, clr_popup_box);
        render::draw_tile(Tile_id::popup_ver_l, panel, Pos(inv_rect.p1.x, inv_rect.p0.y),
                          clr_popup_box);
    }

    draw_detailed_item_descr(item);

    render::update_screen();

    TRACE_FUNC_END_VERBOSE;
}

void draw_equip(const Menu_browser& browser, const Slot_id slot_id_to_equip,
                const std::vector<size_t>& gen_inv_indexes)
{
    TRACE_FUNC_BEGIN_VERBOSE;

    assert(slot_id_to_equip != Slot_id::END);

    Pos p(0, 0);

    const int NR_ITEMS = browser.nr_of_items_in_first_list();
    render::cover_area(Panel::screen, Pos(0, 1), Pos(MAP_W, NR_ITEMS + 1));

    const bool HAS_ITEM = !gen_inv_indexes.empty();

    std::string str = "";

    switch (slot_id_to_equip)
    {
    case Slot_id::wielded:
        str = HAS_ITEM ?
              "Wield which item?" :
              "I carry no weapon to wield.";
        break;

    case Slot_id::wielded_alt:
        str = HAS_ITEM ?
              "Prepare which weapon?" :
              "I carry no weapon to wield.";
        break;

    case Slot_id::thrown:
        str = HAS_ITEM ?
              "Use which item as thrown weapon?" :
              "I carry no weapon to throw." ;
        break;

    case Slot_id::body:
        str = HAS_ITEM ?
              "Wear which armor?" :
              "I carry no armor.";
        break;

    case Slot_id::head:
        str = HAS_ITEM ?
              "Wear what on head?" :
              "I carry no headwear.";
        break;

    case Slot_id::neck:
        str = HAS_ITEM ?
              "Wear what around the neck?" :
              "I carry nothing to wear around the neck.";
        break;

    case Slot_id::ring1:
    case Slot_id::ring2:
        str = HAS_ITEM ?
              "Wear what ring?" :
              "I carry no ring.";
        break;

    case Slot_id::END: {}
        break;
    }

    if (HAS_ITEM)
    {
        str += " [shift+enter] to drop";
    }

    str += cancel_info_str;

    render::draw_text(str, Panel::screen, p, clr_white_high);

    ++p.y;

    Inventory& inv = map::player->inv();

    const int NR_INDEXES = gen_inv_indexes.size();

    for (int i = 0; i < NR_INDEXES; ++i)
    {
        const bool IS_CUR_POS = browser.pos().y == int(i);
        p.x = 0;

        Item* const item = inv.backpack_[gen_inv_indexes[i]];

        draw_item_symbol(*item, p);
        p.x += 2;

        const Clr item_interf_clr = IS_CUR_POS ? clr_white_high : item->interface_clr();

        const Item_data_t& d    = item->data();
        Item_ref_att_inf att_inf  = Item_ref_att_inf::none;

        if (slot_id_to_equip == Slot_id::wielded || slot_id_to_equip == Slot_id::wielded_alt)
        {
            //Thrown weapons are forced to show melee info instead
            att_inf = d.main_att_mode == Main_att_mode::thrown ? Item_ref_att_inf::melee :
                      Item_ref_att_inf::wpn_context;
        }
        else if (slot_id_to_equip == Slot_id::thrown)
        {
            att_inf = Item_ref_att_inf::thrown;
        }

        str = item->name(Item_ref_type::plural, Item_ref_inf::yes, att_inf);

        text_format::first_to_upper(str);

        render::draw_text(str, Panel::screen, p, item_interf_clr);
        ++p.y;
    }

    render::update_screen();

    TRACE_FUNC_END_VERBOSE;
}

} //render_inventory
