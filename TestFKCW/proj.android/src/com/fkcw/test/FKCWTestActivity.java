/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.fkcw.test;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import org.cocos2dx.lib.Cocos2dxActivity;
import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.format.Formatter;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.TextView;

@SuppressLint("HandlerLeak")
public class FKCWTestActivity extends Cocos2dxActivity {

	private static TextView s_textView = null;
	private static Boolean s_isCreate = false;
	private static String s_strPhoneRAM;
	private static long mStart = 0;
	public static final int INFO_ID = 23434;
	private boolean isRuning = true;
	Handler handler;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// -----------------------------------
		createMenoryTextView();

		createHandle();
	}

	private void createHandle() {
		handler = new Handler() {

			@Override
			public void handleMessage(Message msg) {
				switch (msg.what) {
				case INFO_ID:
					showMonery();
					break;
				default:
					break;
				}

				super.handleMessage(msg);
			}
		};

		Thread thread = new Thread() {
			@Override
			public void run() {
				try {
					while (isRuning) {
						Thread.sleep(5500);
						String str = "1";
						Message message = new Message();
						message.what = INFO_ID;
						message.obj = str;
						handler.sendMessage(message);
					}
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		};
		thread.start();
	}

	private void createMenoryTextView() {
		ViewGroup vg = (ViewGroup) this.getWindow().getDecorView();
		if (vg != null) {
			s_textView = new TextView(this);
			String str_2 = "内存情况";
			s_textView.setText(str_2);
			vg.addView(s_textView);
			s_isCreate = true;
		}
		
		s_strPhoneRAM = "\n手机总内存: " + getTotalMemory() + ", " + "可用内存: "+ getAvailMemory();
	}

	private String getAvailMemory() {// 获取android当前可用内存大小

		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);

		MemoryInfo mi = new MemoryInfo();

		am.getMemoryInfo(mi);// mi.availMem; 当前系统的可用内存
		return Formatter.formatFileSize(getBaseContext(), mi.availMem);// 将获取的内存大小规格化

	}

	private String getTotalMemory() {

		String str1 = "/proc/meminfo";// 系统内存信息文件
		String str2;
		String[] arrayOfString;
		long initial_memory = 0;
		try {

			FileReader localFileReader = new FileReader(str1);

			BufferedReader localBufferedReader = new BufferedReader(
					localFileReader, 8192);

			str2 = localBufferedReader.readLine();// 读取meminfo第一行，系统总内存大小
			arrayOfString = str2.split("\\s+");

			for (String num : arrayOfString) {

				Log.i(str2, num + "\t");

			}

			initial_memory = Integer.valueOf(arrayOfString[1]).intValue() * 1024;// 获得系统总内存，单位是KB，乘以1024转换为Byte

			localBufferedReader.close();
		} catch (IOException e) {
		}

		return Formatter.formatFileSize(getBaseContext(), initial_memory);// Byte转换为KB或者MB，内存大小规格化
	}

	public static void showMonery() {
		
		long end = System.currentTimeMillis();
		if (end - mStart > 5000) {
			mStart = System.currentTimeMillis();
			Runtime myRun = Runtime.getRuntime();
			String strTotal = myRun.totalMemory() / (1024) + "kb/";
			String strMax = myRun.maxMemory() / (1014) + "kb/";
			String strFree = myRun.freeMemory() / (1014) + "kb";
			String str_2 = "内存占有情况:" + strTotal + strMax + strFree + s_strPhoneRAM;
			
			Log.d("monery", str_2);
			if (s_isCreate) {
				s_textView.setText(str_2);
			}
		}

	}

	static {
		System.loadLibrary("fkcw");
	}
}
