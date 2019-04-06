package com.example.esp8266_arduinoclient;

import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MainActivity extends AppCompatActivity {

    MqttAndroidClient client;
    TextView subText, txt_Execution;

    static String MQTT_broker = "tcp://io.adafruit.com:1883";
    static String IO_USERNAME = "karinamg";
    static String IO_KEY = "d5165b2f551b4d23a0cc4a25edf1cced";
    String IO_TOPIC_START = "karinamg/feeds/start";
    String IO_TOPIC_STATUS = "karinamg/feeds/commands";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        subText = (TextView)findViewById(R.id.SubTExt);
        txt_Execution = (TextView)findViewById(R.id.txt_execution);

        String clientId = MqttClient.generateClientId();
        client = new MqttAndroidClient(this.getApplicationContext(), MQTT_broker, clientId);

        MqttConnectOptions options = new MqttConnectOptions();
        options.setUserName(IO_USERNAME);
        options.setPassword(IO_KEY.toCharArray());

        try {
            IMqttToken token = client.connect(options);
            token.setActionCallback(new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Toast.makeText(MainActivity.this, "Connected", Toast.LENGTH_LONG).show();
                    setSubscription();
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Toast.makeText(MainActivity.this, "Not connected ", Toast.LENGTH_LONG).show();
                    System.out.print("------------------------" + exception.getMessage());
                    Log.d("Test", exception.getMessage());
                }
            });
        } catch (MqttException e) {
            e.printStackTrace();
        }

        client.setCallback(new MqttCallback() {
            @Override
            public void connectionLost(Throwable cause) {

            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {
                if(topic.startsWith(IO_TOPIC_START)){
                    txt_Execution.setText(new String(message.getPayload()));
                }
                subText.setText(new String(message.getPayload()));
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

            }
        });


    }

    public void pub(View v){
        String topic = IO_TOPIC_START;
        String message = "1,2,3 ... Start";
        try {
            client.publish(topic, message.getBytes(), 0,false);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public void setSubscription(){
        try{
            client.subscribe(IO_TOPIC_STATUS, 0);
        }catch (MqttException e){
            e.printStackTrace();
        }
    }

    public void setExecution(){
        try{
            client.subscribe(IO_TOPIC_START, 0);
        }catch (MqttException e){
            e.printStackTrace();
        }
    }
}
