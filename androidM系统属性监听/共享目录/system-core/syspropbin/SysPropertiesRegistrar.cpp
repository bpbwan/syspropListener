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
#define LOG_TAG "syspropbin"

#include "SysPropertiesRegistrar.h"
#include <syspropservice/SyspropService.h>
#include <syspropservice/ISysPropertiesListener.h>
#include <syspropservice/ISysPropertiesRegistrar.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/PermissionCache.h>
#include <private/android_filesystem_config.h>
#include <utils/Errors.h>
#include <utils/Mutex.h>
#include <utils/String16.h>
#include <cutils/klog.h>
#include<stdio.h>
#include "syspropbin.h"

namespace android {

void SysPropertiesRegistrar::publish() {
    defaultServiceManager()->addService(String16("syspropserver"), this);
}

void SysPropertiesRegistrar::notifyListeners(struct SysProperty props) {
  //  Mutex::Autolock _l(mRegistrationLock);

	
	const char *mBuf = props.name.string();
	size_t const cmpLen = strlen(mBuf);
    for (size_t i = 0; i < mListeners.size(); i++) {

		struct ListeningProps data = mListeners[i]->getListenProps();

		for (int j = 0; j < data.mSize; j++) {
				struct PropsItem val = data.mProps[j];
				
				
			//	printf("onPropertiesChanged: %d  %d || [%s] ? = [%s]\n", cmpLen,  strlen(val.mProps.string()), mBuf, val.mProps.string());
				if ( memcmp(mBuf, val.mProps.string(), cmpLen)  == 0)
				{
					props.mPropId = val.mPropsId;
					mListeners[i]->onPropertiesChanged(props);
					break;
				}
		}
		
	
    }
	 
	 
}

void SysPropertiesRegistrar::registerListener(const sp<ISysPropertiesListener>& listener) {
    {
		
        if (listener == NULL)
            return;
        Mutex::Autolock _l(mRegistrationLock);
        // check whether this is a duplicate
	
        for (size_t i = 0; i < mListeners.size(); i++) {
            if (IInterface::asBinder(mListeners[i]) == IInterface::asBinder(listener)) {
                return;
            }
        }
	
        mListeners.add(listener);
        IInterface::asBinder(listener)->linkToDeath(this);
		printf("registerListener size :%d \n", mListeners.size());
    }
   
}

void SysPropertiesRegistrar::unregisterListener(const sp<ISysPropertiesListener>& listener) {
    if (listener == NULL)
        return;
    Mutex::Autolock _l(mRegistrationLock);
    for (size_t i = 0; i < mListeners.size(); i++) {
        if (IInterface::asBinder(mListeners[i]) == IInterface::asBinder(listener)) {
            IInterface::asBinder(mListeners[i])->unlinkToDeath(this);
            mListeners.removeAt(i);
            break;
        }
    }
	printf("unregisterListener size:%d \n", mListeners.size());
}

status_t SysPropertiesRegistrar::getProperty(int id, struct SysProperty *val) {
	 KLOG_ERROR(LOG_TAG, "getProperty %d '%d'\n",id, val);
    return 0;
}

status_t SysPropertiesRegistrar::dump(int fd, const Vector<String16>& /*args*/) {
    IPCThreadState* self = IPCThreadState::self();
    const int pid = self->getCallingPid();
    const int uid = self->getCallingUid();
    if ((uid != AID_SHELL) &&
        !PermissionCache::checkPermission(
                String16("android.permission.DUMP"), pid, uid))
        return PERMISSION_DENIED;
	KLOG_ERROR(LOG_TAG, "dump %d \n",fd);

   // healthd_dump_battery_state(fd);
    return OK;
}

void SysPropertiesRegistrar::binderDied(const wp<IBinder>& who) {
    Mutex::Autolock _l(mRegistrationLock);

    for (size_t i = 0; i < mListeners.size(); i++) {
        if (IInterface::asBinder(mListeners[i]) == who) {
            mListeners.removeAt(i);
            break;
        }
    }
	printf("binderDied size:%d \n", mListeners.size());
}

}  // namespace android
