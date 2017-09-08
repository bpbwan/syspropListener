/*
 * Copyright (C) 2008 The Android Open Source Project
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

package android.os;

import android.content.Context;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemProperties;
import android.util.Log;
import android.os.ISysPropertiesRegistrar;
import android.os.ISysPropertiesListener;
import android.os.RemoteException;
import android.os.IBinder;

/**
 */
public class SysPropertiesManager {

    private  ISysPropertiesRegistrar mSysPropertiesRegistrar;

	String TAG = "bpb_SysPropertiesManager";

 
    public SysPropertiesManager() {
 
        mSysPropertiesRegistrar = ISysPropertiesRegistrar.Stub.asInterface(
                ServiceManager.getService("syspropserver"));
		try {
			if ( mSysPropertiesRegistrar != null )
			mSysPropertiesRegistrar.asBinder().linkToDeath(mDeathRecipent, 0);
		} catch (RemoteException e) {
			Log.e(TAG, "SysPropertiesManager lingToDeath err! "+e);

		}
		Log.d(TAG, "SysPropertiesManager create obj: "+mSysPropertiesRegistrar);
		
    }

	
	public void registeredPropListener (ISysPropertiesListener.Stub isl) throws RemoteException{
		Log.d(TAG, "SysPropertiesManager registeredPropListener ===========");

			
			if ( mSysPropertiesRegistrar == null ) {
					if ( !linkAgian()) {
						throw new NullPointerException("registeredPropListener fail !	try registeredPropListener <ISysPropertiesListener> angain.");
					}
			}
			
			mSysPropertiesRegistrar.registerListener(isl);
	
			
		}

	public void unRegisteredPropListener (ISysPropertiesListener.Stub isl)  throws RemoteException {
		Log.d(TAG, "SysPropertiesManager unRegisteredPropListener ===========");
		mSysPropertiesRegistrar.unregisterListener(isl);
	}

	public int getProperty( int id ){
			return 0;
		}

	public void startPropListener() {
		if ( mSysPropertiesRegistrar == null ) {
		
				throw new NullPointerException("syspropserver   null !  try registeredPropListener <ISysPropertiesListener> angain.");

		}
		
		SystemProperties.set("sys.prop.listen", "1");
		Log.d(TAG, "SysPropertiesManager startPropListener ===========");
		
	}

	public void stopPropListener() {
		SystemProperties.set("sys.prop.listen", "0");
		Log.d(TAG, "SysPropertiesManager stopPropListener ===========");

	}



	private boolean linkAgian() throws RemoteException {
			mSysPropertiesRegistrar = ISysPropertiesRegistrar.Stub.asInterface(
						ServiceManager.getService("syspropserver"));
			if ( mSysPropertiesRegistrar != null ) {

				try {
					mSysPropertiesRegistrar.asBinder().linkToDeath(mDeathRecipent, 0);
					return true;
				} catch (RemoteException e) {
						Log.e(TAG, "SysPropertiesManager lingToDeath err! "+e);

				}
				
			}
			
			return false;

		}

	

	private IBinder.DeathRecipient mDeathRecipent = new IBinder.DeathRecipient() {

			@Override
			public void binderDied() {
					if (mSysPropertiesRegistrar != null ) {
						mSysPropertiesRegistrar.asBinder().unlinkToDeath(mDeathRecipent, 0);
					}
					mSysPropertiesRegistrar = null;
				}
		};

 
}
