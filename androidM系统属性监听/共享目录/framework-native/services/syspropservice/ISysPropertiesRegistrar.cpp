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

#define LOG_TAG "ISysPropertiesRegistrar"
//#define LOG_NDEBUG 0
#include <utils/Log.h>

#include <syspropservice/ISysPropertiesListener.h>
#include <syspropservice/ISysPropertiesRegistrar.h>
#include <stdint.h>
#include <sys/types.h>
#include <binder/Parcel.h>

namespace android {

class BpSysPropertiesRegistrar : public BpInterface<ISysPropertiesRegistrar> {
public:
    BpSysPropertiesRegistrar(const sp<IBinder>& impl)
        : BpInterface<ISysPropertiesRegistrar>(impl) {}

        void registerListener(const sp<ISysPropertiesListener>& listener) {
            Parcel data;
            data.writeInterfaceToken(ISysPropertiesRegistrar::getInterfaceDescriptor());
            data.writeStrongBinder(IInterface::asBinder(listener));
            remote()->transact(REGISTER_LISTENER, data, NULL);
        }

        void unregisterListener(const sp<ISysPropertiesListener>& listener) {
            Parcel data;
            data.writeInterfaceToken(ISysPropertiesRegistrar::getInterfaceDescriptor());
            data.writeStrongBinder(IInterface::asBinder(listener));
            remote()->transact(UNREGISTER_LISTENER, data, NULL);
        }

        status_t getProperty(int id, struct SysProperty *val) {
            Parcel data, reply;
            data.writeInterfaceToken(ISysPropertiesRegistrar::getInterfaceDescriptor());
            data.writeInt32(id);
            remote()->transact(GET_PROPERTY, data, &reply);
            int32_t ret = reply.readExceptionCode();
            if (ret != 0) {
                return ret;
            }
            ret = reply.readInt32();
            int parcelpresent = reply.readInt32();
            if (parcelpresent)
                val->readFromParcel(&reply);
                
            return ret;
        }
};

IMPLEMENT_META_INTERFACE(SysPropertiesRegistrar, "android.os.ISysPropertiesRegistrar");

status_t BnSysPropertiesRegistrar::onTransact(uint32_t code,
                                                  const Parcel& data,
                                                  Parcel* reply,
                                                  uint32_t flags)
{
    switch(code) {
        case REGISTER_LISTENER: {
            CHECK_INTERFACE(ISysPropertiesRegistrar, data, reply);
            sp<ISysPropertiesListener> listener =
                interface_cast<ISysPropertiesListener>(data.readStrongBinder());
            registerListener(listener);
            return OK;
        }

        case UNREGISTER_LISTENER: {
            CHECK_INTERFACE(ISysPropertiesRegistrar, data, reply);
            sp<ISysPropertiesListener> listener =
                interface_cast<ISysPropertiesListener>(data.readStrongBinder());
            unregisterListener(listener);
            return OK;
        }

        case GET_PROPERTY: {
            CHECK_INTERFACE(ISysPropertiesRegistrar, data, reply);
            int id = data.readInt32();
            struct SysProperty val;
            status_t result = getProperty(id, &val);
            reply->writeNoException();
            reply->writeInt32(result);
            reply->writeInt32(1);
            val.writeToParcel(reply);
            return OK;
        }
    }
    return BBinder::onTransact(code, data, reply, flags);
};

// ----------------------------------------------------------------------------

}; // namespace android
