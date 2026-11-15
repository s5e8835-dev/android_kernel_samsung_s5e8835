// SPDX-License-Identifier: GPL-2.0-only
/*
 *  user_space.c - A simple user space Thermal events notifier
 *
 *  Copyright (C) 2012 Intel Corp
 *  Copyright (C) 2012 Durgadoss R <durgadoss.r@intel.com>
 *
 *  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include <linux/slab.h>
#include <linux/thermal.h>

#include "thermal_core.h"

/**
 * notify_user_space - Notifies user space about thermal events
 * @tz: thermal_zone_device
 * @trip: trip point index
 *
 * This function notifies the user space through UEvents.
 */
static int notify_user_space(struct thermal_zone_device *tz, int trip)
{
	char *thermal_prop[5];
	int i, trip_temp;

	/*
	 * The governor runs for every trip on every poll. Only tell user space
	 * when a trip is crossed or the event was explicit, instead of flooding
	 * it with a uevent per trip per poll.
	 */
	if (tz->notify_event == THERMAL_EVENT_UNSPECIFIED &&
	    tz->last_temperature != THERMAL_TEMP_INVALID &&
	    !tz->ops->get_trip_temp(tz, trip, &trip_temp) &&
	    (tz->last_temperature >= trip_temp) == (tz->temperature >= trip_temp))
		return 0;

	mutex_lock(&tz->lock);
	thermal_prop[0] = kasprintf(GFP_KERNEL, "NAME=%s", tz->type);
	thermal_prop[1] = kasprintf(GFP_KERNEL, "TEMP=%d", tz->temperature);
	thermal_prop[2] = kasprintf(GFP_KERNEL, "TRIP=%d", trip);
	thermal_prop[3] = kasprintf(GFP_KERNEL, "EVENT=%d", tz->notify_event);
	thermal_prop[4] = NULL;
	kobject_uevent_env(&tz->device.kobj, KOBJ_CHANGE, thermal_prop);
	for (i = 0; i < 4; ++i)
		kfree(thermal_prop[i]);
	mutex_unlock(&tz->lock);
	return 0;
}

static struct thermal_governor thermal_gov_user_space = {
	.name		= "user_space",
	.throttle	= notify_user_space,
};
THERMAL_GOVERNOR_DECLARE(thermal_gov_user_space);
