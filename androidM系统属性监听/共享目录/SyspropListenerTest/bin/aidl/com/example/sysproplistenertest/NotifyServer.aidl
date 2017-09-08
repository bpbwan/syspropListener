package com.example.sysproplistenertest;

/**
*如果没找到该import 请在对应sdk 的framework。aidl 加进去parcelable android.os.Messenger;
*/
import android.os.Bundle;
import android.os.Messenger;

interface NotifyServer {

	      void onPropertiesChanged(in SysProperty props);
		Messenger getListenProps();
	}