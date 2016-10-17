package com.zego.zegosdkdemo;

import java.io.IOException;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import com.zego.AVRoom;
import com.zego.AVRoomCallback;
import com.zego.AuxData;
import com.zego.RoomUser;
import com.zego.TextMsg;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.method.ScrollingMovementMethod;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemSelectedListener;
import android.content.DialogInterface;


public class ChatRoom extends Activity {
	
	AVRoom avRoom = null;
	String mStrID = "";
	String mStrName = "";
	private Handler mHandler;
	
	final int EVENT_LOGIN = 1;
	final int EVENT_USER_UPDATE = 2;
	final int EVENT_ONRECV_TEXTMSG = 3;
	final int EVENT_DISCONNECTED = 4;
	final int EVENT_SELF_BEGIN_SPEAK = 5;
	final int EVENT_SELF_END_SPEAK = 6;
	final int EVENT_OTHER_BEGIN_SPEAK = 7;
	final int EVENT_OTHER_END_SPEAK = 8;
	final int EVENT_SELF_KEEP_SPEAK = 9;
	final int EVENT_OTHER_KEEP_SPEAK = 10;
	
	final int EVENT_AUX = 11;
	
	final int EVENT_RECORD = 12;
	
	Dialog alertDialog;
	
	private String[] szItem = {"房间1","房间2","房间3","房间4"}; 
    private Spinner spinner;
    private ArrayAdapter<String> adapter;

    private InputStream mPcmFile;
	private boolean mIsAuxEnable = false;
	private boolean mIsMuteEnable = false;

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if ((keyCode == KeyEvent.KEYCODE_BACK)) { 
			avRoom.SetCallback(null);     //类析构的时候需要设置回调为null
			avRoom.LeaveRoom();
			finish(); 
       }
		
