package com.example.sysproplistenertest;


import android.app.Activity;
import android.content.Context;
import android.os.BaseSysPropListener;
import android.os.Bundle;
import android.os.RemoteException;
import android.os.SysPropertiesManager;
import android.os.SysProperty;
import android.os.SystemProperties;
import android.util.Log;
import android.view.View;
import android.widget.Button;



public class MainActivity extends Activity {
	SysPropertiesManager m;	
	static final int PROPID_1 = 100;
	static final int PROPID_2 = 101;
	static final int PROPID_3 = 102;
	Button button1;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        m =  (SysPropertiesManager)getSystemService(Context.PROPERTY_SERVICE);
        if (m == null) {
        	Log.d("bpb", "can not find SysPropertiesManager!");
        } else {
        	m.startPropListener();
        	try {
    			m.registeredPropListener(mMyPropListener);

    			mMyPropListener.addProperty("sss.sss.sss1", PROPID_1);
    			mMyPropListener.addProperty("sss.sss.sss2", PROPID_2);
    			mMyPropListener.addProperty("sss.sss.sss3", PROPID_3);

    		} catch (RemoteException e) {
    			// TODO Auto-generated catch block
    			e.printStackTrace();
    		}
        }
        
        button1 = (Button)findViewById(R.id.button1);
        button1.setOnClickListener(new View.OnClickListener() {
		
			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				Log.d("bpb", "onClick  ");
				SystemProperties.set("xxx.xxx.xxx1", "xxx1");
				SystemProperties.set("sss.sss.sss2", "sss2");
				SystemProperties.set("xxx.xxx.xxx1", "xxx1");
				SystemProperties.set("xxx.xxx.xxx1", "xxx1");
				SystemProperties.set("sss.sss.sss1", "sss1");
			}
		});
    }

    MyPropListener mMyPropListener = new MyPropListener();
    class MyPropListener extends BaseSysPropListener {
    	@Override
    	public void onPropertiesChanged(SysProperty props) {
    		super.onPropertiesChanged(props);

    		Log.d("bpb", "MainActivity onPropertiesChanged name:  "+props.name+" value:  "+props.value+" "+props.mPropId);
    		
    		switch(props.mPropId) {
    		case PROPID_1:

    			break;
    		case PROPID_2:
    			
    			break;
    		case PROPID_3:
    			
    			break;
    		}
    	}
    }
    
    @Override
    protected void onDestroy() {
    	// TODO Auto-generated method stub
    	super.onDestroy();
    	try {
			m.unRegisteredPropListener(mMyPropListener);
		} catch (RemoteException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }
}
