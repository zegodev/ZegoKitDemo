package com.zego.zegosdkdemo;


import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.TextView;

public class Config extends Activity{

	static public final int ONLINE_ENV = 0;
	static public final int TEST_ENV = 1;
	static public final int DEMO_APP = 0;
	static public final int CUSTOM_APP = 1;
	
	private String[] szItem = {"正式环境","开发环境"};
	private String[] szItemSign = {"Demo APPID","自定义APPID"};
    private Spinner spinner;
    private ArrayAdapter<String> adapter;
    
    private Spinner spinnerAPP;
    private ArrayAdapter<String> adapterAPP;
    
    @SuppressLint("NewApi") 
    public void SetResult()
    {
    	int nPos = spinner.getSelectedItemPosition();
    	Intent intent = new Intent();
    	intent.putExtra("Env", nPos);
    	if(nPos == TEST_ENV)
    	{
    		EditText edIP = (EditText)findViewById(R.id.editIP);
        	EditText edPort = (EditText)findViewById(R.id.editPort);
    		String strIP = edIP.getText().toString();
    		String strPort = edPort.getText().toString();
    		if(!strPort.isEmpty())
    		{
    			int nPort = Integer.parseInt(strPort);
    			intent.putExtra("Port", nPort);
    		}
    	    intent.putExtra("IP", strIP);
    	}
    	
    	int nPosApp = spinnerAPP.getSelectedItemPosition();
    	intent.putExtra("EnvAPP", nPosApp);
    	if(nPosApp == CUSTOM_APP)
    	{
    		EditText edAppID = (EditText)findViewById(R.id.editAppID);
        	EditText edSign = (EditText)findViewById(R.id.editSign);
    		String strAppID = edAppID.getText().toString();
    		String strSign = edSign.getText().toString();
    		if(!strAppID.isEmpty())
    		{
    			int nAppID = Integer.parseInt(strAppID);
    			intent.putExtra("AppID", nAppID);
    		}
    		if(!strSign.isEmpty())
    		{
    			byte[] szSign = ConvertStringToSign(strSign);
    			intent.putExtra("Sign", szSign);
    		}
    	}
	    setResult(RESULT_OK, intent);
    }
    
    @Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if ((keyCode == KeyEvent.KEYCODE_BACK)) { 
			SetResult();
			finish(); 
       }
		
