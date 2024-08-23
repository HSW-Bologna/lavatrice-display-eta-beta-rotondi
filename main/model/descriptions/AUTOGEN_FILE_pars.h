#ifndef AUTOGEN_FILE_PARS_H_INCLUDED
#define AUTOGEN_FILE_PARS_H_INCLUDED

typedef enum {
    PARS_VISUALIZZAZIONE_SELF = 0,
    PARS_VISUALIZZAZIONE_LABORATORIO,
} pars_visualizzazione_t;

extern const char *pars_visualizzazione[2][5];
typedef enum {
    PARS_TIPO_INVERTER_AVANTI_INDIETRO = 0,
    PARS_TIPO_INVERTER_MARCIA_DIREZIONE,
} pars_tipo_inverter_t;

extern const char *pars_tipo_inverter[2][5];
typedef enum {
    PARS_SCALA_ACCELEROMETRO_0 = 0,
    PARS_SCALA_ACCELEROMETRO_1,
    PARS_SCALA_ACCELEROMETRO_2,
    PARS_SCALA_ACCELEROMETRO_3,
    PARS_SCALA_ACCELEROMETRO_4,
    PARS_SCALA_ACCELEROMETRO_5,
} pars_scala_accelerometro_t;

extern const char *pars_scala_accelerometro[6][5];
typedef enum {
    PARS_AUSILIARI_SBLOCCO_PAGAMENTO = 0,
    PARS_AUSILIARI_STANDBY_SAPONI,
    PARS_AUSILIARI_MACCHINA_OCCUPATA,
    PARS_AUSILIARI_LAMPEGGIANTE,
    PARS_AUSILIARI_H2O_RECUPERO,
    PARS_AUSILIARI_POMPA_RICIRCOLO,
    PARS_AUSILIARI_SCARICO_RECUPERO,
    PARS_AUSILIARI_RISCALDAMENTO_INDIRETTO,
} pars_ausiliari_t;

