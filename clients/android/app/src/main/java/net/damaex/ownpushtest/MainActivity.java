package net.damaex.ownpushtest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import net.damaex.ownpush.OwnPush;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        createBackgroundService();
    }

    void createBackgroundService() {
        OwnPush.start();
    }
}
