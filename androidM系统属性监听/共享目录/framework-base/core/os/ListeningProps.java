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

import java.util.ArrayList;
import java.util.List;

import android.os.Parcel;

public class ListeningProps implements Parcelable {

	class PropItem {
		public PropItem(String prop, int id) {
				this.mProp = prop;
				this.mPropId = id;
			}
		
		public String mProp;
		public int mPropId;
	}
	
    private List<PropItem> mData;

    public ListeningProps() {
    	mData = new ArrayList<PropItem>();
    }

	

	public int size() {
			return mData.size();
		}

    public void set(ListeningProps other) {
		mData = other.mData;
    }

	public void addProps(String prop, int id) {
		synchronized (mData) {
			PropItem tmp = new PropItem(prop, id);
				mData.add(tmp);
			}
			
		}

	private PropItem findItem( String prop) {
			List<PropItem> mTmp = new ArrayList<PropItem>();
			mTmp.addAll(mData);

			for (PropItem item : mTmp) {
				if ( prop.equals(item.mProp) ){
					return item;
				}
			}

			return null;
		}

	private PropItem findItem( int id) {
		 List<PropItem> mTmp = new ArrayList<PropItem>();
			mTmp.addAll(mData);

			for (PropItem item : mTmp) {
				if ( id == item.mPropId ){
					return item;
				}
			}

			return null;
		}

	public boolean remove(String prop) {

		PropItem m = findItem(prop);
		
		if ( m == null) {
			
			return false;
		}
		
		synchronized (mData) {
				mData.remove(m);
			}
			return true;
	}

	public boolean remove(int id) {

		PropItem m = findItem(id);
		
		if ( m == null) {
			
			return false;
		}
		
		synchronized (mData) {
				mData.remove(m);
			}
			return true;
	}
	

    private ListeningProps(Parcel p) {
    	
    		int len = p.readInt();
    		for(int i = 0; i < len; i++) {
				
				PropItem m = new PropItem(p.readString(), p.readInt());
    			mData.add( m );
    		}
		
    }

    public void writeToParcel(Parcel p, int flags) {
    	
    		int len = mData.size();
    		p.writeInt(len);
    		for(int i = 0; i < len; i++) { 
    		 p.writeString( mData.get(i).mProp );
			 p.writeInt( mData.get(i).mPropId );
    		}
	  
    }

    public static final Parcelable.Creator<ListeningProps> CREATOR
        = new Parcelable.Creator<ListeningProps>() {
        public ListeningProps createFromParcel(Parcel p) {
            return new ListeningProps(p);
        }

        public ListeningProps[] newArray(int size) {
            return new ListeningProps[size];
        }
    };

    public int describeContents() {
        return 0;
    }
}