		return super.onKeyDown(keyCode, event);
	}
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.config);
		
		Intent intent = getIntent();
		int nEnv = intent.getIntExtra("Env", 0);
		if(nEnv == TEST_ENV)
		{
			String strIP = intent.getStringExtra("IP");
			int nPort = intent.getIntExtra("Port", 9001);
			EditText edIP = (EditText)findViewById(R.id.editIP);
	    	EditText edPort = (EditText)findViewById(R.id.editPort);
	    	edIP.setText(strIP);
	    	edPort.setText(Integer.toString(nPort));
		}
		
		//room list
		spinner = (Spinner) findViewById(R.id.spinnerServer); 
        adapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,szItem); 	        
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);  	          
        spinner.setAdapter(adapter);
        //添加事件Spinner事件监听    
        spinner.setOnItemSelectedListener(new SpinnerSelectedListener());
        spinner.setSelection(nEnv);
        spinner.setVisibility(View.VISIBLE);
        
        int nAPP = intent.getIntExtra("EnvAPP", 0);
        if(nAPP == CUSTOM_APP)
        {
        	int nAPPID = intent.getIntExtra("AppID", 0);
        	byte[] szTestSignkey = intent.getByteArrayExtra("Sign");
        	EditText edAppID = (EditText)findViewById(R.id.editAppID);
        	edAppID.setText(Integer.toString(nAPPID));
        	EditText edAppSign = (EditText)findViewById(R.id.editSign);
        	edAppSign.setText(ConvertSignToString(szTestSignkey));
        }
        spinnerAPP = (Spinner) findViewById(R.id.spinnerSign); 
        adapterAPP = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,szItemSign); 	        
        adapterAPP.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);  	          
        spinnerAPP.setAdapter(adapterAPP);
        //添加事件Spinner事件监听    
        spinnerAPP.setOnItemSelectedListener(new SpinnerSelectedListener());
        spinnerAPP.setSelection(nAPP);
        spinnerAPP.setVisibility(View.VISIBLE);
        
        Button btnOK = (Button)findViewById(R.id.btnOK);
        btnOK.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View arg0) {
				SetResult();
				finish();
			}
			
		});
	}
	
	//使用数组形式操作  
    class SpinnerSelectedListener implements OnItemSelectedListener{
  
        public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
                long arg3) {
        	if(arg0 == spinner)
        		ShowTestServerUI(arg2);
        	else if(arg0 == spinnerAPP)
        		SHowTestAppUI(arg2);
        }  
  
        public void onNothingSelected(AdapterView<?> arg0) {  
        }  
    }
    
    private void ShowTestServerUI(int nEnv)
    {
    	TextView tvIP = (TextView)findViewById(R.id.tvIP);
    	TextView tvPort = (TextView)findViewById(R.id.tvPort);
    	EditText edIP = (EditText)findViewById(R.id.editIP);
    	EditText edPort = (EditText)findViewById(R.id.editPort);
    	if(nEnv == ONLINE_ENV)
    	{
    		tvIP.setVisibility(View.INVISIBLE);
    		tvPort.setVisibility(View.INVISIBLE);
    		edIP.setVisibility(View.INVISIBLE);
    		edPort.setVisibility(View.INVISIBLE);
    	}
    	else if(nEnv == TEST_ENV)
    	{
    		tvIP.setVisibility(View.VISIBLE);
    		tvPort.setVisibility(View.VISIBLE);
    		edIP.setVisibility(View.VISIBLE);
    		edPort.setVisibility(View.VISIBLE);
    	}
    }
    
    private void SHowTestAppUI(int nAPP)
    {
    	TextView tvAppID = (TextView)findViewById(R.id.tvAppID);
    	TextView tvSign = (TextView)findViewById(R.id.tvSign);
    	EditText edAppID = (EditText)findViewById(R.id.editAppID);
    	EditText edSign = (EditText)findViewById(R.id.editSign);
    	if(nAPP == DEMO_APP)
    	{
    		tvAppID.setVisibility(View.INVISIBLE);
    		tvSign.setVisibility(View.INVISIBLE);
    		edAppID.setVisibility(View.INVISIBLE);
    		edSign.setVisibility(View.INVISIBLE);
    	}
    	else if(nAPP == CUSTOM_APP)
    	{
    		tvAppID.setVisibility(View.VISIBLE);
    		tvSign.setVisibility(View.VISIBLE);
    		edAppID.setVisibility(View.VISIBLE);
    		edSign.setVisibility(View.VISIBLE);
    	}
    }
    
    @SuppressLint("DefaultLocale") 
    private String ConvertSignToString(byte[] szSignkey)
    {
    	String strRet = "";
    	if(szSignkey == null)
    		return strRet;
    	int nLen = szSignkey.length;
    	String strTemp = "";
    	for(int i = 0; i < nLen; i++)
    	{
    		strTemp = Integer.toHexString(szSignkey[i]);
    		if(strTemp.length() == 1)
    			strRet = strRet + "0x" + strTemp;
    		else
    			strRet = strRet + "0x" +strTemp.toLowerCase().substring(strTemp.length() - 2, strTemp.length());
    		if(i < (nLen-1))
    			strRet = strRet + ",";
    	}
    	return strRet;
    }
    
    @SuppressLint({ "DefaultLocale", "NewApi" }) 
    private byte[] ConvertStringToSign(String strSign)
    {
    	if(strSign == null || strSign.isEmpty())
    		return null;
    	String strTemp = strSign;
    	strTemp = strTemp.toLowerCase();
    	strTemp = strTemp.replaceAll(" ", "");
    	strTemp = strTemp.replaceAll("0x", "");
    	String[] szStrTemp = strTemp.split(",");
    	int nLen = szStrTemp.length;
    	byte[] szRet = new byte[nLen];
    	for(int i = 0; i < nLen; i++)
    	{
    		if(szStrTemp[i].length() == 1)
    			szRet[i] = (byte) (toByte(szStrTemp[i].charAt(0)));
    		else
    			szRet[i] = (byte) (toByte(szStrTemp[i].charAt(0)) << 4 | toByte(szStrTemp[i].charAt(1)));
    	}
    	return szRet;
    }
    
    private byte toByte(char c) {   
        byte b = (byte)"0123456789abcdef".indexOf(c);   
        return b;   
    }
}
