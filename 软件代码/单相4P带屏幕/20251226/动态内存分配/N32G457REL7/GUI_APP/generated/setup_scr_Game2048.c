/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_Game2048(lv_ui *ui)
{
    //Write codes Game2048
    ui->Game2048 = lv_obj_create(NULL);
    lv_obj_set_size(ui->Game2048, 320, 240);
    lv_obj_set_scrollbar_mode(ui->Game2048, LV_SCROLLBAR_MODE_OFF);

    //Write style for Game2048, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Game2048, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048, lv_color_hex(0xffedcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_btnm_2048
    ui->Game2048_btnm_2048 = lv_buttonmatrix_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_btnm_2048, 9, 22);
    lv_obj_set_size(ui->Game2048_btnm_2048, 150, 150);
    lv_obj_remove_flag(ui->Game2048_btnm_2048, LV_OBJ_FLAG_CLICKABLE);
    static const char *Game2048_btnm_2048_text_map[] = {" ", " ", " ", " ", "\n", " ", " ", " ", " ", "\n", " ", " ", " ", " ", "\n", " ", " ", " ", " ", "",};
    lv_buttonmatrix_set_map(ui->Game2048_btnm_2048, Game2048_btnm_2048_text_map);

    //Write style for Game2048_btnm_2048, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_btnm_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_btnm_2048, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_btnm_2048, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_btnm_2048, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_btnm_2048, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui->Game2048_btnm_2048, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui->Game2048_btnm_2048, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_btnm_2048, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_btnm_2048, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_btnm_2048, lv_color_hex(0x9c8678), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_btnm_2048, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Game2048_btnm_2048, Part: LV_PART_ITEMS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_btnm_2048, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_btnm_2048, lv_color_hex(0x1a170d), LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_btnm_2048, &lv_font_Alatsi_Regular_13, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_btnm_2048, 255, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_btnm_2048, 8, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_btnm_2048, 255, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_btnm_2048, lv_color_hex(0xBDAC97), LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_btnm_2048, LV_GRAD_DIR_NONE, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_btnm_2048, 4, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Game2048_btnm_2048, lv_color_hex(0x666666), LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Game2048_btnm_2048, 255, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Game2048_btnm_2048, 1, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Game2048_btnm_2048, 0, LV_PART_ITEMS|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Game2048_btnm_2048, 2, LV_PART_ITEMS|LV_STATE_DEFAULT);

    //Write codes Game2048_label_2048
    ui->Game2048_label_2048 = lv_label_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_label_2048, 193, 3);
    lv_obj_set_size(ui->Game2048_label_2048, 100, 50);
    lv_label_set_text(ui->Game2048_label_2048, "2048");
    lv_label_set_long_mode(ui->Game2048_label_2048, LV_LABEL_LONG_WRAP);

    //Write style for Game2048_label_2048, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_label_2048, lv_color_hex(0x9c8678), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_label_2048, &lv_font_Alatsi_Regular_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_label_2048, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_label_2048, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_label_2048, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_btn_left
    ui->Game2048_btn_left = lv_button_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_btn_left, 172, 197);
    lv_obj_set_size(ui->Game2048_btn_left, 40, 35);
    ui->Game2048_btn_left_label = lv_label_create(ui->Game2048_btn_left);
    lv_label_set_text(ui->Game2048_btn_left_label, "" LV_SYMBOL_LEFT "");
    lv_label_set_long_mode(ui->Game2048_btn_left_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Game2048_btn_left_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Game2048_btn_left, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Game2048_btn_left_label, LV_PCT(100));

    //Write style for Game2048_btn_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Game2048_btn_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_btn_left, lv_color_hex(0x9c8678), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_btn_left, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Game2048_btn_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_btn_left, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_btn_left, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Game2048_btn_left, lv_color_hex(0xBDAC97), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Game2048_btn_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Game2048_btn_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Game2048_btn_left, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Game2048_btn_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_btn_left, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_btn_left, &lv_font_Alatsi_Regular_23, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_btn_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_btn_left, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_btn_new_game
    ui->Game2048_btn_new_game = lv_button_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_btn_new_game, 172, 60);
    lv_obj_set_size(ui->Game2048_btn_new_game, 137, 29);
    ui->Game2048_btn_new_game_label = lv_label_create(ui->Game2048_btn_new_game);
    lv_label_set_text(ui->Game2048_btn_new_game_label, "New  Game");
    lv_label_set_long_mode(ui->Game2048_btn_new_game_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Game2048_btn_new_game_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Game2048_btn_new_game, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Game2048_btn_new_game_label, LV_PCT(100));

    //Write style for Game2048_btn_new_game, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Game2048_btn_new_game, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_btn_new_game, lv_color_hex(0x9c8678), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_btn_new_game, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Game2048_btn_new_game, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_btn_new_game, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_btn_new_game, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_btn_new_game, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_btn_new_game, &lv_font_Acme_Regular_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_btn_new_game, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_btn_new_game, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_btn_right
    ui->Game2048_btn_right = lv_button_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_btn_right, 266, 197);
    lv_obj_set_size(ui->Game2048_btn_right, 40, 35);
    ui->Game2048_btn_right_label = lv_label_create(ui->Game2048_btn_right);
    lv_label_set_text(ui->Game2048_btn_right_label, "" LV_SYMBOL_RIGHT "");
    lv_label_set_long_mode(ui->Game2048_btn_right_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Game2048_btn_right_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Game2048_btn_right, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Game2048_btn_right_label, LV_PCT(100));

    //Write style for Game2048_btn_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Game2048_btn_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_btn_right, lv_color_hex(0x9c8678), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_btn_right, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Game2048_btn_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_btn_right, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_btn_right, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Game2048_btn_right, lv_color_hex(0xBDAC97), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Game2048_btn_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Game2048_btn_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Game2048_btn_right, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Game2048_btn_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_btn_right, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_btn_right, &lv_font_Alatsi_Regular_23, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_btn_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_btn_right, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_btn_up
    ui->Game2048_btn_up = lv_button_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_btn_up, 219, 153);
    lv_obj_set_size(ui->Game2048_btn_up, 40, 35);
    ui->Game2048_btn_up_label = lv_label_create(ui->Game2048_btn_up);
    lv_label_set_text(ui->Game2048_btn_up_label, "" LV_SYMBOL_UP "");
    lv_label_set_long_mode(ui->Game2048_btn_up_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Game2048_btn_up_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Game2048_btn_up, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Game2048_btn_up_label, LV_PCT(100));

    //Write style for Game2048_btn_up, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Game2048_btn_up, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_btn_up, lv_color_hex(0x9c8678), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_btn_up, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Game2048_btn_up, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_btn_up, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_btn_up, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Game2048_btn_up, lv_color_hex(0xBDAC97), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Game2048_btn_up, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Game2048_btn_up, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Game2048_btn_up, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Game2048_btn_up, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_btn_up, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_btn_up, &lv_font_Alatsi_Regular_23, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_btn_up, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_btn_up, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_btn_down
    ui->Game2048_btn_down = lv_button_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_btn_down, 219, 197);
    lv_obj_set_size(ui->Game2048_btn_down, 40, 35);
    ui->Game2048_btn_down_label = lv_label_create(ui->Game2048_btn_down);
    lv_label_set_text(ui->Game2048_btn_down_label, "" LV_SYMBOL_DOWN "");
    lv_label_set_long_mode(ui->Game2048_btn_down_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Game2048_btn_down_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Game2048_btn_down, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Game2048_btn_down_label, LV_PCT(100));

    //Write style for Game2048_btn_down, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Game2048_btn_down, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_btn_down, lv_color_hex(0x9c8678), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_btn_down, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Game2048_btn_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_btn_down, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_btn_down, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Game2048_btn_down, lv_color_hex(0xBDAC97), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Game2048_btn_down, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Game2048_btn_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Game2048_btn_down, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Game2048_btn_down, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_btn_down, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_btn_down, &lv_font_Alatsi_Regular_23, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_btn_down, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_btn_down, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_label_score_title
    ui->Game2048_label_score_title = lv_label_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_label_score_title, 172, 99);
    lv_obj_set_size(ui->Game2048_label_score_title, 66, 44);
    lv_label_set_text(ui->Game2048_label_score_title, "Score");
    lv_label_set_long_mode(ui->Game2048_label_score_title, LV_LABEL_LONG_WRAP);

    //Write style for Game2048_label_score_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_label_score_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_label_score_title, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_label_score_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_label_score_title, &lv_font_Alatsi_Regular_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_label_score_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Game2048_label_score_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Game2048_label_score_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_label_score_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_label_score_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_label_score_title, lv_color_hex(0xBDAC97), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_label_score_title, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_label_score_title, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_label_score_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_label_score_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_label_score_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_label_score_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_label_score
    ui->Game2048_label_score = lv_label_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_label_score, 178, 120);
    lv_obj_set_size(ui->Game2048_label_score, 54, 22);
    lv_label_set_text(ui->Game2048_label_score, "0");
    lv_label_set_long_mode(ui->Game2048_label_score, LV_LABEL_LONG_WRAP);

    //Write style for Game2048_label_score, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_label_score, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_label_score, &lv_font_Alatsi_Regular_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_label_score, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_label_score, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_label_score, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_label_best_title
    ui->Game2048_label_best_title = lv_label_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_label_best_title, 244, 99);
    lv_obj_set_size(ui->Game2048_label_best_title, 66, 44);
    lv_label_set_text(ui->Game2048_label_best_title, "Best");
    lv_label_set_long_mode(ui->Game2048_label_best_title, LV_LABEL_LONG_WRAP);

    //Write style for Game2048_label_best_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_label_best_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_label_best_title, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_label_best_title, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_label_best_title, &lv_font_Alatsi_Regular_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_label_best_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Game2048_label_best_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Game2048_label_best_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_label_best_title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_label_best_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_label_best_title, lv_color_hex(0xBDAC97), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_label_best_title, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_label_best_title, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_label_best_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_label_best_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_label_best_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_label_best_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_label_best
    ui->Game2048_label_best = lv_label_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_label_best, 251, 120);
    lv_obj_set_size(ui->Game2048_label_best, 54, 22);
    lv_label_set_text(ui->Game2048_label_best, "0");
    lv_label_set_long_mode(ui->Game2048_label_best, LV_LABEL_LONG_WRAP);

    //Write style for Game2048_label_best, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_label_best, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_label_best, &lv_font_Alatsi_Regular_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_label_best, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_label_best, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_label_best, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_cont_msgbox
    ui->Game2048_cont_msgbox = lv_obj_create(ui->Game2048);
    lv_obj_set_pos(ui->Game2048_cont_msgbox, 0, 0);
    lv_obj_set_size(ui->Game2048_cont_msgbox, 320, 238);
    lv_obj_set_scrollbar_mode(ui->Game2048_cont_msgbox, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(ui->Game2048_cont_msgbox, LV_OBJ_FLAG_HIDDEN);

    //Write style for Game2048_cont_msgbox, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_cont_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_cont_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_cont_msgbox, 180, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_cont_msgbox, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_cont_msgbox, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_cont_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_cont_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_cont_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_cont_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_cont_msgbox, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_label_warning
    ui->Game2048_label_warning = lv_label_create(ui->Game2048_cont_msgbox);
    lv_obj_set_pos(ui->Game2048_label_warning, 77, 37);
    lv_obj_set_size(ui->Game2048_label_warning, 164, 47);
    lv_label_set_text(ui->Game2048_label_warning, "Game Over");
    lv_label_set_long_mode(ui->Game2048_label_warning, LV_LABEL_LONG_WRAP);

    //Write style for Game2048_label_warning, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_label_warning, lv_color_hex(0xc41111), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_label_warning, &lv_font_Alatsi_Regular_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_label_warning, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_label_warning, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_label_warning, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_label_results_title
    ui->Game2048_label_results_title = lv_label_create(ui->Game2048_cont_msgbox);
    lv_obj_set_pos(ui->Game2048_label_results_title, 102, 88);
    lv_obj_set_size(ui->Game2048_label_results_title, 60, 86);
    lv_label_set_text(ui->Game2048_label_results_title, "Move: \nScore: \nBest: ");
    lv_label_set_long_mode(ui->Game2048_label_results_title, LV_LABEL_LONG_WRAP);

    //Write style for Game2048_label_results_title, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_label_results_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_label_results_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_label_results_title, lv_color_hex(0x766d68), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_label_results_title, &lv_font_Alatsi_Regular_17, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_label_results_title, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Game2048_label_results_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Game2048_label_results_title, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_label_results_title, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_label_results_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_label_results_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_label_results_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_label_results_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_label_results_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_label_results_title, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_btn_again
    ui->Game2048_btn_again = lv_button_create(ui->Game2048_cont_msgbox);
    lv_obj_set_pos(ui->Game2048_btn_again, 120, 187);
    lv_obj_set_size(ui->Game2048_btn_again, 80, 35);
    ui->Game2048_btn_again_label = lv_label_create(ui->Game2048_btn_again);
    lv_label_set_text(ui->Game2048_btn_again_label, "Try Again");
    lv_label_set_long_mode(ui->Game2048_btn_again_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Game2048_btn_again_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Game2048_btn_again, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Game2048_btn_again_label, LV_PCT(100));

    //Write style for Game2048_btn_again, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Game2048_btn_again, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Game2048_btn_again, lv_color_hex(0x766d68), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Game2048_btn_again, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Game2048_btn_again, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_btn_again, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_btn_again, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_btn_again, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_btn_again, &lv_font_Alatsi_Regular_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_btn_again, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_btn_again, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Game2048_label_results
    ui->Game2048_label_results = lv_label_create(ui->Game2048_cont_msgbox);
    lv_obj_set_pos(ui->Game2048_label_results, 178, 88);
    lv_obj_set_size(ui->Game2048_label_results, 68, 86);
    lv_label_set_text(ui->Game2048_label_results, "45\n999\n1285");
    lv_label_set_long_mode(ui->Game2048_label_results, LV_LABEL_LONG_WRAP);

    //Write style for Game2048_label_results, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Game2048_label_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Game2048_label_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Game2048_label_results, lv_color_hex(0x766d68), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Game2048_label_results, &lv_font_Alatsi_Regular_17, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Game2048_label_results, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Game2048_label_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Game2048_label_results, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Game2048_label_results, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Game2048_label_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Game2048_label_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Game2048_label_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Game2048_label_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Game2048_label_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Game2048_label_results, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Game2048.


    //Update current screen layout.
    lv_obj_update_layout(ui->Game2048);

    //Init events for screen.
    events_init_Game2048(ui);
}