extern const char *pars_ausiliari[8][5];
typedef enum {
    PARS_DESCRIPTIONS_PARAMETRO = 0,
    PARS_DESCRIPTIONS_LINGUA,
    PARS_DESCRIPTIONS_LOGO,
    PARS_DESCRIPTIONS_NODO_MACCHINA,
    PARS_DESCRIPTIONS_LIVELLO_ACCESSO,
    PARS_DESCRIPTIONS_TEMPO_TASTO_PAUSA,
    PARS_DESCRIPTIONS_TEMPO_TASTO_STOP,
    PARS_DESCRIPTIONS_TEMPO_OUT_PAGINE,
    PARS_DESCRIPTIONS_DURATA,
    PARS_DESCRIPTIONS_TEMPO_ATTIVO,
    PARS_DESCRIPTIONS_MOTO_IN_RIEMPIMENTO,
    PARS_DESCRIPTIONS_VELOCITA_RIEMPIMENTO,
    PARS_DESCRIPTIONS_INVERSIONE_RIEMPIMENTO,
    PARS_DESCRIPTIONS_TEMPO_MOTO_RIEMPIMENTO,
    PARS_DESCRIPTIONS_TEMPO_PAUSA_RIEMPIMENTO,
    PARS_DESCRIPTIONS_VELOCITA_LAVAGGIO,
    PARS_DESCRIPTIONS_INVERSIONE_LAVAGGIO,
    PARS_DESCRIPTIONS_GIRO_LAVAGGIO,
    PARS_DESCRIPTIONS_PAUSA_LAVAGGIO,
    PARS_DESCRIPTIONS_RISCALDAMENTO,
    PARS_DESCRIPTIONS_TEMPERATURA,
    PARS_DESCRIPTIONS_TIPO_RISCALDAMENTO,
    PARS_DESCRIPTIONS_TIPO_GETTONIERA,
    PARS_DESCRIPTIONS_VALORE_IMPULSO,
    PARS_DESCRIPTIONS_VALORE_PREZZO_UNICO,
    PARS_DESCRIPTIONS_PREZZO_UNICO,
    PARS_DESCRIPTIONS_CIFRE_PREZZO,
    PARS_DESCRIPTIONS_CIFRE_DECIMALI_PREZZO,
    PARS_DESCRIPTIONS_VISUALIZZAZIONE_PREZZO,
    PARS_DESCRIPTIONS_RICHIESTA_PAGAMENTO,
    PARS_DESCRIPTIONS_SBLOCCO_GETTONIERA,
    PARS_DESCRIPTIONS_INIBIZIONE_ALLARMI,
    PARS_DESCRIPTIONS_DIAMETRO_CESTO,
    PARS_DESCRIPTIONS_PROFONDITA_CESTO,
    PARS_DESCRIPTIONS_ALTEZZA_TRAPPOLA,
    PARS_DESCRIPTIONS_SENSORE_PROSSIMITA,
    PARS_DESCRIPTIONS_NUMERO_RAGGI,
    PARS_DESCRIPTIONS_CORREZIONE_CONTAGIRI,
    PARS_DESCRIPTIONS_SCALA_ACCELEROMETRO,
    PARS_DESCRIPTIONS_SOGLIA_X_ACCELEROMETRO,
    PARS_DESCRIPTIONS_SOGLIA_Y_ACCELEROMETRO,
    PARS_DESCRIPTIONS_SOGLIA_Z_ACCELEROMETRO,
    PARS_DESCRIPTIONS_SOGLIA_X_ACCELEROMETRO_H,
    PARS_DESCRIPTIONS_SOGLIA_Y_ACCELEROMETRO_H,
    PARS_DESCRIPTIONS_SOGLIA_Z_ACCELEROMETRO_H,
    PARS_DESCRIPTIONS_GIRI_ACCELEROMETRO,
    PARS_DESCRIPTIONS_GIRI_ACCELEROMETRO_2,
    PARS_DESCRIPTIONS_DELTA_ACCELEROMETRO,
    PARS_DESCRIPTIONS_TEMPO_ATTESA_ACCELEROMETRO,
    PARS_DESCRIPTIONS_TEMPERATURA_MASSIMA,
    PARS_DESCRIPTIONS_ISTERESI_TEMPERATURA,
    PARS_DESCRIPTIONS_TEMPERATURA_SICUREZZA,
    PARS_DESCRIPTIONS_TEMPERATURA_TERMODEGRADAZIONE,
    PARS_DESCRIPTIONS_TIPO_LIVELLO,
    PARS_DESCRIPTIONS_TEMPO_ISTERESI_LIVELLO,
    PARS_DESCRIPTIONS_MAX_LIVELLO,
    PARS_DESCRIPTIONS_LIVELLO_SFIORO,
    PARS_DESCRIPTIONS_LIVELLO_MINIMO_SCARICO,
    PARS_DESCRIPTIONS_LIVELLO_MINIMO_RISCALDAMENTO,
    PARS_DESCRIPTIONS_LITRI_MASSIMI,
    PARS_DESCRIPTIONS_LITRI_MINIMI_RISCALDAMENTO,
    PARS_DESCRIPTIONS_IMPULSI_LITRI,
    PARS_DESCRIPTIONS_TIPO_INVERTER,
    PARS_DESCRIPTIONS_VELOCITA_SERVIZIO,
    PARS_DESCRIPTIONS_VELOCITA_MINIMA_LAVAGGIO,
    PARS_DESCRIPTIONS_VELOCITA_MASSIMA_LAVAGGIO,
    PARS_DESCRIPTIONS_PREPARAZIONE_ROTAZIONE,
    PARS_DESCRIPTIONS_TEMPO_MARCIA_PREPARAZIONE_ROTAZIONE,
    PARS_DESCRIPTIONS_TEMPO_SOSTA_PREPARAZIONE_ROTAZIONE,
    PARS_DESCRIPTIONS_VELOCITA_MINIMA_PREPARAZIONE,
    PARS_DESCRIPTIONS_VELOCITA_MASSIMA_PREPARAZIONE,
    PARS_DESCRIPTIONS_VELOCITA_MINIMA_CENTRIFUGA_1,
    PARS_DESCRIPTIONS_VELOCITA_MASSIMA_CENTRIFUGA_1,
    PARS_DESCRIPTIONS_VELOCITA_MINIMA_CENTRIFUGA_2,
    PARS_DESCRIPTIONS_VELOCITA_MASSIMA_CENTRIFUGA_2,
    PARS_DESCRIPTIONS_VELOCITA_MINIMA_CENTRIFUGA_3,
    PARS_DESCRIPTIONS_VELOCITA_MASSIMA_CENTRIFUGA_3,
    PARS_DESCRIPTIONS_TEMPO_MINIMO_RAMPA,
    PARS_DESCRIPTIONS_TEMPO_MASSIMO_RAMPA,
    PARS_DESCRIPTIONS_NUMERO_MAX_SBILANCIAMENTI,
    PARS_DESCRIPTIONS_MIN_SEC,
    PARS_DESCRIPTIONS_TIPO_SERRATURA,
    PARS_DESCRIPTIONS_DURATA_IMPULSO_SERRATURA,
    PARS_DESCRIPTIONS_CONTROLLO_TEMPERATURA_CONTINUO,
    PARS_DESCRIPTIONS_LIVELLO,
    PARS_DESCRIPTIONS_RICIRCOLO,
    PARS_DESCRIPTIONS_ACQUA_FREDDA,
    PARS_DESCRIPTIONS_ACQUA_CALDA,
    PARS_DESCRIPTIONS_ACQUA_DEPURATA,
    PARS_DESCRIPTIONS_SAPONE,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_1,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_1,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_2,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_2,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_3,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_3,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_4,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_4,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_5,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_5,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_6,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_6,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_7,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_7,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_8,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_8,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_9,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_9,
    PARS_DESCRIPTIONS_TEMPO_SAPONE_10,
    PARS_DESCRIPTIONS_RITARDO_SAPONE_10,
    PARS_DESCRIPTIONS_MOTO_FERMO,
    PARS_DESCRIPTIONS_RECUPERO,
    PARS_DESCRIPTIONS_TEMPO_PREPARAZIONE,
    PARS_DESCRIPTIONS_VELOCITA_PREPARAZIONE,
    PARS_DESCRIPTIONS_TIPO_SCARICO,
    PARS_DESCRIPTIONS_VELOCITA_CENTRIFUGA_1,
    PARS_DESCRIPTIONS_TEMPO_RAMPA_1,
    PARS_DESCRIPTIONS_VELOCITA_CENTRIFUGA_2,
    PARS_DESCRIPTIONS_TEMPO_RAMPA_2,
    PARS_DESCRIPTIONS_VELOCITA_CENTRIFUGA_3,
    PARS_DESCRIPTIONS_TEMPO_RAMPA_3,
    PARS_DESCRIPTIONS_TEMPO_FRENATA,
    PARS_DESCRIPTIONS_TEMPO_ATTESA,
    PARS_DESCRIPTIONS_TEMPO_AVVISO_ATTESA_ON,
    PARS_DESCRIPTIONS_TEMPO_AVVISO_ATTESA_OFF,
    PARS_DESCRIPTIONS_NUMERO_RAMPE,
    PARS_DESCRIPTIONS_TEMPO_ATTIVO_SAPONE,
    PARS_DESCRIPTIONS_TEMPO_ATTESA_CENTRIFUGA_1,
    PARS_DESCRIPTIONS_TEMPO_ATTESA_CENTRIFUGA_2,
    PARS_DESCRIPTIONS_NUMERO_SAPONI,
    PARS_DESCRIPTIONS_TEMPO_ALLARME_LIVELLO,
    PARS_DESCRIPTIONS_TEMPO_ALLARME_SCARICO,
    PARS_DESCRIPTIONS_TEMPO_RITARDO_MICRO_OBLO,
    PARS_DESCRIPTIONS_TEMPO_PRECARICA,
    PARS_DESCRIPTIONS_TEMPO_PULIZIA_SAPONI,
    PARS_DESCRIPTIONS_TEMPO_CARICO_SAPONI,
    PARS_DESCRIPTIONS_TEMPO_COLPO_SCARICO,
    PARS_DESCRIPTIONS_TEMPO_MINIMO_SCARICO,
    PARS_DESCRIPTIONS_TEMPO_MINIMO_FRENATA,
    PARS_DESCRIPTIONS_SCARICO_RECUPERO,
    PARS_DESCRIPTIONS_PERCENTUALE_LIVELLO_CARICO_RIDOTTO,
    PARS_DESCRIPTIONS_PERCENTUALE_SAPONE_CARICO_RIDOTTO,
    PARS_DESCRIPTIONS_AUTOAVVIO,
    PARS_DESCRIPTIONS_ACCELEROMETRO,
    PARS_DESCRIPTIONS_TEMPO_SCARICO_ACCELEROMETRO,
    PARS_DESCRIPTIONS_TEMPO_ALLARME_TEMPERATURA,
    PARS_DESCRIPTIONS_ESPANSIONE_IO,
    PARS_DESCRIPTIONS_ESCLUSIONE_SAPONE,
    PARS_DESCRIPTIONS_MACCHINA_LIBERA,
    PARS_DESCRIPTIONS_TIPO_MACCHINA_LIBERA,
    PARS_DESCRIPTIONS_TIPO_IN_AUX_1,
    PARS_DESCRIPTIONS_TIPO_OUT_AUX_2,
    PARS_DESCRIPTIONS_TIPO_OUT_AUX_3,
    PARS_DESCRIPTIONS_TIPO_OUT_AUX_4,
    PARS_DESCRIPTIONS_LAVAGGIO_PROGRAMMATO,
    PARS_DESCRIPTIONS_MACCHINA_FERMA,
    PARS_DESCRIPTIONS_MACCHINA_LAVORO,
    PARS_DESCRIPTIONS_MACCHINA_PAUSA,
    PARS_DESCRIPTIONS_MACCHINA_ATTESA,
    PARS_DESCRIPTIONS_MACCHINA_AVVISO,
    PARS_DESCRIPTIONS_MACCHINA_ALLARME,
    PARS_DESCRIPTIONS_INTERFACCIA_START,
    PARS_DESCRIPTIONS_INTERFACCIA_STOP,
    PARS_DESCRIPTIONS_INTERFACCIA_MENU,
    PARS_DESCRIPTIONS_VELOCITA,
    PARS_DESCRIPTIONS_MAX_PROGRAMMI_UTENTE,
    PARS_DESCRIPTIONS_RIPETIZIONE_CICLO,
    PARS_DESCRIPTIONS_INTERFACCIA_SAPONI,
    PARS_DESCRIPTIONS_TEMPO_SCARICO,
} pars_descriptions_t;

