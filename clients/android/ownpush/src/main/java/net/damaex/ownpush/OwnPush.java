package net.damaex.ownpush;

import java.util.concurrent.TimeUnit;

import androidx.work.PeriodicWorkRequest;
import androidx.work.WorkManager;

public class OwnPush {
    private OwnPush() {
        throw new AssertionError();
    }

    public static void start() {
        PeriodicWorkRequest runWork = new PeriodicWorkRequest.Builder(OwnPushRunChecker.class, 30, TimeUnit.SECONDS).build();
        WorkManager.getInstance().enqueue(runWork);
    }
}
