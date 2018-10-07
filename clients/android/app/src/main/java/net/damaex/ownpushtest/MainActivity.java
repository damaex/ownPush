package net.damaex.ownpushtest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private static String Host = "192.168.1.57";
    private static String ClientID = "004917653193753";
    private static String Secret = "GFcQb7AMd6fs3TXlhG8tS8GwYYGPKPsx5rPtOZWtBGLj9BWZ7INw31Xe3BZQcCyc";

    private Client pushClient = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        pushClient = new Client(new Client.OwnPushHandler() {
            @Override
            public void onPushReceived(String message) {

            }

            @Override
            public void connectionStatus(boolean connected) {

            }
        }, Host, ClientID, Secret);
    }

    public void btnRunOnClick(View v) {
        Button b = (Button)v;

        if(pushClient.isRunning()) {
            pushClient.stop();
            b.setText(getString(R.string.btnRun));
        } else {
            pushClient.start();
            b.setText(getString(R.string.btnStop));
        }
    }
}
