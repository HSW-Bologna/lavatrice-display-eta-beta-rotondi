const char *pars_tipo_serratura[5][5] = {
    {"Nuova/3/livello", "New/1/level", "Nuevo/3/nivel", "Nouveau/3/niveau", "Neu/3/Stufe", },
    {"Bobina/1/livello", "Reel/1/level", "Bobina/1/nivel", "Bobine/1/niveau", "Spule/1/Stufe", },
    {"Nuova/3/no livello", "New/3/no level", "Nuevo/3/sin nivel", "Nouveau/3/sans niveau", "Neu/3/keine Stufe", },
    {"Bobina/1/no livello", "Reel/1/no level", "Bobina/1/sin nivel", "Bobine/1/niveau", "Spule/1/keine Stufe", },
    {"Doppio impulso", "Double pulse", "Double pulse", "Double pulse", "Double pulse", },
};

const char *pars_accelerometro[6][5] = {
    {"Disabilitato", "Deactivated", "Desactivado", "Désactivé", "Deaktiviert", },
    {"1 velocita'", "1 speed", "1 velocidad", "1 vitesse", "1 Geschwindigkeit", },
    {"2 velocita'", "2 speed", "2 velocidades", "2 vitesses", "2 Geschwindigkeiten", },
    {"3 velocita'", "3 speed", "3 velocidades", "3 vitesses", "3 Geschwindigkeiten", },
    {"soglie delta", "delta thresh.", "Umbrales delta", "Seuils delta", "Delta-Schwellenwerte", },
    {"soglie HI", "HI thresh.", "Umbrales HI", "Seuils HI", "HI-Schwellenwerte", },
};

const char *pars_scala_accelerometro[6][5] = {
    {"0-2g con offset", "0-2g with offset", "0-2g con desviación", "0-2g avec décalage", "0-2g mit offset", },
    {"1-4g con offset", "1-4g with offset", "1-4g con desviación", "1-4g avec décalage", "1-4g mit offset", },
    {"2-8g con offset", "2-8g with offset", "2-8g con desviación", "2-8g avec décalage", "2-8g mit offset", },
    {"3-2g no offset", "3-2g no offset", "3-2g sin desviación", "3-2g sans décalage", "3-2g kein Offset", },
    {"4-4g no offset", "4-4g no offset", "4-4g sin desviación", "4-4g sans décalage", "4-4g kein Offset", },
    {"5-8g no offset", "5-8g no offset", "5-8g sin desviación", "5-8g sans décalage", "5-8g kein Offset", },
};

const char *pars_controllo_step[3][5] = {
    {"Avanti e indietro", "Forward and backwards", "Forward and backwards", "Forward and backwards", "Forward and backwards", },
    {"Solo avanti", "Forward only", "Forward only", "Forward only", "Forward only", },
    {"Disabilitato", "Deactivated", "Desactivado", "Desactive", "Deaktiviert", },
};

const char *pars_loghi[6][5] = {
    {"Nessuno", "None", "Nadie", "Personne", "Niemand", },
    {"MS", "MS", "MS", "MS", "MS", },
    {"Lavenda", "Lavenda", "Lavenda", "Lavenda", "Lavenda", },
    {"Rotondi", "Rotondi", "Rotondi", "Rotondi", "Rotondi", },
    {"Schulthess", "Schulthess", "Schulthess", "Schulthess", "Schulthess", },
    {"Hoover", "Hoover", "Hoover", "Hoover", "Hoover", },
};

const char *pars_diretto_indiretto[2][5] = {
    {"Diretto", "Direct", "Directo", "Direct", "Direkt", },
    {"Indiretto", "Indirect", "Indirecta", "Indirect", "Indirekt", },
};

const char *pars_controllo_lucchetto[3][5] = {
    {"Nessuno", "None", "None", "None", "None", },
    {"Solo apertura", "Open only", "Open only", "Open only", "Open only", },
    {"Apertura/chiusura", "Open/close", "Open/close", "Open/close", "Open/close", },
};

const char *pars_abilitazione[2][5] = {
    {"Disabilitato", "Deactivated", "Desactivado", "Desactive", "Deaktiviert", },
    {"Abilitato", "Activated", "Activado", "Active", "Aktiviert", },
};

const char *pars_tempo_attivo[5][5] = {
    {"Subito", "Immediate", "Inmediatamente", "Immédiatement", "Unmittelbar", },
    {"Dopo il livello", "After the level", "Después del nivel", "Après le niveau", "Nach Füllstand", },
    {"Dopo la temperatura", "After the temperature", "Después de la temperatura", "Après la température", "Nach der Temperatur", },
    {"Dopo liv. e temp.", "After lev. and temp.", "Después de nivel y temperatura", "Après le niveau et la température", "Nach Füllstand und Temperatur", },
    {"Con vel. di riempimento", "With fill speed", "Con velocidad de llenado", "Avec vitesse de remplissage", "Mit Füllgeschwindigkeit", },
};

