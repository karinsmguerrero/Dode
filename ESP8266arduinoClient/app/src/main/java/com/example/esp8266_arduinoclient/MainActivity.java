package com.example.esp8266_arduinoclient;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;

public class MainActivity extends AppCompatActivity {

    MqttAndroidClient client;
    static String MQTT_broker = "tcp://io.adafruit.com:1883";
    static String IO_USERNAME = "karinamg";
    static String IO_KEY = "d5165b2f551b4d23a0cc4a25edf1cced";
    String IO_TOPIC = "karinamg/feeds/test";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


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
    }

    public void pub(View v){
        String topic = IO_TOPIC;
        String message = "hello world";

        try {
            client.publish(topic, message.getBytes(), 0,false);
        } catch (MqttException e) {
            e.printStackTrace();
        }
    }
}
