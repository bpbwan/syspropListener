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

#include <stdint.h>
#include <sys/types.h>
#include <syspropservice/SyspropService.h>
#include <binder/Parcel.h>
#include <utils/Errors.h>
#include <utils/String8.h>
#include <utils/String16.h>

namespace android {

/*
 * Parcel read/write code must be kept in sync with
 * frameworks/base/core/java/android/os/BatteryProperties.java
 */

status_t ListeningProps::propReadFromParcel(Parcel* p) {
    mSize = p->readInt32();
    for ( int i = 0; i < mSize; i++) {
		struct PropsItem val;
		val.mProps = String8((p->readString16()).string());
		val.mPropsId = p->readInt32();
		ALOGW("bpb item.mPropsId %d", val.mPropsId);
    	mProps.add(val);
    	}
    return OK;
}

status_t ListeningProps::propWriteToParcel(Parcel* p) const {
    p->writeInt32(mSize);
  //DO NOTHING
   
    return OK;
}

}; // namespace android