const char *pars_tipo_pagamento[5][5] = {
    {"Nessuno", "None", "Ninguno", "Aucun", "Keine", },
    {"Prezzo", "Price", "Precio", "Prix", "Preis", },
    {"Prezzo e rimanente", "Price and remaining", "Precio y resto", "Prix et reste", "Preis und Restbetrag", },
    {"Prezzo e credito", "Price and credit", "Precio y crédito", "Prix et crédit", "Preis und Kredit", },
    {"Rimanente e credito", "Remainder and credit", "Resto y crédito", "Reste et crédit", "Restbetrag und Gutschrift", },
};

const char *pars_na_nc[2][5] = {
    {"Normalmente aperto", "Normally open", "Normalmente abierto", "Normalement ouvert", "Normalerweise offen", },
    {"Normalmente chiuso", "Normally closed", "Normalmente cerrado", "Normalement fermé", "Normalerweise geschlossen", },
};

const char *pars_richiesta_pagamento[4][5] = {
    {"Inserire gettone", "Insert token", "Insertar ficha", "Insérer un jeton", "Jeton einwerfen", },
    {"Inserire moneta", "Insert coin", "Insertar moneda", "Insérer une pièce", "Münze einwerfen", },
    {"Pagamento cassa", "Pay at the desk", "Pago en efectivo", "Paiement en espèces", "Barzahlung", },
    {"Pagamento importo", "Pay the required amount", "Importe del pago", "Montant du paiement", "Zahlungsbetrag", },
};

const char *pars_ausiliari[8][5] = {
    {"Sblocco pagamento", "Unlocking payment", "Desbloqueo del pago", "Déblocage du paiement", "Entriegelung der Zahlung", },
    {"Standby saponi", "Soap standby", "Jabón en espera", "Savon en attente", "Seife in Bereitschaft", },
    {"Macchina occupata", "Machine busy", "Máquina ocupada", "Machine occupée", "Maschine besetzt", },
    {"Lampeggiante", "Flashing light", "Luz intermitente", "Lumière clignotante", "Blinkendes Licht", },
    {"H2O recupero", "H2O recycling", "Recirculación de H2O", "Recyclage H2O", "H2O Rückführung", },
    {"Pompa ricircolo", "Recirculation pump", "Bomba de recirculación", "Pompe de recirculation", "Umwälzpumpe", },
    {"Scarico recupero", "Drain recovery", "Recuperación de drenaje", "Récupération des eaux usées", "Rückgewinnung des Abflusses", },
    {"Riscald. indir.", "Indirect heating", "Calefacción indirecta", "Chauffage indirect", "Indirekte Heizung", },
};

const char *pars_tipo_livello[3][5] = {
    {"Centimetri", "Centimeters", "Centímetros", "Centimètres", "Zentimeter", },
    {"1 contalitri", "1 liter counter", "Contador de 1 litro", "Compteur de 1 litre", "1-Liter-Zähler", },
    {"2 contalitri", "2 liter counter", "Contadores de 2 litros", "Compteurs 2 litres", "2-Liter-Zähler", },
};

const char *pars_tipo_inverter[2][5] = {
    {"Avanti/indietro", "Forward/backward", "Avance/retroceso", "Avant/arrière", "Vorwärts/Rückwärts", },
    {"Marcia/direzione", "Run/direction", "Avance/dirección", "Avant/direction", "Vorwärts/Richtung", },
};

