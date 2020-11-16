#include "IO_TriggerFx.h"

void __DS5W::setTriggerEffect(unsigned char* ptrTriggerFxBuffer, DS5W::TriggerFX* ptrTriggerFx){
    // No input parameter validation required is done by caller

    // Switch on type
    switch (ptrTriggerFx->type) {
        // Trigger move to poision
        case DS5W_TRIGGER_FXTYPE_POS:
            ptrTriggerFxBuffer[0] = 0x01;
            ptrTriggerFxBuffer[1] = 255 - ((DS5W::TriggerFX_Pos*)ptrTriggerFx)->startPosition;
            ptrTriggerFxBuffer[2] = ((DS5W::TriggerFX_Pos*)ptrTriggerFx)->force;
            ptrTriggerFxBuffer[3] = 0x00;
            break;

        // Trigger clicky
        case DS5W_TRIGGER_FXTYPE_CLICKY:
            ptrTriggerFxBuffer[0] = 0x23;
            ptrTriggerFxBuffer[1] = 0xFF;
            ptrTriggerFxBuffer[3] = ((DS5W::TriggerFX_Clicky*)ptrTriggerFx)->clickyness;
            break;

        // Default do nothing
        default:
            break;
    }
}
