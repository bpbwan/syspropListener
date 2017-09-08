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
#include <syspropservice/ISysPropertiesListener.h>
#include <binder/Parcel.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <syspropservice/SyspropService.h>


namespace android {

class BpSysPropertiesListener : public BpInterface<ISysPropertiesListener>
{
public:
    BpSysPropertiesListener(const sp<IBinder>& impl)
        : BpInterface<ISysPropertiesListener>(impl)
    {
    }
	
	struct  ListeningProps	gListenProps;
	
	 Mutex mPropsLock;
	

	 struct ListeningProps  getListenProps() {

		
		Parcel data, reply;
        data.writeInterfaceToken(ISysPropertiesListener::getInterfaceDescriptor());
        remote()->transact(TRANSACT_GETPROPERTIESLIST, data, &reply);
		reply.readExceptionCode();
		reply.readInt32();
		struct ListeningProps val;
		val.mSize = reply.readInt32();
		//ALOGW("bpb getListenProps 0 :exc %d retCode: %d propsSize: %d", exc, retCode, val.mSize);

		 for ( int i = 0; i <val.mSize; i++) {
		 	struct PropsItem item;
			item.mProps = String8((reply.readString16()).string());
			item.mPropsId = reply.readInt32();
			
			val.mProps.add(item);
		 } 
		

		//val.propReadFromParcel(&reply);

		return val;
	 }

	
    void onPropertiesChanged(struct SysProperty props)
    {
        Parcel data, reply;
        data.writeInterfaceToken(ISysPropertiesListener::getInterfaceDescriptor());
        data.writeInt32(1);
        props.writeToParcel(&data);
        remote()->transact(TRANSACT_SYSPROPERTIESCHANGED, data, NULL);
    }
};

IMPLEMENT_META_INTERFACE(SysPropertiesListener, "android.os.ISysPropertiesListener");

// ----------------------------------------------------------------------------

}; // namespace android