const char *pars_descriptions[173][5] = {
    {"Parametro", "Parameter", "Parámetro", "Paramètre", "Parameter", },
    {"Lingua", "Language", "Idioma", "Langue", "SprachE", },
    {"Logo", "Logo", "Logo", "Logo", "Logo", },
    {"Nodo macchina", "Machine node", "Nodo máquina", "Nœud de la machine", "Maschinenknoten", },
    {"Livello di accesso", "Access level", "Nivel de acceso", "Niveau d'acces", "Zugriffsebene", },
    {"Tempo tasto pausa", "Pause button time", "Botón de pausa tiempo", "Durée de la touche pause", "Pause-taste zeit", },
    {"Tempo tasto stop", "Stop button time", "Botón de parada tiempo", "Bouton d'arret temps", "Stop-taste zeit", },
    {"Tempo out pagine", "Out page time", "Tiempo fuera páginas", "Temps de sortie pages", "Time out seiten", },
    {"Durata", "Duration", "Duración", "Durée de l'activité", "Dauer", },
    {"Tempo attivo", "Active time", "Tiempo activo", "Temps actif", "Aktive Zeit", },
    {"Moto in riempimento", "Fillup motion", "Marcha de llenado", "Mode de remplissage", "Füllgeschwindigkeit", },
    {"Velocita' riempimento", "Fillup speed", "Velocidad de llenado", "Vitesse de remplissage", "Geschwindigkeit der Befüllung", },
    {"Inversione riempimento", "Fillup reversal", "Llenado inverso", "Remplissage inversé", "Füllen in umgekehrter Richtung", },
    {"T. moto riempimento", "Fillup motion time", "Tiempo de movimiento de llenado", "Temps de mouvement du remplissage", "Zeit der Füllbewegung", },
    {"T. pausa riempimento", "Fillup pause time", "Tiempo de pausa de llenado", "Temps de pause du remplissage", "Pausenzeit beim Füllen", },
    {"Velocita' lavaggio", "Wash speed", "Velocidad de lavado", "Vitesse de lavage", "Geschwindigkeit des Waschens", },
    {"Inversione lavaggio", "Wash reversal", "Lavado inverso", "Lavage en marche arrière", "Waschen Inversion", },
    {"T. moto lavaggio", "Wash motion time", "Tiempo de movimiento de lavado", "Temps de mouvement du lavage", "Bewegungszeit Waschen", },
    {"T. pausa lavaggio", "Wash pause time", "Tiempo de pausa de lavado", "Temps de pause du lavage", "Waschpausenzeit", },
    {"Riscaldamento", "Heating", "Calefacción", "Chauffage", "Heizung", },
    {"Temperatura", "Temperature", "Temperatura", "Température d'utilisation", "Temperatur", },
    {"Tipo riscaldamento", "Heating type", "Tipo de calefacción", "Type de chauffage", "Art der Heizung", },
    {"Tipo gettoniera", "Payment type", "Tipo de ficha", "Type de jeton", "Token-Typ", },
    {"Valore impulso", "Pulse value", "Valor de impulso", "Valeur de l'impulsion", "Impulswertigkeit", },
    {"Valore prezzo unico", "Absolute price value", "Valor de precio único", "Valeur du prix unique", "Einzelpreiswert", },
    {"Prezzo unico", "Absolute price", "Precio único", "Prix unique", "Einzelner Preis", },
    {"Cifre prezzo", "Price digits", "Dígitos del precio", "Chiffres du prix", "Preis Ziffern", },
    {"Cifre decimali prezzo", "Decimal price digits", "Dígitos decimales del precio", "Chiffres décimaux du prix", "Preis Dezimalziffern", },
    {"Visualizzazione prezzo", "Price display", "Visualización del precio", "Affichage du prix", "Preisanzeige", },
    {"Richiesta pagamento", "Payment request", "Solicitud de pago", "Demande de paiement", "Zahlungsanforderung", },
    {"Sblocco gettoniera", "Coin acc. unlock", "Liberación de fichas", "Libération du jeton", "Token-Freigabe", },
    {"Inibizione allarmi", "Disable alarms", "Inhibición de alarma", "Inhibition de l'alarme", "Alarmverriegelung", },
    {"Diametro cesto", "Basket diameter", "Diámetro de la cesta", "Diamètre du panier", "Korbdurchmesser", },
    {"Profondita' cesto", "Basked depth", "Profundidad de la cesta", "Profondeur du panier", "Tiefe des Korbes", },
    {"Altezza trappola", "Trap height", "Altura de la trampa", "Hauteur de la trappe", "Höhe des Auffangbehälters", },
    {"Sensore prossimita'", "Proximity sensor", "Sensor de proximidad", "Capteur de proximité", "Näherungsschalter", },
    {"Numero raggi", "Radiuses", "Número de haces", "Nombre de faisceaux", "Anzahl der Strahlen", },
    {"Correzione contagiri", "Rounds corrector", "Corrección del tacómetro", "Correction du tachymètre", "Tachometer-Korrektur", },
    {"Scala accelerometro", "Accelerometer scale", "Escala del acelerómetro", "Échelle de l'accéléromètre", "Skala des Beschleunigungsmessers", },
    {"Soglia x acc.", "Acc. x limit", "Umbral x acelerómetro", "Accéléromètre x à seuil", "Schwellenwert x-Beschleunigungsmesser", },
    {"Soglia y acc.", "Acc. y limit", "Umbral y acelerómetro", "Seuil accéléromètre y", "Schwellenwert y-Beschleunigungsmesser", },
    {"Soglia z acc.", "Acc. z limit", "Umbral z acelerómetro", "Seuil z accéléromètre", "Schwellenwert z-Beschleunigungsmesser", },
    {"Soglia x acc. (h)", "Acc. x limit (h)", "Umbral x acelerómetro (h)", "Seuil accéléromètre x (h)", "Schwellenwert x-Beschleunigungsmesser (h)", },
    {"Soglia y acc. (h)", "Acc. y limit (h)", "Umbral y acelerómetro (h)", "Seuil y accéléromètre (h)", "Schwellenwert y-Beschleunigungsmesser (h)", },
    {"Soglia z acc. (h)", "Acc. z limit (h)", "Umbral z acelerómetro (h)", "Seuil accéléromètre z (h)", "Schwellenwert z-Beschleunigungsmesser (h)", },
    {"Soglia accelerometro", "Accelerometer limit", "Umbral acelerómetro", "Seuil accéléromètre", "Schwellenwert Beschleunigungsmesser", },
    {"Soglia accelerometro 2", "Accelerometer limit 2", "Umbral acelerómetro 2", "Seuil accéléromètre 2", "Schwellenwert Beschleunigungsmesser 2", },
    {"Delta accelerometro", "Delta accelerometer", "Delta del acelerómetro", "Accéléromètre delta", "Beschleunigungsmesser delta", },
    {"Tempo attesa acc.", "Acc. wait time", "Tiempo de espera del acelerómetro", "Temps d'attente de l'accéléromètre", "Beschleunigungsmesser Wartezeit", },
    {"Temperatura massima", "Maximum temperature", "Temperatura máxima", "Température maximale", "Höchsttemperatur", },
    {"Isteresi temperatura", "Temperature hysteresis", "Histéresis de temperatura", "Hystérésis de température", "Temperatur-Hysterese", },
    {"Temperatura sicurezza", "Safety temperature", "Temperatura de seguridad", "Température de sécurité", "Sicherheitstemperatur", },
    {"Temperatura termodegradazione", "Thermal degradation temperature", "Temperatura de termodegradación", "Température de thermodégradation", "Temperatur der thermischen Zersetzung", },
    {"Tipo livello", "Level type", "Tipo de nivel", "Type de niveau", "Niveau-Typ", },
    {"Tempo isteresi livello", "Level hysteresis time", "Tiempo de histéresis de nivel", "Temps d'hystérésis du niveau", "Pegel-Hysteresezeit", },
    {"Livello massimo", "Max level", "Nivel máximo", "Niveau maximum", "Maximaler Füllstand", },
    {"Livello sfioro", "Touch level", "Nivel de desbordamiento", "Niveau de débordement", "Überlaufniveau", },
    {"Livello minimo scarico", "Minimum drain level", "Nivel mínimo de drenaje", "Niveau de vidange minimum", "Minimales Abflussniveau", },
    {"Livello minimo riscaldamento", "Minimum heating level", "Nivel mínimo de calefacción", "Niveau minimum de chauffage", "Minimales Heizungsniveau", },
    {"Litri massimi", "Maximum liters", "Litros máximos", "Lits maximums", "Maximale Liter", },
    {"Litri minimi riscaldamento", "Minimum heating liters", "Calentamiento mínimo litros", "Chauffage minimum litres", "Minimale Heizleistung Liter", },
    {"Impulsi litri", "Pulses per liters", "Pulsos litros", "Impulsions litres", "Pulsierende Liter", },
    {"Tipo inverter", "Inverter type", "Tipo de inversor", "Type d'inverseur", "Inverter-Typ", },
    {"Velocita' minima lavaggio", "Minimum wash speed", "Velocidad mínima de lavado", "Vitesse de lavage minimum", "Minimale Waschgeschwindigkeit", },
    {"Velocita' massima lavaggio", "Maximum wash speed", "Velocidad máxima de lavado", "Vitesse de lavage maximale", "Maximale Waschgeschwindigkeit", },
    {"Cicli preparazione", "Preparation cycles", "Ciclos de preparación", "Cycles de préparation", "Aufbereitungszyklen", },
    {"T. marcia prep. rot.", "Run time rot. prep.", "Tiempo de preparación de la rotación", "Temps de fonctionnement de la préparation de la rotation", "Vorbereitungszeit der Rotation", },
    {"T. sosta prep. rot.", "Pause time rot. prep.", "Tiempo de preparación de la rotación", "Temps d'arrêt de la préparation de la rotation", "Verweilzeit für die Vorbereitung der Rotation", },
    {"V. minima prep.", "Minimum prep. speed", "Velocidad mínima de preparación", "Vitesse de préparation minimale", "Min. Vorbereitungsgeschwindigkeit", },
    {"V. massima prep.", "Max prep. speed", "Velocidad máxima de preparación", "Vitesse de préparation maximale", "Maximale Präparationsgeschwindigkeit", },
    {"V. min. centrifuga 1", "Minimum cen. 1 speed", "Velocidad mín. de centrifugado 1", "Vitesse d'essorage minimale 1", "Min. Schleuderdrehzahl 1", },
    {"V. max centrifuga 1", "Maximum cen. 1 speed", "Velocidad máx. de centrifugado 1", "Vitesse d'essorage maximale 1", "Max. Schleuderdrehzahl 1", },
    {"V. min. centrifuga 2", "Minimum cen. 2 speed", "Velocidad mín. de centrifugado 2", "Vitesse d'essorage minimale 2", "Min. Schleuderdrehzahl 2", },
    {"V. max centrifuga 2", "Maximum cen. 2 speed", "Velocidad máx. de centrifugado 2", "Vitesse d'essorage maximale 2", "Max. Schleuderdrehzahl 2", },
    {"V. min. centrifuga 3", "Minimum cen. 3 speed", "Velocidad mín. de centrifugado 3", "Vitesse d'essorage min. 3", "Min. Schleuderdrehzahl 3", },
    {"V. max centrifuga 3", "Maximum cen. 3 speed", "Velocidad máx. de centrifugado 3", "Vitesse d'essorage max. 3", "Max. Schleuderdrehzahl 3", },
    {"T. minimo rampa", "Minium ramp time", "Tiempo mínimo de rampa", "Temps de rampe minimum", "Min. Rampenzeit", },
    {"T. massimo rampa", "Maximum ramp time", "Tiempo de rampa máx.", "Durée maximale de la rampe", "Max. Rampenzeit", },
    {"N. max sbilanciamenti", "N. max sbilanciamenti", "Número máx. de desequilibrio", "Nombre de déséquilibres max.", "Max. Unwuchtzahl", },
    {"Min/sec", "Min/sec", "Min/seg", "Min/sec", "Min/Sek.", },
    {"Tipo serratura", "Lock type", "Tipo de bloqueo", "Type de blocage", "Art der Sperre", },
    {"Durata impulso serratura", "Lock pulse duration", "Duración del impulso de bloqueo", "Durée de l'impulsion de verrouillage", "Dauer des Sperrimpulses", },
    {"Controllo t. continuo", "Continous t. control", "Control de tiempo continuo", "Contrôle du temps continu", "Kontinuierliche Zeitsteuerung", },
    {"Livello", "Level", "Nivel", "Niveau", "Pegel", },
    {"Ricircolo", "Recycling", "Recirculación", "Recirculation", "Umwälzung", },
    {"Acqua fredda", "Cold water", "Agua fría", "Eau froide", "Kaltes Wasser", },
    {"Acqua calda", "Hot water", "Agua caliente", "Eau chaude", "Warmes Wasser", },
    {"Acqua depurata/recupero", "Purified/recovery water", "Agua depurada/recuperada", "Eau purifiée/récupérée", "Gereinigtes/zurückgewonnenes Wasser", },
    {"Sapone", "Detergent", "Jabón", "Savon", "Seife", },
    {"Tempo sapone 1", "Detergent time 1", "Tiempo jabón 1", "Durée du savon 1", "Seife Zeit 1", },
    {"Ritardo sapone 1", "Detergent delay 1", "Tiempo de jabón 1", "Délai de savon 1", "Seife Verzögerung 1", },
    {"Tempo sapone 2", "Detergent time 2", "Tiempo de jabón 2", "Temps de savon 2", "Seife Zeit 2", },
    {"Ritardo sapone 2", "Detergent delay 2", "Retraso del jabón 2", "Délai de savon 2", "Seife Verzögerung 2", },
    {"Tempo sapone 3", "Detergent time 3", "Jabón Tiempo 3", "Temps de savon 3", "Seife Zeit 3", },
    {"Ritardo sapone 3", "Detergent delay 3", "Jabón Retraso 3", "Délai de savon 3", "Seife Verzögerung 3", },
    {"Tempo sapone 4", "Detergent time 4", "Jabón Tiempo 4", "Temps de savon 4", "Seife Zeit 4", },
    {"Ritardo sapone 4", "Detergent delay 4", "Jabón Retraso 4", "Délai de savon 4", "Seife Verzögerung 4", },
    {"Tempo sapone 5", "Detergent time 5", "Tiempo de jabón 5", "Temps de savon 5", "Seife Zeit 5", },
    {"Ritardo sapone 5", "Detergent delay 5", "Jabón Retraso 5", "Délai de savon 5", "Seife Verzögerung 5", },
    {"Tempo sapone 6", "Detergent time 6", "Tiempo de jabón 6", "Temps de savon 6", "Seife Zeit 6", },
    {"Ritardo sapone 6", "Detergent delay 6", "Retraso del jabón 6", "Délai de savon 6", "Seife Verzögerung 6", },
    {"Tempo sapone 7", "Detergent time 7", "Tiempo de jabón 7", "Temps de savon 7", "Seife Zeit 7", },
    {"Ritardo sapone 7", "Detergent delay 7", "Retraso del jabón 7", "Délai de savon 7", "Seife Verzögerung 7", },
    {"Tempo sapone 8", "Detergent time 8", "Tiempo de jabón 8", "Temps de savon 8", "Seife Zeit 8", },
    {"Ritardo sapone 8", "Detergent delay 8", "Retraso del jabón 8", "Délai de savon 8", "Seife Verzögerung 8", },
    {"Tempo sapone 9", "Detergent time 9", "Tiempo de jabón 9", "Temps de savon 9", "Seife Zeit 9", },
    {"Ritardo sapone 9", "Detergent delay 9", "Retraso del jabón 9", "Délai de savon 9", "Seife Verzögerung 9", },
    {"Tempo sapone 10", "Detergent time 10", "Tiempo de jabón 10", "Temps de savon 10", "Seife Zeit 10", },
    {"Ritardo sapone 10", "Detergent delay 10", "Retraso del jabón 10", "Délai de savon 10", "Seife Verzögerung 10", },
    {"Movimento", "Motion", "Movimiento", "Mouvement", "Bewegung", },
    {"Recupero", "Recovery", "Recuperación", "Récupération", "Erholung", },
    {"Tempo preparazione", "Preparation time", "Tiempo de preparación", "Temps de préparation", "Vorbereitungszeit", },
    {"Velocita' preparazione", "Preparation speed", "Velocidad de preparación", "Vitesse de préparation", "Geschwindigkeit der Vorbereitung", },
    {"Tipo scarico", "Drain type", "Tipo de descarga", "Type de décharge", "Art der Entleerung", },
    {"Velocita' cen. 1", "Cen. speed 1", "Velocidad de centrifugado 1", "Vitesse de centrifugation 1", "Zentrifugengeschwindigkeit 1", },
    {"Tempo rampa 1", "Ramp time 1", "Tiempo de rampa 1", "Temps de rampe 1", "Rampenzeit 1", },
    {"Velocita' cen. 2", "Cen. speed 2", "Velocidad de centrifugado 2", "Vitesse de centrifugation 2", "Zentrifugendrehzahl 2", },
    {"Tempo rampa 2", "Ramp time 2", "Tiempo de rampa 2", "Durée de la rampe 2", "Rampenzeit 2", },
    {"Velocita' cen. 3", "Cen. speed 3", "Velocidad centrífuga 3", "Vitesse de centrifugation 3", "Zentrifugaldrehzahl 3", },
    {"Tempo rampa 3", "Ramp time 3", "Tiempo de rampa 3", "Temps de rampe 3", "Rampenzeit 3", },
    {"Tempo frenata", "Stop time", "Tiempo de frenado", "Temps de freinage", "Abbremszeit", },
    {"Tempo attesa", "Wait time", "Tiempo de espera", "Temps d'attente", "Wartezeit", },
    {"Tempo avviso attesa on", "Tempo avviso attesa on", "Tiempo de espera activado", "Temps d'attente activé", "Wartezeit Warnzeit ein", },
    {"Tempo avviso attesa off", "Tempo avviso attesa off", "Tiempo de aviso de espera desactivado", "Temps d'attente et d'avertissement désactivé", "Wartezeit Vorwarnzeit aus", },
    {"Numero rampe", "Ramp number", "Número de rampa", "Nombre de rampes", "Rampennummer", },
    {"Tempo attivo sapone", "Detergent active time", "Tiempo activo de jabón", "Temps de savonnage actif", "Seife aktive Zeit", },
    {"T. attesa cen. 1", "Cen. 1 wait time", "Tiempo de espera centrifugadora 1", "Temps d'attente en centrifugeuse 1", "Zentrifuge Wartezeit 1", },
    {"T. attesa cen. 2", "Cen. 2 wait time", "Tiempo de espera centrifugado 2", "Temps d'attente en centrifugeuse 2", "Zentrifugenwartezeit 2", },
    {"Numero saponi", "Detergent count", "Número de jabón", "Numéro de savon", "Anzahl der Seifen", },
    {"Ritardo allarme livello", "Level alarm delay", "Retardo de alarma de nivel", "Retard de l'alarme de niveau", "Verzögerung Füllstandsalarm", },
    {"Ritardo allarme scarico", "Drain alarm delay", "Retardo de alarma de descarga", "Retard de l'alarme d'échappement", "Verzögerung Entleerungsalarm", },
    {"Ritardo micro oblo", "Door contact delay", "Retardo micro portilla", "Retard de l'alarme du hublot", "Verzögerung Porthole Micro", },
    {"Tempo precarica", "Preload time", "Tiempo de precarga", "Temps de précharge", "Vorladezeit", },
    {"Tempo pulizia saponi", "Soap cleaning time", "Tiempo de limpieza del jabón", "Temps de nettoyage du savon", "Seifenreinigungszeit", },
    {"Tempo carico saponi", "Soap load time", "Tiempo de carga del jabón", "Temps de chargement du savon", "Seifenladezeit", },
    {"Tempo colpo scarico", "Drain opening time", "Tiempo de descarga", "Durée de la course de décharge", "Entleerungshubzeit", },
    {"Tempo minimo scarico", "Minimum drain time", "Tiempo mínimo de descarga", "Temps de déchargement minimum", "Minimale Entladezeit", },
    {"Tempo minimo frenata", "Minimum stop time", "Tiempo mínimo de frenado", "Temps de freinage minimum", "Minimale Abbremszeit", },
    {"Scarico recupero", "Drain recovery", "Recuperación de la descarga", "Récupération de la décharge", "Erholung der Entladung", },
    {"Livello carico ridotto", "Reduced load level", "Nivel de carga reducido", "Niveau de charge réduit", "Reduzierter Füllstand", },
    {"Sapone carico ridotto", "Reduced load detergent", "Jabón de carga reducida", "Savon à charge réduite", "Reduzierte Lastseife", },
    {"Autoavvio", "Autostart", "Autoarranque", "Démarrage automatique", "Auto-Start", },
    {"Accelerometro", "Accelerometer", "Acelerómetro", "Accéléromètre", "Beschleunigungsmesser", },
    {"Tempo scarico accelerometro", "Accelerometer drain time", "Tiempo de descarga del acelerómetro", "Temps de déchargement de l'accéléromètre", "Beschleunigungsmesser-Entladezeit", },
    {"Tempo allarme temperatura", "Temperature alarm time", "Tiempo de alarma de temperatura", "Temps d'alarme de température", "Temperaturalarmzeit", },
    {"Espansione IO", "IO expansion", "Expansión IO", "Extension IO", "IO-Erweiterung", },
    {"Esclusione sapone", "Detergent exclusion", "Exclusión de jabón", "Exclusion du savon", "Ausschluss von Seife", },
    {"Macchina libera", "Machine free", "Máquina libre", "Machine libre", "Freie Maschine", },
    {"Tipo contatto macchina libera", "Machine free signal type", "Máquina tipo libre", "Type de machine libre", "Maschinentyp frei", },
    {"Tipo IN ausiliario 1", "Auxiliary IN type 1", "Auxiliar IN tipo 1", "Entrée auxiliaire type 1", "Auxiliary IN Typ 1", },
    {"Tipo OUT ausiliario 2", "Auxiliary OUT type 2", "Auxiliar OUT tipo 2", "Auxiliaire OUT type 2", "Auxiliary OUT Typ 2", },
    {"Tipo OUT ausiliario 3", "Auxiliary OUT type 3", "Auxiliar OUT tipo 3", "Auxiliaire OUT type 3", "Hilfsausgang Typ 3", },
    {"Tipo OUT ausiliario 4", "Auxiliary OUT type 4", "Salida auxiliar tipo 4", "Sortie auxiliaire type 4", "Hilfsausgang Typ 4", },
    {"Lavaggio programmato", "Scheduled cycle", "Lavado programado", "Lavage programmé", "Programmiertes Waschen", },
    {"LED da fermo", "LED while stopped", "LED en reposo", "LED à l'arrêt", "LED im Stillstand", },
    {"LED al lavoro", "LED while working", "LED en funcionamiento", "LED au travail", "LED bei der Arbeit", },
    {"LED in pausa", "LED in pause", "LED en pausa", "LED en pause", "LED pausiert", },
    {"LED in attesa", "LED waiting", "LED en espera", "LED en attente", "LED im Wartezustand", },
    {"LED con avviso", "LED with notification", "LED con aviso", "LED avec avertissement", "LED mit Warnung", },
    {"LED con allarme", "LED with alarmti", "LED con alarma", "LED avec alarme", "LED mit Alarm", },
    {"Interf. al lavoro", "Work interf.", "Interfaz en funcionamiento", "Interface en fonctionnement", "Schnittstelle in Betrieb", },
    {"Interf. da fermo", "Stopped interf.", "Interfaz en reposo", "Interface à l'arrêt", "Schnittstelle im Leerlauf", },
    {"Interf. menu'", "Menu interface", "Interfaz de menú", "Interface de menu", "Menü-Schnittstelle", },
    {"Velocita'", "Speed", "Velocidad", "Vitesse", "Geschwindigkeit", },
    {"Max programmi utente", "Max user programs", "Programas de usuario máx.", "Programmes utilisateur max.", "Max. Benutzerprogramme", },
    {"Ripetizione ciclo", "Cycle repetition", "Repetición de ciclo", "Répétition du cycle", "Zykluswiederholung", },
    {"Interf. saponi", "Detergent interf.", "Interfaz de jabón", "Interface savon", "Seifenschnittstelle", },
    {"Tempo scarico servizio", "Service drain time", "Tiempo de drenaje de servicio", "Temps de vidange du service", "Service-Ablaufzeit", },
    {"Smart Wave", "Smart Wave", "Smart Wave", "Smart Wave", "Smart Wave", },
    {"Visualizzazione esclusione sapone", "Show detergent exclusion", "Show detergent exclusion", "Show detergent exclusion", "Show detergent exclusion", },
    {"Controllo pausa/stop", "Pause/stop control", "Pause/stop control", "Pause/stop control", "Pause/stop control", },
    {"Controllo lucchetto", "Lock control", "Lock control", "Lock control", "Lock control", },
    {"Controllo step", "Step control", "Step control", "Step control", "Step control", },
};

