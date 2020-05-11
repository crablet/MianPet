#include "HeartbeatData.h"

HeartbeatData::HeartbeatData()
{
    SetMethod(HERATBEAT);
    SetPayload({ {"id", mianPetId} });
}
