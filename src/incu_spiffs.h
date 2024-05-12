#ifndef INCU_SPIFFS_H_
#define INCU_SPIFFS_H_

#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "incu__defs.h"

void incuSpiffsInit();

uint8_t incuSpiffsSaveConfig(IncuConfig*);

uint8_t incuSpiffsReadConfig(IncuConfig*);

#endif /* INCU_SPIFFS_H_ */

