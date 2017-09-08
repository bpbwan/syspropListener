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

#ifndef ANDROID_SYSPROPSERVICE_H
#define ANDROID_SYSPROPSERVICE_H

#include <binder/Parcel.h>
#include <sys/types.h>
#include <utils/Errors.h>
#include <utils/String8.h>

namespace android {

struct SysProperty {
	String8 name;
	String8 value;
	int mPropId;
    status_t writeToParcel(Parcel* parcel) const;
    status_t readFromParcel(Parcel* parcel);
};


struct PropsItem {
    String8 mProps;
	int mPropsId;
};


struct ListeningProps {
    int mSize;
    Vector <PropsItem> mProps;

    status_t propWriteToParcel(Parcel* parcel) const;
    status_t propReadFromParcel(Parcel* parcel);
 };

}; // namespace android

#endif // ANDROID_BATTERYSERVICE_H