extern const char *pars_descriptions[169][5];
typedef enum {
    PARS_NC_NA_NC = 0,
    PARS_NC_NA_NA,
} pars_nc_na_t;

extern const char *pars_nc_na[2][5];
typedef enum {
    PARS_TIPO_SERRATURA_NUOVA_TRE_MICRO_LIVELLO = 0,
    PARS_TIPO_SERRATURA_BOBINA_UN_MICRO_LIVELLO,
    PARS_TIPO_SERRATURA_NUOVA_TRE_MICRO_NO_LIVELLO,
    PARS_TIPO_SERRATURA_BOBINA_UN_MICRO_NO_LIVELLO,
} pars_tipo_serratura_t;

extern const char *pars_tipo_serratura[4][5];
typedef enum {
    PARS_MACCHINA_LIBERA_NON_GESTITA = 0,
    PARS_MACCHINA_LIBERA_COMMUTA_START,
    PARS_MACCHINA_LIBERA_COMMUTA_PAGAMENTO,
} pars_macchina_libera_t;

extern const char *pars_macchina_libera[3][5];
typedef enum {
    PARS_NA_NC_NA = 0,
    PARS_NA_NC_NC,
} pars_na_nc_t;

extern const char *pars_na_nc[2][5];
typedef enum {
    PARS_LINGUE_ITALIANO = 0,
    PARS_LINGUE_INGLESE,
    PARS_LINGUE_SPAGNOLO,
    PARS_LINGUE_FRANCESE,
    PARS_LINGUE_TEDESCO,
} pars_lingue_t;

