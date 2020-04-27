package com.gd.deepsec.nativeencyption;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{
    private Button aseEncBtn;
    private Button aseDecBtn;
    private EditText plainTextET;
    private TextView infoTV;

    private String data;
    private String key = "1122334455667788";
//    private byte[] key = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    private byte[] encData;
    private byte[] decData;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        aseEncBtn = findViewById(R.id.aes_enc_btn);
        aseDecBtn = findViewById(R.id.aes_dec_btn);
        plainTextET = findViewById(R.id.plain_text_et);
        infoTV = findViewById(R.id.info_tv);

        aseEncBtn.setOnClickListener(this);
        aseDecBtn.setOnClickListener(this);

//        NdkJNIUtils.getInstance().helloFromJNI();
    }

    @Override
    public void onClick(View view) {
        switch(view.getId()) {
            case R.id.aes_enc_btn:
                data = plainTextET.getText().toString();
                Log.v("hello", "data:" + data);
                Log.v("hello", "data hex:" + ByteArrayUtils.bytesToHexString(data.getBytes()));
                print("data:" + data);
                print("data hex:" + ByteArrayUtils.bytesToHexString(data.getBytes()));
                encData = NdkJNIUtils.getInstance().aesEncrypt(data.getBytes(), key.getBytes(), data.getBytes().length);
                Log.v("hello", "encData hex:" + ByteArrayUtils.bytesToHexString(encData));
                print("encData hex:" + ByteArrayUtils.bytesToHexString(encData));
                break;
            case R.id.aes_dec_btn:
                decData = NdkJNIUtils.getInstance().aesDecrypt(encData, key.getBytes(), encData.length);
                Log.v("hello", "decData hex:" + ByteArrayUtils.bytesToHexString(decData));
                Log.v("hello", "decData:" + new String(decData));
                print("decData hex:" + ByteArrayUtils.bytesToHexString(decData));
                print("decData:" + new String(decData));
                break;
        }
    }

    public void print(String data) {
        infoTV.append(data + "\n");
    }
}
