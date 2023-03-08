#include "globals.hlsli"

#define COLLISION_NOT   0
#define COLLISION_ENTER 1
#define COLLISION_STAY  2
#define COLLISION_EXIT  3

float4 main(float4 position : SV_Position) : SV_TARGET
{
    float4 Out = MAGENTA;
    switch (cbiData1)
    {
        case COLLISION_NOT :
            Out = GREEN;
            break;
        case COLLISION_ENTER:
            Out = BLUE;
            break;
        case COLLISION_STAY:
            Out = RED;
            break;
        case COLLISION_EXIT:
            Out = WHITE;
            break;
        default :
            Out = MAGENTA;
            break;
    }
    
    return Out;
}