extern const char *pars_lingue[5][5];
typedef enum {
    PARS_RGB_SPENTO = 0,
    PARS_RGB_BLU,
    PARS_RGB_VERDE,
    PARS_RGB_AZZURRO,
    PARS_RGB_ROSSO,
    PARS_RGB_VIOLA,
    PARS_RGB_GIALLO,
    PARS_RGB_BIANCO,
} pars_rgb_t;

extern const char *pars_rgb[8][5];
typedef enum {
    PARS_ACCELEROMETRO_DISABILITATO = 0,
    PARS_ACCELEROMETRO_UNA_VELOCITA,
    PARS_ACCELEROMETRO_DUE_VELOCITA,
    PARS_ACCELEROMETRO_TRE_VELOCITA,
    PARS_ACCELEROMETRO_SOGLIE_DELTA,
    PARS_ACCELEROMETRO_HI_SOGLIE,
} pars_accelerometro_t;

extern const char *pars_accelerometro[6][5];
typedef enum {
    PARS_TIPO_LIVELLO_CENTIMETRI = 0,
    PARS_TIPO_LIVELLO_UN_CONTALITRI,
    PARS_TIPO_LIVELLO_DUE_CONTALITRI,
} pars_tipo_livello_t;

extern const char *pars_tipo_livello[3][5];
typedef enum {
    PARS_RICHIESTA_PAGAMENTO_INSERIRE_GETTONE = 0,
    PARS_RICHIESTA_PAGAMENTO_INSERIRE_MONETA,
    PARS_RICHIESTA_PAGAMENTO_PAGAMENTO_CASSA,
    PARS_RICHIESTA_PAGAMENTO_PAGAMENTO_IMPORTO,
} pars_richiesta_pagamento_t;

