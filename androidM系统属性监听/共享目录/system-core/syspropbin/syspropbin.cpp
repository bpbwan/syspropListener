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

#define LOG_TAG "bpbsyspropbin"
#define KLOG_LEVEL 6

#include "syspropbin.h"
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <syspropservice/SyspropService.h>
#include <cutils/klog.h>
#include <cutils/uevent.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <utils/Errors.h>
#include<cutils/properties.h>


using namespace android;

// Periodic chores intervals in seconds
#define DEFAULT_PERIODIC_CHORES_INTERVAL_FAST (60 * 1)
#define DEFAULT_PERIODIC_CHORES_INTERVAL_SLOW (60 * 10)



static int eventct;
static int epollfd;

#define POWER_SUPPLY_SUBSYSTEM "power_supply"

// epoll_create() parameter is actually unused
#define MAX_EPOLL_EVENTS 40

// -1 for no epoll timeout
static int awake_poll_interval = -1;


struct healthd_mode_ops *healthd_mode_ops;

// Android mode

extern void healthd_mode_android_init(void);
extern int healthd_mode_android_preparetowait(void);
extern void healthd_mode_android_battery_update(
    struct android::SysProperty *props);


static struct healthd_mode_ops android_ops = {
    .init = healthd_mode_android_init,
    .preparetowait = healthd_mode_android_preparetowait,
    .heartbeat = NULL,
    .battery_update = healthd_mode_android_battery_update,
};



int healthd_register_event(int fd, void (*handler)(uint32_t)) {
    struct epoll_event ev;
//EPOLLWAKEUP
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = (void *)handler;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        KLOG_ERROR(LOG_TAG,
                   "epoll_ctl failed; errno=%d\n", errno);
	
        return -1;
    }

    eventct++;
    return 0;
}




void healthd_battery_update(void) {
    healthd_mode_ops->battery_update(NULL);
}

void healthd_dump_battery_state(int fd){ 
	printf("healthd_dump_battery_state %d \n", (fd));

}



static void periodic_chores() {
    healthd_battery_update();
}





static void healthd_mainloop(void) {
    while (1) {
        struct epoll_event events[eventct];
        int nevents;
        int timeout = awake_poll_interval;
        int mode_timeout;

        healthd_mode_ops->preparetowait();
        nevents = epoll_wait(epollfd, events, eventct, -1);
	

        if (nevents == -1) {
            if (errno == EINTR)
                continue;
            KLOG_ERROR(LOG_TAG, "healthd_mainloop: epoll_wait failed\n");
            break;
        }

        for (int n = 0; n < nevents; ++n) {
            if (events[n].data.ptr)
                (*(void (*)(int))events[n].data.ptr)(events[n].events);
        }

    
    }

    return;
}

static int healthd_init() {
    epollfd = epoll_create(MAX_EPOLL_EVENTS);
    if (epollfd == -1) {
        KLOG_ERROR(LOG_TAG,
                   "epoll_create failed; errno=%d\n",
                   errno);
        return -1;
    }

    healthd_mode_ops->init();
    return 0;
}

int main(int argc, char **argv) {
    int ch;
    int ret;

    klog_set_level(KLOG_LEVEL);
    healthd_mode_ops = &android_ops;
	printf(" sysprop start ! argc %d : %s\n", argc,argv[0]);


    ret = healthd_init();
    if (ret) {
        KLOG_ERROR("Initialization failed, exiting\n");
        exit(2);
    }

    healthd_mainloop();
    KLOG_ERROR("Main loop terminated, exiting\n");
    return 3;
}
