#include <stdio.h>
#include "view.h"
#include "common.h"
#include "theme/style.h"
#include "intl/intl.h"


#define SQUARE_BUTTON_SIZE 48


static const char *get_alarm_description(uint16_t alarm_code, uint16_t language);


void view_common_set_hidden(lv_obj_t *obj, uint8_t hidden) {
    if (hidden) {
        lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
    }
}


void view_common_set_disabled(lv_obj_t *obj, uint8_t disabled) {
    if (disabled) {
        lv_obj_add_state(obj, LV_STATE_DISABLED);
    } else {
        lv_obj_clear_state(obj, LV_STATE_DISABLED);
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
    lv_obj_add_style(img, &style_white_icon, LV_STATE_DEFAULT);
    lv_obj_center(img);
    lv_obj_align(btn, LV_ALIGN_LEFT_MID, 4, 0);
    view_register_object_default_callback(btn, back_id);
    lv_obj_t *button_back = btn;

    lv_obj_t *button_next = NULL;
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
        button_next = btn;
    }

    lbl = lv_label_create(cont);
    lv_label_set_long_mode(lbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
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

    return (view_title_t){
        .label_title = lbl,
        .button_back = button_back,
        .button_next = button_next,
        .obj_title   = cont,
    };
}


void view_common_image_set_src(lv_obj_t *img, const lv_image_dsc_t *img_dsc) {
    if (lv_image_get_src(img) != img_dsc) {
        lv_image_set_src(img, img_dsc);
    }
}


void view_common_alarm_popup_update(popup_t *alarm_popup, uint16_t language, uint16_t alarm_code) {
    const char *description = get_alarm_description(alarm_code, language);
    lv_label_set_text_fmt(alarm_popup->lbl_description, "%s\n\n%s: %i", description,
                          view_intl_get_string_in_language(language, STRINGS_CODICE), alarm_code);
}


popup_t view_common_alarm_popup_create(lv_obj_t *parent, int id) {
    return view_common_popup_create(parent, "", id, -1);
}


popup_t view_common_popup_create(lv_obj_t *parent, const char *text, int ok_id, int cancel_id) {
    lv_obj_t *blanket = lv_obj_create(parent);
    lv_obj_add_style(blanket, &style_transparent_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(blanket, LV_PCT(100), LV_PCT(100));

    lv_obj_t *cont = lv_obj_create(blanket);
    lv_obj_set_size(cont, LV_PCT(70), LV_PCT(90));
    lv_obj_center(cont);
    lv_obj_set_style_radius(cont, 48, LV_STATE_DEFAULT);

    lv_obj_t *lbl_msg = lv_label_create(cont);
    lv_obj_set_style_text_align(lbl_msg, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl_msg, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
    lv_label_set_long_mode(lbl_msg, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(lbl_msg, LV_PCT(95));
    lv_label_set_text(lbl_msg, text);
    lv_obj_align(lbl_msg, LV_ALIGN_CENTER, 0, -32);

    lv_obj_t *btn_cancel = NULL;
    if (cancel_id >= 0) {
        lv_obj_t *btn = lv_button_create(cont);
        lv_obj_set_size(btn, 64, 48);
        lv_obj_t *lbl = lv_label_create(btn);
        lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
        lv_label_set_text(lbl, LV_SYMBOL_CLOSE);
        lv_obj_center(lbl);
        lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        view_register_object_default_callback(btn, cancel_id);
        btn_cancel = btn;
    }

    lv_obj_t *btn = lv_button_create(cont);
    lv_obj_set_size(btn, 64, 48);
    lv_obj_t *lbl = lv_label_create(btn);
    lv_obj_set_style_text_font(lbl, STYLE_FONT_SMALL, LV_STATE_DEFAULT);
    lv_label_set_text(lbl, LV_SYMBOL_OK);
    lv_obj_center(lbl);
    if (cancel_id >= 0) {
        lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    } else {
        lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, 0);
    }
    view_register_object_default_callback(btn, ok_id);

    return (popup_t){
        .blanket         = blanket,
        .btn_ok          = btn,
        .btn_cancel      = btn_cancel,
        .lbl_description = lbl_msg,
    };
}


communication_error_popup_t view_common_communication_error_popup(lv_obj_t *parent) {
    lv_obj_t *blanket = lv_obj_create(parent);
    lv_obj_add_style(blanket, &style_transparent_cont, LV_STATE_DEFAULT);
    lv_obj_set_size(blanket, LV_PCT(100), LV_PCT(100));

    lv_obj_t *cont = lv_obj_create(blanket);
    lv_obj_set_size(cont, LV_PCT(80), LV_PCT(80));
    lv_obj_center(cont);

    lv_obj_t *lbl_msg = lv_label_create(cont);
    lv_label_set_long_mode(lbl_msg, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(lbl_msg, LV_PCT(95));
    // lv_label_set_text(lbl_msg, view_intl_get_string(model, STRINGS_ERRORE_DI_COMUNICAZIONE));
    lv_obj_align(lbl_msg, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t *btn = lv_button_create(cont);
    lv_obj_set_size(btn, 96, 48);
    lv_obj_t *lbl_retry = lv_label_create(btn);
    lv_obj_center(lbl_retry);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *btn_disable = lv_button_create(cont);
    lv_obj_set_size(btn_disable, 96, 48);
    lv_obj_t *lbl_disable = lv_label_create(btn_disable);
    lv_obj_center(lbl_disable);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, 0);

    return (communication_error_popup_t){
        .blanket     = blanket,
        .btn_retry   = btn,
        .btn_disable = btn_disable,
        .lbl_msg     = lbl_msg,
        .lbl_retry   = lbl_retry,
        .lbl_disable = lbl_disable,
    };
}


const char *view_common_alarm_description(model_t *pmodel) {
    uint16_t alarm_code = model_alarm_code(pmodel);
    return get_alarm_description(alarm_code, pmodel->run.lingua);
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


const char *view_common_step2str(model_t *model, uint16_t step) {
    const strings_t step2str[NUM_STEPS] = {
        STRINGS_AMMOLLO, STRINGS_PRELAVAGGIO, STRINGS_LAVAGGIO,     STRINGS_RISCIACQUO,
        STRINGS_SCARICO, STRINGS_CENTRIFUGA,  STRINGS_SROTOLAMENTO, STRINGS_ATTESA_OPERATORE,
    };

    assert(step <= NUM_STEPS && step > 0);
    return view_intl_get_string(model, step2str[step - 1]);
}


const char *view_common_pedantic_string(model_t *pmodel) {
    const strings_t pedantic2str[] = {
        STRINGS_PRECARICA_IN_CORSO,
        STRINGS_ATTESA_LIVELLO_E_TEMPERATURA,
        STRINGS_ATTESA_LIVELLO,
        STRINGS_ATTESA_TEMPERATURA,
        STRINGS_RIEMPIMENTO,
        STRINGS_ATTESA_TERMODEGRADAZIONE,
        STRINGS_ATTESA_LIVELLO_SCARICO,
        STRINGS_PRESCARICO,
        STRINGS_PREPARAZIONE,
        STRINGS_RAGGIUNGIMENTO_VELOCITA,
        STRINGS_IN_FRENATA,
        STRINGS_ATTESA_FRENATA,
        STRINGS_SCARICO_FORZATO,
        STRINGS_RECUPERO,
        STRINGS_USCITA_LAVAGGIO,
    };

    if (pmodel->run.macchina.descrizione_pedante > sizeof(pedantic2str) / sizeof(pedantic2str[0]) ||
        pmodel->run.macchina.descrizione_pedante == 0) {
        return "";
    } else {
        return view_intl_get_string(pmodel, pedantic2str[pmodel->run.macchina.descrizione_pedante - 1]);
    }
}


const pman_page_t *view_common_main_page(model_t *model) {
    (void)model;
    return &page_main_self;
}


const pman_page_t *view_common_choice_page(model_t *model) {
    (void)model;
    return &page_choice_self;
}


const pman_page_t *view_common_washing_page(model_t *model) {
    (void)model;
    return &page_washing_self;
}


static const char *get_alarm_description(uint16_t alarm_code, uint16_t language) {
    switch (alarm_code) {
        case 1:
            return view_intl_get_string_in_language(language, STRINGS_ERRORE_EEPROM);
        case 2:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_SPEGNIMENTO);
        case 3:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_COMUNICAZIONE);
        case 4:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_OBLO_APERTO);
        case 5:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_OBLO_SBLOCCATO);
        case 6 ... 7:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_EMERGENZA);
        case 8:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_INVERTER);
        case 9:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_TEMPERATURA_1);
        case 10 ... 11:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_SBILANCIAMENTO);
        case 12:
        case 14:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_CHIAVISTELLO_BLOCCATO);
        case 13:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_SERRATURA);
        case 15:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_APERTO_H2O);
        case 16:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_SERRATURA_FORZATA);
        case 17:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_ACCELEROMETRO);
        case 18:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_ACCELEROMETRO_FUORI_SCALA);
        case 19:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_VELOCITA);
        case 20:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_NO_MOTO);
        case 21:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_NO_FERMO);
        case 22:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_TEMPERATURA);
        case 23:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_H2O_IN_VASCA);
        case 24:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_LIVELLO);
        case 30:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_RIEMPIMENTO);
        case 31:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_RISCALDAMENTO);
        case 32:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_SCARICO);
        case 50:
            return view_intl_get_string_in_language(language, STRINGS_MACCHINA_ACCESA);
        case 51:
            return view_intl_get_string_in_language(language, STRINGS_ALLARME_SPEGNIMENTO);
        case 52:
            return view_intl_get_string_in_language(language, STRINGS_INIZIO_LAVAGGIO);
        case 53:
            return view_intl_get_string_in_language(language, STRINGS_FINE_LAVAGGIO);
        case 54:
            return view_intl_get_string_in_language(language, STRINGS_LAVAGGIO_INTERROTTO);
        case 60:
            return view_intl_get_string_in_language(language, STRINGS_APRIRE_OBLO);
        case 70:
            return view_intl_get_string_in_language(language, STRINGS_STANDBY_SAPONI);

        default: {
            static char codice_generico[32] = {0};
            snprintf(codice_generico, sizeof(codice_generico), "%s %i",
                     view_intl_get_string_in_language(language, STRINGS_ALLARME), alarm_code);
            return codice_generico;
        }
    }
}