extern const char *pars_richiesta_pagamento[4][5];
typedef enum {
    PARS_GETTONIERA_NESSUNO = 0,
    PARS_GETTONIERA_GETT1_NA,
    PARS_GETTONIERA_GETT1_NC,
    PARS_GETTONIERA_GETT2_NA,
    PARS_GETTONIERA_GETT2_NC,
    PARS_GETTONIERA_GETT_DIG,
    PARS_GETTONIERA_GETT_DIG_SINGOLA,
    PARS_GETTONIERA_CASSA_NA,
    PARS_GETTONIERA_CASSA_NC,
} pars_gettoniera_t;

extern const char *pars_gettoniera[9][5];
typedef enum {
    PARS_LIVELLO_ACCESSO_UTENTE = 0,
    PARS_LIVELLO_ACCESSO_TECNICO,
    PARS_LIVELLO_ACCESSO_DISTRIBUTORE,
    PARS_LIVELLO_ACCESSO_COSTRUTTORE,
} pars_livello_accesso_t;

extern const char *pars_livello_accesso[4][5];
typedef enum {
    PARS_TIPO_PAGAMENTO_NESSUNO = 0,
    PARS_TIPO_PAGAMENTO_PREZZO,
    PARS_TIPO_PAGAMENTO_PREZZO_E_RIMANENTE,
    PARS_TIPO_PAGAMENTO_PREZZO_E_CREDITO,
    PARS_TIPO_PAGAMENTO_RIMANENTE_E_CREDITO,
} pars_tipo_pagamento_t;

extern const char *pars_tipo_pagamento[5][5];
typedef enum {
    PARS_TEMPO_ATTIVO_SUBITO = 0,
    PARS_TEMPO_ATTIVO_DOPO_LIVELLO,
    PARS_TEMPO_ATTIVO_DOPO_TEMPERATURA,
    PARS_TEMPO_ATTIVO_DOPO_LIVELLO_E_TEMPERATURA,
    PARS_TEMPO_ATTIVO_CON_VELOCITA_RIEMPIMENTO,
} pars_tempo_attivo_t;

extern const char *pars_tempo_attivo[5][5];
typedef enum {
    PARS_ABILITAZIONE_DISABILITATO = 0,
    PARS_ABILITAZIONE_ABILITATO,
} pars_abilitazione_t;

extern const char *pars_abilitazione[2][5];
typedef enum {
    PARS_DIRETTO_INDIRETTO_DIRETTO = 0,
    PARS_DIRETTO_INDIRETTO_INDIRETTO,
} pars_diretto_indiretto_t;

extern const char *pars_diretto_indiretto[2][5];
typedef enum {
    PARS_LOGHI_NESSUNO = 0,
    PARS_LOGHI_MS,
    PARS_LOGHI_LAVENDA,
    PARS_LOGHI_ROTONDI,
    PARS_LOGHI_SCHULTHESS,
    PARS_LOGHI_HOOVER,
} pars_loghi_t;

extern const char *pars_loghi[6][5];

#endif
