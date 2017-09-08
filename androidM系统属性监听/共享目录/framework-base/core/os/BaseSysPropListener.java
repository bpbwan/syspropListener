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

import android.os.SysProperty;
import android.os.ISysPropertiesListener;
import android.os.ListeningProps;
import android.util.Log;



public class BaseSysPropListener extends ISysPropertiesListener.Stub {

	//for save listening sysprops
	private ListeningProps mListeningProps= new ListeningProps();
	
	@Override
	public void onPropertiesChanged(SysProperty props) {

		}
	
	@Override
	public ListeningProps  getListenProps() {
			return mListeningProps;
		}

	/**
	*add prop for listen
	*/
	public void addProperty(String prop, int mId) {
		mListeningProps.addProps(prop, mId);

	  }

    public void removeProperty(String prop){
		mListeningProps.remove(prop);

     }

	public void removeProperty(int id){
		mListeningProps.remove(id);

     }
}
