package net.damaex.ownpush;

import android.content.Context;
import android.support.annotation.NonNull;
import android.util.Log;

import androidx.work.Worker;
import androidx.work.WorkerParameters;

public class OwnPushRunner extends Worker {

    static String HOST = "id_host";
    static String CLIENTID = "id_clientid";
    static String SECRET = "id_secret";

    public OwnPushRunner(@NonNull Context appContext, @NonNull WorkerParameters workerParams) {
        super(appContext, workerParams);
    }

    @NonNull
    @Override
    public Result doWork() {
        Log.d("OwnPush", "Runner started");
        Client pushClient = new Client(
            new Client.OwnPushHandler() {
                @Override
                public void onPushReceived(String message) {
                    handlePushMessage(message);
                }

                @Override
                public void connectionStatus(boolean connected) {

                }
            },
            getInputData().getString(OwnPushRunner.HOST),
            getInputData().getString(OwnPushRunner.CLIENTID),
            getInputData().getString(OwnPushRunner.SECRET)
        );

        pushClient.start();

        while (pushClient.isRunning()) {
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
