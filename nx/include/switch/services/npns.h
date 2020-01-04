/**
 * @file npns.h
 * @brief Nintendo push notification (npns) service IPC wrapper.
 * @author crc-32
 */
#pragma once
#include "../types.h"
#include "../sf/service.h"

Result npnsInitialize();
void npnsExit();