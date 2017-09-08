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

#define LOG_TAG "syspropbin-android"

#include "syspropbin.h"
#include "SysPropertiesRegistrar.h"
#include <syspropservice/SyspropService.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <cutils/klog.h>
#include <sys/epoll.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<cutils/properties.h>

using namespace android;

static int gBinderFd;
static sp<SysPropertiesRegistrar> gBatteryPropertiesRegistrar;
static int gFifoFd;
Mutex mPropReadLock;

void healthd_mode_android_battery_update(
    struct android::SysProperty *props) {
    if (gBatteryPropertiesRegistrar != NULL)
        gBatteryPropertiesRegistrar->notifyListeners(*props);
	//printf("bpb healthd_mode_android_battery_update\n");
    return;
}

int healthd_mode_android_preparetowait(void) {
    IPCThreadState::self()->flushCommands();
	//printf("bpb healthd_mode_android_preparetowait\n");
    return -1;
}

static void binder_event(uint32_t /*epevents*/) {
    IPCThreadState::self()->handlePolledCommands();

}

int findChar(char *c, char r, int pos) {

	int ret = pos;
	int allSize = strlen(c);

	for (; ret < allSize; ret++) {
			if ( c[ret] == r ) {
			return ret;
		}
	}
	
	return -1;
}

void readSomeChar(int fd, char *getBuf,  int getLen) {
	int getSize = 0;
	while (getSize != getLen ) {
		int m =	read(fd, getBuf, getLen - getSize);  
		if ( m < 0)
			break;
		
		getSize += m;
	}

}

static void sysprop_change_event(uint32_t /*epevents*/) {
	
	Mutex::Autolock _l(mPropReadLock);


	if(gFifoFd >0) {
 	char r_buf[100];
	readSomeChar(gFifoFd, r_buf, 2 );
	int propSize = (r_buf[0] - '0') * 10 + (r_buf[1] - '0') + 1;
	//printf("propSize [%d] \n", propSize ); 
	memset(r_buf, 0, 100);
	readSomeChar(gFifoFd, r_buf, propSize );


	//printf("prop [%s][%d] \n", r_buf, strlen(r_buf)); 


	int findPos = findChar(r_buf, ':', 0);
	//printf("findPos %d \n", findPos); 
	if (findPos >= 0)  {
		int   len = strlen(r_buf);
		char name[findPos + 1];
		char value[len - findPos];
		memcpy(name, r_buf, findPos);
		memcpy(value, r_buf + findPos + 1, len - findPos - 1);
		value[len - findPos - 1] = '\0';
		name[findPos] = '\0';
		
		printf("name [%s]  [%s] \n", name, value); 
		struct android::SysProperty prop =  {String8(name),String8(value), -1000};
		healthd_mode_android_battery_update(&prop);
	}
	}
}





void  clearFiFo() {
	char r_buf[4096];
	if(gFifoFd >0) {
    int r_num=read(gFifoFd,r_buf,4096);  
    printf("clearFiFo is %d \n",r_num);  
	}
}

void healthd_mode_android_init(void) {
    ProcessState::self()->setThreadPoolMaxThreadCount(0);
    IPCThreadState::self()->disableBackgroundScheduling(true);
    IPCThreadState::self()->setupPolling(&gBinderFd);

    if (gBinderFd >= 0) {
        if (healthd_register_event(gBinderFd, binder_event))
            KLOG_ERROR(LOG_TAG,
                       "Register for binder events failed\n");
		printf("gBinderFd %d \n", gBinderFd);

    }
	

	
	gFifoFd =  open("/data/myfifo", O_RDONLY|O_NONBLOCK);
	printf("open myfifo %d\n", gFifoFd);
	if (gFifoFd < 0) {
		mkfifo("/data/myfifo",0777);
		gFifoFd =  open("/data/myfifo", O_RDONLY|O_NONBLOCK);
		  printf("open myfifo %d\n", gFifoFd);
		}
	if ( gFifoFd > 0) {
		if (healthd_register_event(gFifoFd, sysprop_change_event))
		printf("Register for gFifoFd events failed\n");
		}

	clearFiFo();
	gBatteryPropertiesRegistrar = new SysPropertiesRegistrar();
    gBatteryPropertiesRegistrar->publish();
}
