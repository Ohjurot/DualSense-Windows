#include "IO_TriggerFx.h"

bool __DS5W::setTriggerEffect(unsigned char* ptrTriggerFxBuffer, DS5W::TriggerFX* ptrTriggerFx){
    // No input parameter validation required is done by caller

    // Switch on type
    switch (ptrTriggerFx->type) {
        // Trigger move to poision
        case DS5W_TRIGGER_FXTYPE_POS:

            ptrTriggerFxBuffer[0] = 0x21;
            ptrTriggerFxBuffer[1] = ((DS5W::TriggerFX_Pos*)ptrTriggerFx)->position;

            return true;
            break;

        // Default will return false
        default:
            return false;
            break;
    }

    // Fallback false
    return false;
}
