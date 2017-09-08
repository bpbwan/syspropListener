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
	 
	 status_t SysProperty::readFromParcel(Parcel* p) {
		 name = String8((p->readString16()).string());
		 value = String8((p->readString16()).string());
		 mPropId = p->readInt32();
		 return OK;
	 }
	 
	 status_t SysProperty::writeToParcel(Parcel* p) const {
		 p->writeString16(String16(name));
		 p->writeString16(String16(value));
		  p->writeInt32(mPropId);
		 return OK;
	 }
	 
	 }; // namespace android