const char *pars_rgb[8][5] = {
    {"Spento", "Off", "Apagado", "Arrêt", "Aus", },
    {"Blu", "Blue", "Azul", "Bleu", "Blau", },
    {"Verde", "Green", "Verde", "Vert", "Grün", },
    {"Azzurro", "Cyan", "Azul", "azur", "Azurblau", },
    {"Rosso", "Red", "Rojo", "Rouge", "Rot", },
    {"Viola", "Purple", "Morado", "Pourpre", "Lila", },
    {"Giallo", "Yellow", "Amarillo", "Jaune", "Gelb", },
    {"Bianco", "White", "Blanco", "Blanc", "Weiß", },
};

const char *pars_gettoniera[9][5] = {
    {"Nessuno", "None", "Ninguno", "Aucun", "Keine", },
    {"1 gettoniera NA", "1 Coint count. NO", "1 monedero NA", "1 accepteur de pièces NA", "1 NA-Münzprüfer", },
    {"1 gettoniera NC", "1 Coin count. NC", "1 monedero NC", "1 monnayeur NC", "1 NC-Münzprüfer", },
    {"2 gettoniere NA", "2 Coin count. NO", "2 monederos NA", "2 monnayeurs NA", "2 NA-Münzkästen", },
    {"2 gettoniere NC", "2 Coin count. NC", "2 monederos NC", "2 monnayeurs NC", "2 NC-Münzkästen", },
    {"Gettoniera digitale", "Digital coin count.", "Monedero digital", "Monnayeur numérique", "Digitaler Münzprüfer", },
    {"G. dig. a linea singola", "D. c.c. single line", "Monedero digital de una línea", "Accepteur de pièces numérique à ligne unique", "Einzeiliger digitaler Münzprüfer", },
    {"Cassa NA", "Cash Desk NA", "Maletín NA", "Caisse NA", "NA-Koffer", },
    {"Cassa NC", "Cash Desk NC", "Caja NC", "Caisse NC", "NC-Gehäuse", },
};

