package net.damaex.ownpush;

import android.content.Context;
import android.support.annotation.NonNull;
import android.util.Log;

import androidx.work.Worker;
import androidx.work.WorkerParameters;

public class OwnPushRunner extends Worker {
    private static String Host = "192.168.1.57";
    private static String ClientID = "004917653193753";
    private static String Secret = "GFcQb7AMd6fs3TXlhG8tS8GwYYGPKPsx5rPtOZWtBGLj9BWZ7INw31Xe3BZQcCyc";

    public OwnPushRunner(@NonNull Context appContext, @NonNull WorkerParameters workerParams) {
        super(appContext, workerParams);
    }

    @NonNull
    @Override
    public Result doWork() {
        Log.d("OwnPush", "Runner started");
        Client pushClient = new Client(new Client.OwnPushHandler() {
            @Override
            public void onPushReceived(String message) {
                handlePushMessage(message);
            }

            @Override
            public void connectionStatus(boolean connected) {

            }
        }, Host, ClientID, Secret);

        pushClient.start();

        while(pushClient.isRunning()) {
            try {
                Thread.sleep(3000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        return Result.RETRY;
    }

    private void handlePushMessage(String message) {
        //TODO notification
    }
}
