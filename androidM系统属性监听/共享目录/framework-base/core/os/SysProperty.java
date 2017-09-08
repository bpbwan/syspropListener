/* Copyright 2013, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

package android.os;


public class SysProperty implements Parcelable {

    public String name;
	public String value;
	public int mPropId;
    public SysProperty() {
    }

    public void set(SysProperty other) {
        name = other.name;
        value = other.value;
		mPropId = other.mPropId;
    }

    /*
     * Parcel read/write code must be kept in sync with
     * frameworks/native/services/batteryservice/BatteryProperties.cpp
     */

    private SysProperty(Parcel p) {

        name = p.readString();
		value = p.readString();
		mPropId = p.readInt();
    }
	
    public void readFromParcel(Parcel p) {
        name = p.readString();
		value = p.readString();
		mPropId = p.readInt();
    }

    public void writeToParcel(Parcel p, int flags) {
        p.writeString(name);
		p.writeString(value);
		p.writeInt(mPropId);
    }

    public static final Parcelable.Creator<SysProperty> CREATOR
        = new Parcelable.Creator<SysProperty>() {
        public SysProperty createFromParcel(Parcel p) {
            return new SysProperty(p);
        }

        public SysProperty[] newArray(int size) {
            return new SysProperty[size];
        }
    };

    public int describeContents() {
        return 0;
    }
}
