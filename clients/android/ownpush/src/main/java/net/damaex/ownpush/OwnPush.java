package net.damaex.ownpush;

import java.util.concurrent.TimeUnit;

import androidx.work.Data;
import androidx.work.PeriodicWorkRequest;
import androidx.work.WorkManager;

public class OwnPush {
    private OwnPush() {
        throw new AssertionError();
    }

    public static void start(String host, String clientID, String secret) {

        Data pushData = new Data.Builder()
                .putString(OwnPushRunner.HOST, host)
                .putString(OwnPushRunner.CLIENTID, clientID)
                .putString(OwnPushRunner.SECRET, secret)
                .build();

        PeriodicWorkRequest runWork = new PeriodicWorkRequest.Builder(OwnPushRunChecker.class, 30, TimeUnit.SECONDS).setInputData(pushData).build();
        WorkManager.getInstance().enqueue(runWork);
    }
}