const char *pars_nc_na[2][5] = {
    {"Normalmente chiuso", "Normally closed", "Normalmente cerrado", "Normalement fermé", "Normalerweise geschlossen", },
    {"Normalmente aperto", "Normally open", "Normalmente abierto", "Normalement ouvert", "Normalerweise offen", },
};

const char *pars_livello_accesso[2][5] = {
    {"Utente", "User", "Usuario", "Utilisateur", "Benutzer", },
    {"Costruttore", "Constructor", "Fabricante", "Fabricant", "Hersteller", },
};

const char *pars_visualizzazione[2][5] = {
    {"Self", "Self", "Auto", "Autonome", "Selbst", },
    {"Laboratorio", "Laboratory", "Taller", "Atelier", "Werkstatt", },
};

const char *pars_lingue[5][5] = {
    {"Italiano", "Italian", "Italiano", "Italien", "Italienisch", },
    {"Inglese", "English", "Inglés", "Anglais", "Englisch", },
    {"Spagnolo", "Spanish", "Español", "Espagnol", "Spanisch", },
    {"Francese", "French", "Francés", "Français", "Französisch", },
    {"Tedesco", "German", "Alemán", "Allemand", "Deutsch", },
};

const char *pars_macchina_libera[3][5] = {
    {"Non gestita", "Not managed", "No gestionado", "Non géré", "Nicht verwaltet", },
    {"Commuta allo start", "Changes on start", "Cambio al inicio", "Commutation au démarrage", "Wechsel beim Start", },
    {"Commuta al pagamento", "Changes on payment", "Cambio en el momento del pago", "Commutation au moment du paiement", "Umschalten bei Zahlung", },
};

