/**
 * @file ovln.h
 * @brief Overlay notifications (ovln) service IPC wrapper.
 * @author crc-32
 */
#pragma once
#include "../types.h"
#include "../kernel/event.h"

typedef enum {
	BatteryNotifType = 0xb90b,
	VolumeNotifType  = 0xe803,
    ScreenshotNotifType = 0x441f,
    ScreenshotUnkNotifType = 0x411f,
    VideoNotifType = 0xa61f
} IReceiverNotifType;

typedef struct {
	u16 type;
	u16 content;
} IReceiverNotification;

Result ovlnInitialize();
void ovlnExit();

Result ovlnIReceiverGetNotification(IReceiverNotification *out);
Result ovlnIReceiverGetEvent(Event *out);