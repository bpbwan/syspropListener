/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _HEALTHD_H_
#define _HEALTHD_H_

#include <syspropservice/SyspropService.h>
#include <sys/types.h>
#include <utils/Errors.h>
#include <utils/String8.h>



// Global helper functions

int healthd_register_event(int fd, void (*handler)(uint32_t));
void healthd_battery_update();
android::status_t healthd_get_property(int id,
    struct android::SysProperty *val);
void healthd_dump_battery_state(int fd);

struct healthd_mode_ops {
    void (*init)(void);
    int (*preparetowait)(void);
    void (*heartbeat)(void);
    void (*battery_update)(struct android::SysProperty *props);
};

extern struct healthd_mode_ops *healthd_mode_ops;




#endif /* _HEALTHD_H_ */
