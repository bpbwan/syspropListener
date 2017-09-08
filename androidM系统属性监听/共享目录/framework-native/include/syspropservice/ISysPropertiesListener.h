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

#ifndef ANDROID_ISYSPROPERTIESLISTENER_H
#define ANDROID_ISYSPROPERTIESLISTENER_H

#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <utils/Mutex.h>
#include <utils/String16.h>
#include <syspropservice/SyspropService.h>

namespace android {

// must be kept in sync with interface defined in IBatteryPropertiesListener.aidl
enum {
        TRANSACT_SYSPROPERTIESCHANGED = IBinder::FIRST_CALL_TRANSACTION,
		TRANSACT_GETPROPERTIESLIST,
};

// ----------------------------------------------------------------------------

class ISysPropertiesListener : public IInterface {
public:
    DECLARE_META_INTERFACE(SysPropertiesListener);
	
    virtual void onPropertiesChanged(struct SysProperty props) = 0;

	virtual struct ListeningProps getListenProps() = 0;
};

// ----------------------------------------------------------------------------

}; // namespace android

#endif // ANDROID_IBATTERYPROPERTIESLISTENER_H