		return super.onKeyDown(keyCode, event);
	}

	public void ShowLog(String strLog)
	{
		TextView tv = (TextView)findViewById(R.id.textViewShowMsg);
		String strMsgShow = tv.getText().toString();
		strMsgShow = strMsgShow + "\n事件通知:" + strLog;
		tv.setText(strMsgShow);
	}
	
	public void SetRoomTitle(String strTitle)
	{
		setTitle(strTitle);
	}
	
	@SuppressLint({ "HandlerLeak", "NewApi" })
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.chat_room);
		
		TextView tv = (TextView)findViewById(R.id.textViewShowMsg);
		tv.setMovementMethod(ScrollingMovementMethod.getInstance());
		
    	
		alertDialog = new AlertDialog.Builder(this)
        .setTitle("断线了")
        .setMessage("你已掉线，是否重连？")	                            
        .setPositiveButton("确定", new DialogInterface.OnClickListener() {
        	
            @Override  
            public void onClick(DialogInterface dialog, int which) {
            	avRoom.ReGetInRoom();
            } 
        })
        .setNegativeButton("取消",new DialogInterface.OnClickListener() { 
         @Override  
  
         public void onClick(DialogInterface dialog, int which) {
        	 avRoom.SetCallback(null);     //类析构的时候需要设置回调为null
			 avRoom.LeaveRoom();
			 finish();
         }  
  
     })
        .setIcon(R.drawable.ic_launcher)
        .create();


        AssetManager am = getAssets();
        try {
            mPcmFile = am.open("a.pcm");
        } catch (IOException e) {
            e.printStackTrace();
        }

		mHandler = new Handler()
		{
		    public void handleMessage(Message msg) 
		    {
			    super.handleMessage(msg);
			    int event = msg.getData().getInt("event");
			    switch(event)
			    {
			    case EVENT_LOGIN:
			    {
			    	int nResult = msg.getData().getInt("result");
			    	int nRoomKey = msg.getData().getInt("roomkey");
			    	int nPos = nRoomKey - MainActivity.DEMO_ROOMKEY_BASE + 1;
			    	if(nResult == AVRoomCallback.ERROR_CODE_LOGIN_OK)
			    	{
			        	//avRoom.EnableAux(true);
			        	//avRoom.EnableRecorder(true);

                        Button btnAux = (Button)findViewById(R.id.btnAux);

                        if(mIsAuxEnable)
                        {
                            btnAux.setText("关Aux");
                        }
                        else
                        {
                            btnAux.setText("开Aux");
                        }

						//avRoom.SetAuxVolume(0);

						//mIsAuxEnable = true;
			        	
			    		ShowLog("进入房间" + nPos +"成功");
			    		SetRoomTitle("房间:" + (nRoomKey - MainActivity.DEMO_ROOMKEY_BASE + 1));
			    		
			    		Button btnSpeak = (Button)findViewById(R.id.btnSpeaker);
			    		if(avRoom.GetSpeakerState())
							btnSpeak.setText("关扬声器");
						else
							btnSpeak.setText("开扬声器");
			    		
			    		Button btnMic = (Button)findViewById(R.id.btnMic);
						if(avRoom.GetMicState())
							btnMic.setText("关Mic");
						else
							btnMic.setText("开Mic");
						
						Button btnPause = (Button)findViewById(R.id.btnPause);
						int nAudioState = avRoom.GetAudioState();
						if(nAudioState == 0 || nAudioState == 1)
							btnPause.setText("暂停");
						else if(nAudioState == 2)
							btnPause.setText("恢复");
			    	}
					else
					{
						ShowLog("进入房间" + nPos +"失败");
					}
			    }
			    break;
			    case EVENT_USER_UPDATE:
			    {
			    	String strShowTip = msg.getData().getString("msg");
			    	ShowLog(strShowTip);
			    }
			    break;
			    case EVENT_ONRECV_TEXTMSG:
			    {
			    	String strMsg =  msg.getData().getString("msg");
			    	TextView tv = (TextView)findViewById(R.id.textViewShowMsg);
					String strMsgShow = tv.getText().toString();
					strMsgShow = strMsgShow + strMsg;
					tv.setText(strMsgShow);
			    }
			    break;
			    case EVENT_DISCONNECTED:
			    {
	                alertDialog.show();
			    }
			    break;
			    case EVENT_SELF_BEGIN_SPEAK:
			    {
			    	Toast.makeText(getApplicationContext(), "自己开始说话", Toast.LENGTH_SHORT).show();
			    }
			    break;
			    case EVENT_SELF_KEEP_SPEAK:
			    {
			    	Toast.makeText(getApplicationContext(), "自己正在说话", Toast.LENGTH_SHORT).show();
			    }
			    break;
			    case EVENT_SELF_END_SPEAK:
			    {
			    	Toast.makeText(getApplicationContext(), "自己结束说话", Toast.LENGTH_SHORT).show();
			    }
			    break;
			    case EVENT_OTHER_BEGIN_SPEAK:
			    {
			    	String strID = msg.getData().getString("ID");
			    	String strName = msg.getData().getString("Name");
			    	Toast.makeText(getApplicationContext(), strName + "[" + strID + "]开始说话", Toast.LENGTH_SHORT).show();
			    }
			    break;
			    case EVENT_OTHER_KEEP_SPEAK:
			    {
			    	String strID = msg.getData().getString("ID");
			    	String strName = msg.getData().getString("Name");
			    	Toast.makeText(getApplicationContext(), strName + "[" + strID + "]正在说话", Toast.LENGTH_SHORT).show();
			    }
			    break;
			    case EVENT_OTHER_END_SPEAK:
			    {
			    	String strID = msg.getData().getString("ID");
			    	String strName = msg.getData().getString("Name");
			    	Toast.makeText(getApplicationContext(), strName + "[" + strID + "]结束说话", Toast.LENGTH_SHORT).show();
			    }
			    break;
			    
			    case EVENT_AUX:
			    {
			    	Toast.makeText(getApplicationContext(), "EVENT_AUX", Toast.LENGTH_SHORT).show();
			    }
			    break;
			    case EVENT_RECORD:
			    {
			    	//Toast.makeText(getApplicationContext(), "EVENT_RECORD", Toast.LENGTH_SHORT).show();
			    }
			    break;
			    }
		    }
	   };
	   
	   final AVRoomCallback mAVRoomCallback = new AVRoomCallback(){
			public void OnGetInResult(int nResult, int nRoomKey)
			{
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_LOGIN); //1 joinroom
			     bundle.putInt("result", nResult);
			     bundle.putInt("roomkey", nRoomKey);
			     message.setData(bundle);
			     mHandler.sendMessage(message);
			}
			
			public void OnDisconnected(int nErrorCode)
			{
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_DISCONNECTED); 
			     message.setData(bundle);
			     mHandler.sendMessage(message);
			}

			public void OnSendBroadcastTextMsgResult(int nResult, String strMsg, long nMsgSeq)
			{
			}
			
			//public void OnSendCustomBroadcastMsgResult(int nResult, byte[] szMsg, long nMsgSeq)
			//{
			//}

			public void OnRoomUsersUpdate(RoomUser[] arrNewUsers, RoomUser[] arrLeftUsers)
			{
				int nLenNew = arrNewUsers.length;
				int nLenLeft = arrLeftUsers.length;
				String strShowTip = "";
				String strNewUsers = "";
				String strLeftUsers = "";
				if(nLenNew != 0)
				{
					for(int i = 0; i < nLenNew; i++)
					{
						strNewUsers = strNewUsers + "ID:" + arrNewUsers[i].strID + ", Name:" + arrNewUsers[i].strName;
					}
					strShowTip = strNewUsers + "   进入了房间." + "\n";
				}
				
				if(nLenLeft != 0)
				{
					for(int j = 0; j < nLenLeft; j++)
					{
						strLeftUsers = strLeftUsers + "ID:" + arrLeftUsers[j].strID + ", Name:" + arrLeftUsers[j].strName;
					}
					strShowTip = strShowTip + strLeftUsers + "   离开房间.";
				}
				
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_USER_UPDATE); //2 OnRoomUsersUpdate ,show
			     bundle.putString("msg", strShowTip);
			     message.setData(bundle);
			     mHandler.sendMessage(message);		
			}
			
			public void OnRoomUserUpdateAll(RoomUser [] arrUsers)
			{
			}

			@SuppressLint("SimpleDateFormat")
			public void OnReceiveBroadcastTextMsg(TextMsg textMsg)
			{
				SimpleDateFormat formatterData = new SimpleDateFormat("HH:mm"); 
		    	Date curDateData = new Date(textMsg.SendTime.toMillis(true));
		    	String strDataTime = formatterData.format(curDateData); 
		    	
				String strMsgShow = "\n" + strDataTime + "\n" + textMsg.SendUser.strName + ": " + textMsg.Msg;
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_ONRECV_TEXTMSG); //3 textMsg
			     bundle.putString("msg", strMsgShow);
			     message.setData(bundle);
			     mHandler.sendMessage(message);	
			}
			
			public void OnSelfBeginTalking()
			{
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_SELF_BEGIN_SPEAK);
			     message.setData(bundle);
			     mHandler.sendMessage(message);	
			}
			
			public void OnSelfKeepTalking()
			{
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_SELF_KEEP_SPEAK);
			     message.setData(bundle);
			     mHandler.sendMessage(message);	
			}
			
			public void OnSelfEndTalking()
			{
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_SELF_END_SPEAK);
			     message.setData(bundle);
			     mHandler.sendMessage(message);	
			}
			
			public void OnOthersBeginTalking(RoomUser roomUser)
			{
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_OTHER_BEGIN_SPEAK);
			     bundle.putString("ID", roomUser.strID);
			     bundle.putString("Name", roomUser.strName);
			     message.setData(bundle);
			     mHandler.sendMessage(message);	
			}
			
			public void OnOthersKeepTalking(RoomUser roomUser)
			{
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_OTHER_KEEP_SPEAK);
			     bundle.putString("ID", roomUser.strID);
			     bundle.putString("Name", roomUser.strName);
			     message.setData(bundle);
			     mHandler.sendMessage(message);	
			}
			
			public void OnOthersEndTalking(RoomUser roomUser)
			{
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_OTHER_END_SPEAK);
			     bundle.putString("ID", roomUser.strID);
			     bundle.putString("Name", roomUser.strName);
			     message.setData(bundle);
			     mHandler.sendMessage(message);	
			}
			
			
			public AuxData OnAuxCallback(int nLenData)
			{
                if (!mIsAuxEnable || nLenData <= 0)
                {
                    return null;
                }

                AuxData data = new AuxData();
                data.bufData = new byte[nLenData];
                data.nLenData = 0;

                try{
                    if (mPcmFile != null){
                        data.nLenData = mPcmFile.read(data.bufData);

                        if (nLenData != data.nLenData){
                            mPcmFile.close();
                            AssetManager am = getAssets();
                            mPcmFile = am.open("a.pcm");
                        }
                    }
                }catch (IOException e){
                    e.printStackTrace();
                }
				 
				 data.nBitDepth = 16;
				 data.nNumChannels = 2;
				 data.nSampleRate = 44100;
				 
				 return data;
			}
		    
		    public void OnRecorderCallback(byte buffer[], int bufLength, int sampleRate, int channels, int bitDepth)
			{
				Message message = new Message();
			     Bundle bundle = new Bundle();
			     bundle.putInt("event", EVENT_RECORD);
			     bundle.putInt("sampleRate", sampleRate);
			     bundle.putInt("channels", channels);
			     bundle.putInt("bitDepth", bitDepth);
			     message.setData(bundle);
			     mHandler.sendMessage(message);	
			}
		};
		
		avRoom = new AVRoom();
		avRoom.SetCallback(mAVRoomCallback);

    	
		byte[] signkey = {(byte)0x91,(byte)0x93,(byte)0xcc,(byte)0x66,(byte)0x2a,(byte)0x1c,(byte)0xe,(byte)0xc1,
				(byte)0x35,(byte)0xec,(byte)0x71,(byte)0xfb,(byte)0x7,(byte)0x19,(byte)0x4b,(byte)0x38,
				(byte)0x15,(byte)0xf1,(byte)0x43,(byte)0xf5,(byte)0x7c,(byte)0xd2,(byte)0xb5,(byte)0x9a,
				(byte)0xe3,(byte)0xdd,(byte)0xdb,(byte)0xe0,(byte)0xf1,(byte)0x74,(byte)0x36,(byte)0xd};
		
		avRoom.SetLogLevel(getApplicationContext(), AVRoom.AVROOM_LOG_LEVEL_DEBUG, null);//此处可以传入app存放log的目录，传入null则写在sdcard
		
		Intent intent = getIntent();
		int nAppID = intent.getIntExtra("AppID", 0);
		if(nAppID == 0)
		{
			nAppID = 1;
		}
		else
		{
			signkey = intent.getByteArrayExtra("Sign");
		}
		
		if(avRoom.Init(nAppID, signkey, getApplicationContext()) != 0)
		{
			Toast.makeText(getApplicationContext(), "init失败", Toast.LENGTH_SHORT).show();
			finish();
			return;
		}
		
		String strID = intent.getStringExtra("ID");
		String strName = intent.getStringExtra("Name");
		int nRoomKey = intent.getIntExtra("Roomkey", 1);
		String strTestIP = intent.getStringExtra("IP");
		int nTestPort = intent.getIntExtra("Port", 0);
		if(!strTestIP.isEmpty() && nTestPort != 0)
			avRoom.SetTestServer(strTestIP, nTestPort, true);
		else
			avRoom.SetTestServer("", 0, false);
		RoomUser roomUser = new RoomUser();
		roomUser.strID = strID;
		roomUser.strName = strName;
		setTitle("房间:" + (nRoomKey - MainActivity.DEMO_ROOMKEY_BASE + 1));
		mStrID = strID;
		mStrName = strName;
		//RoomKey 可以随意定义
		avRoom.GetInRoom(nRoomKey, roomUser);	
		
		Button btnSendMsg = (Button)findViewById(R.id.btnSendMsg);
		btnSendMsg.setOnClickListener(new Button.OnClickListener(){

			@SuppressLint({ "NewApi", "SimpleDateFormat" })
			@Override
			public void onClick(View arg0) {
				EditText edMsg = (EditText)findViewById(R.id.editMsg);
				String strMsg = edMsg.getText().toString();
				if(strMsg.isEmpty())
				{
					Toast.makeText(getApplicationContext(), "发送内容为空", Toast.LENGTH_SHORT).show();
					return;
				}
				SimpleDateFormat formatterData = new SimpleDateFormat("HH:mm"); 
		    	Date curDateData = new Date(System.currentTimeMillis());
		    	String strDataTime = formatterData.format(curDateData); 

				edMsg.setText("");
				TextView tv = (TextView)findViewById(R.id.textViewShowMsg);
				String strMsgShow = tv.getText().toString();
				strMsgShow = strMsgShow + "\n" + strDataTime + "\n我: " + strMsg;
				tv.setText(strMsgShow);
				avRoom.SendBroadcastTextMsg(strMsg);
			}		
		});
		
		Button btnClose = (Button)findViewById(R.id.btnClose);
		btnClose.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View arg0) {
				avRoom.SetCallback(null);
				avRoom.LeaveRoom();
				finish();
			}
			
		});
		
		Button btnSpeak = (Button)findViewById(R.id.btnSpeaker);
		btnSpeak.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View arg0) {
				Button btnSpeak = (Button)findViewById(R.id.btnSpeaker);
				boolean bSpeaker = avRoom.GetSpeakerState();
				bSpeaker = !bSpeaker;
				avRoom.EnableSpeaker(bSpeaker);
				if(bSpeaker)
					btnSpeak.setText("关扬声器");
				else
					btnSpeak.setText("开扬声器");
			}
			
		});
		
		Button btnMic = (Button)findViewById(R.id.btnMic);
		btnMic.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View arg0) {
				boolean bMic = avRoom.GetMicState();
				bMic = !bMic;
				avRoom.EnableMic(bMic);
				Button btnMic = (Button)findViewById(R.id.btnMic);
				if(bMic)
					btnMic.setText("关Mic");
				else
					btnMic.setText("开Mic");
			}
			
		});
		
		Button btnPause = (Button)findViewById(R.id.btnPause);
		btnPause.setOnClickListener(new Button.OnClickListener(){
			@Override
			public void onClick(View arg0) {
				int nAudioState = avRoom.GetAudioState();
				if(nAudioState == 0)
					Toast.makeText(getApplicationContext(), "语音未启动", Toast.LENGTH_SHORT).show();
				else if(nAudioState == 1)
				{
					avRoom.PauseAudio();
					Button btnPause = (Button)findViewById(R.id.btnPause);
					btnPause.setText("恢复");
				}
				else if(nAudioState == 2)
				{
					avRoom.ResumeAudio();
					Button btnPause = (Button)findViewById(R.id.btnPause);
					btnPause.setText("暂停");
				}
			}		
		});

        Button btnAux = (Button)findViewById(R.id.btnAux);
        btnAux.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View arg0) {
                Button btnAux = (Button)findViewById(R.id.btnAux);
                avRoom.EnableAux(!mIsAuxEnable);

                if(mIsAuxEnable)
                {
                    btnAux.setText("开Aux");
                }
                else
                {
                    btnAux.setText("关Aux");
                }

                mIsAuxEnable = !mIsAuxEnable;
            }
        });


		Button btnMute = (Button)findViewById(R.id.btnMute);
        btnMute.setOnClickListener(new Button.OnClickListener(){
			@Override
			public void onClick(View arg0) {
				Button btnMute = (Button)findViewById(R.id.btnMute);

                mIsMuteEnable = !mIsMuteEnable;

				avRoom.EnableMute(mIsMuteEnable);

				if(mIsMuteEnable)
				{
					btnMute.setText("关静音");
				}
				else
				{
					btnMute.setText("开静音");
				}
			}
		});


		btnSpeak.setText("关扬声器");
		btnMic.setText("关Mic");
		btnPause.setText("暂停");
		
		Button btnUserList = (Button)findViewById(R.id.btnUserListInfo);
		btnUserList.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View arg0) {
				RoomUser[] userList = avRoom.GetRoomUsers();
				int nLen = userList.length;
				String strUserList = "";
				for(int i = 0; i < nLen; i++)
				{
					strUserList = strUserList + "ID:" + userList[i].strID + ", Name:" + userList[i].strName + "\n"; 
				}
				
				Context context = arg0.getContext();
			    Dialog alertDialog = new AlertDialog.Builder(context). 
                setTitle("成员列表"). 
                setMessage(strUserList). 
                setIcon(R.drawable.ic_launcher). 
                create(); 
		        alertDialog.show();
		        
			}
			
		});
		
		 spinner = (Spinner) findViewById(R.id.spinner1); 
	        adapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,szItem);    
	        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);  	          
	        spinner.setAdapter(adapter);  	          
	        spinner.setOnItemSelectedListener(new SpinnerSelectedListener());
	        int nPos = nRoomKey - MainActivity.DEMO_ROOMKEY_BASE;
	        spinner.setSelection(nPos);          
	        spinner.setVisibility(View.VISIBLE); 
	}
	
    class SpinnerSelectedListener implements OnItemSelectedListener{
  
        public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
                long arg3) {  
        	//进入另外一个房间可以直接调用GetInRoom
			int nRoomKey = MainActivity.DEMO_ROOMKEY_BASE + arg2;
			if(nRoomKey == avRoom.GetRoomKey())
				return;
			RoomUser roomUser = new RoomUser();
			roomUser.strID = mStrID;
			roomUser.strName = mStrName;
			avRoom.GetInRoom(nRoomKey, roomUser);
        }  
  
        public void onNothingSelected(AdapterView<?> arg0) {  
        }  
    }
}
