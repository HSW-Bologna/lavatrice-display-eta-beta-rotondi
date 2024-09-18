#include <stdio.h>
#include "view.h"
#include "common.h"
#include "theme/style.h"
#include "intl/intl.h"


#define SQUARE_BUTTON_SIZE 48


void view_common_set_hidden(lv_obj_t *obj, uint8_t hidden) {
    if (hidden) {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}


password_page_options_t *view_common_default_password_page_options(pman_stack_msg_t msg, const char *password) {
    password_page_options_t *fence = (password_page_options_t *)lv_malloc(sizeof(password_page_options_t));
    assert(fence != NULL);
    fence->password = password;
    fence->msg      = msg;
    return fence;
}


view_title_t view_common_create_title(lv_obj_t *root, const char *text, int back_id, int next_id) {
    LV_IMG_DECLARE(img_door);

    lv_obj_t *btn, *lbl, *cont;

    cont = lv_obj_create(root);
    lv_obj_add_style(cont, (lv_style_t *)&style_padless_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(cont, LV_HOR_RES, 56);

    btn = lv_button_create(cont);
    lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
    lv_obj_set_size(btn, SQUARE_BUTTON_SIZE, SQUARE_BUTTON_SIZE);
    lv_obj_t *img = lv_image_create(btn);
    lv_image_set_src(img, &img_door);
    lv_obj_center(img);
    lv_obj_align(btn, LV_ALIGN_LEFT_MID, 4, 0);
    view_register_object_default_callback(btn, back_id);
    lv_obj_t *button_back = btn;

    if (next_id >= 0) {
        btn = lv_button_create(cont);
        lv_obj_add_style(btn, (lv_style_t *)&style_config_btn, LV_STATE_DEFAULT);
        lv_obj_set_size(btn, SQUARE_BUTTON_SIZE, SQUARE_BUTTON_SIZE);
        lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_BIG, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, LV_SYMBOL_RIGHT);
        lv_obj_center(lbl);
        lv_obj_align(btn, LV_ALIGN_RIGHT_MID, -4, 0);
        view_register_object_default_callback(btn, next_id);
    }

    lbl = lv_label_create(cont);
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl, STYLE_FONT_MEDIUM, LV_STATE_DEFAULT);
    lv_label_set_text(lbl, text);

    if (next_id >= 0) {
        lv_obj_set_width(lbl, LV_HOR_RES - SQUARE_BUTTON_SIZE * 2 - 8);
        lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);
    } else {
        lv_obj_set_width(lbl, LV_HOR_RES - SQUARE_BUTTON_SIZE - 8);
        lv_obj_align(lbl, LV_ALIGN_RIGHT_MID, 0, 0);
    }

    return (view_title_t){.label_title = lbl, .button_back = button_back, .obj_title = cont};
}


void view_common_image_set_src(lv_obj_t *img, const lv_image_dsc_t *img_dsc) {
    if (lv_image_get_src(img) != img_dsc) {
        lv_image_set_src(img, img_dsc);
    }
}


communication_error_popup_t view_common_communication_error_popup(lv_obj_t *parent) {
    lv_obj_t *blanket = lv_obj_create(parent);
    lv_obj_add_style(blanket, &style_transparent_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(blanket, LV_PCT(100), LV_PCT(100));

    lv_obj_t *cont = lv_obj_create(blanket);
    lv_obj_set_size(cont, LV_PCT(90), LV_PCT(90));
    lv_obj_center(cont);

    lv_obj_t *lbl_msg = lv_label_create(cont);
    lv_label_set_long_mode(lbl_msg, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(lbl_msg, LV_PCT(95));
    // lv_label_set_text(lbl_msg, view_intl_get_string(model, STRINGS_ERRORE_DI_COMUNICAZIONE));
    lv_obj_align(lbl_msg, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *btn = lv_button_create(cont);
    lv_obj_set_size(btn, 96, 64);
    lv_obj_t *lbl_retry = lv_label_create(btn);
    // lv_label_set_text(lbl_retry, view_intl_get_string(model, STRINGS_RIPROVA));
    lv_obj_center(lbl_retry);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, 0);

    return (communication_error_popup_t){
        .blanket   = blanket,
        .btn_retry = btn,
        .lbl_msg   = lbl_msg,
        .lbl_retry = lbl_retry,
    };
}


const char *view_common_alarm_description(model_t *pmodel) {
    static char codice_generico[32] = {0};
    uint16_t    alarm_code          = model_alarm_code(pmodel);

    switch (alarm_code) {
        case 1:
            return view_intl_get_string(pmodel, STRINGS_ERRORE_EEPROM);
        case 2:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_SPEGNIMENTO);
        case 3:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_COMUNICAZIONE);
        case 4:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_OBLO_APERTO);
        case 5:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_OBLO_SBLOCCATO);
        case 6 ... 7:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_EMERGENZA);
        case 8:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_INVERTER);
        case 9:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_TEMPERATURA_1);
        case 10 ... 11:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_SBILANCIAMENTO);
        case 12:
        case 14:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_CHIAVISTELLO_BLOCCATO);
        case 13:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_SERRATURA);
        case 15:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_APERTO_H2O);
        case 16:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_SERRATURA_FORZATA);
        case 17:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_ACCELEROMETRO);
        case 18:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_ACCELEROMETRO_FUORI_SCALA);
        case 19:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_VELOCITA);
        case 20:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_NO_MOTO);
        case 21:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_NO_FERMO);
        case 22:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_TEMPERATURA);
        case 23:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_H2O_IN_VASCA);
        case 24:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_LIVELLO);
        case 30:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_RIEMPIMENTO);
        case 31:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_RISCALDAMENTO);
        case 32:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_SCARICO);
        case 50:
            return view_intl_get_string(pmodel, STRINGS_MACCHINA_ACCESA);
        case 51:
            return view_intl_get_string(pmodel, STRINGS_ALLARME_SPEGNIMENTO);
        case 52:
            return view_intl_get_string(pmodel, STRINGS_INIZIO_LAVAGGIO);
        case 53:
            return view_intl_get_string(pmodel, STRINGS_FINE_LAVAGGIO);
        case 54:
            return view_intl_get_string(pmodel, STRINGS_LAVAGGIO_INTERROTTO);
        case 60:
            return view_intl_get_string(pmodel, STRINGS_APRIRE_OBLO);
        case 70:
            return view_intl_get_string(pmodel, STRINGS_STANDBY_SAPONI);

        default:
            snprintf(codice_generico, sizeof(codice_generico), "%s %i", view_intl_get_string(pmodel, STRINGS_ALLARME),
                     alarm_code);
            return codice_generico;
    }
}


const char *view_require_payment_string(model_t *pmodel, int language) {
    switch (pmodel->prog.parmac.tipo_gettoniera) {
        case PAGAMENTO_1_LINEA_NA:
        case PAGAMENTO_1_LINEA_NC:
        case PAGAMENTO_2_LINEA_NA:
        case PAGAMENTO_2_LINEA_NC:
            return view_intl_get_string_in_language(language, STRINGS_PAGARE_GETTONE);

        case PAGAMENTO_DIGITALE:
        case PAGAMENTO_DIGITALE_LINEA_SINGOLA:
            return view_intl_get_string_in_language(language, STRINGS_PAGARE_MONETA);

        case PAGAMENTO_CASSA_NA:
        case PAGAMENTO_CASSA_NC:
            return view_intl_get_string_in_language(language, STRINGS_PAGARE_CASSA);

        default:
            return "";
    }
}
