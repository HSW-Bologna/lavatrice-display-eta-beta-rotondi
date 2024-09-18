#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED


#include "model/model.h"
#include "adapters/view/view.h"


void controller_gui_manage(model_t *model);


extern view_protocol_t controller_gui_protocol;


#endif
