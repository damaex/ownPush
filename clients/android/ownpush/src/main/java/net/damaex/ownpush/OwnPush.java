package net.damaex.ownpush;

import android.support.annotation.RawRes;

import java.util.concurrent.TimeUnit;

import androidx.work.Data;
import androidx.work.PeriodicWorkRequest;
import androidx.work.WorkManager;

/**
 * OwnPush main integration class which will start and manage the service
 */
public class OwnPush {
    private OwnPush() {
        throw new AssertionError();
    }

    /**
     * start the ownPush background service
     *
     * @param host      the host to connect to
     * @param clientID  the clientID for this client
     * @param secret    the secret for this client
     */
    public static void start(String host, String clientID, String secret) {
        start(host, clientID, secret, 0);
    }

    /**
     * start the ownPush background service
     *
     * @param host                  the host to connect to
     * @param clientID              the clientID for this client
     * @param secret                the secret for this client
     * @param certificateResource   a certificate resource to use
     */
    public static void start(String host, String clientID, String secret, @RawRes int certificateResource) {

        Data pushData = new Data.Builder()
                .putString(OwnPushRunner.HOST, host)
                .putString(OwnPushRunner.CLIENTID, clientID)
                .putString(OwnPushRunner.SECRET, secret)
                .putInt(OwnPushRunner.CERTIFICATE, certificateResource)
                .build();

        PeriodicWorkRequest runWork = new PeriodicWorkRequest.Builder(
                OwnPushRunChecker.class,
                30,
                TimeUnit.SECONDS
        ).addTag(OwnPushRunChecker.TAG).setInputData(pushData).build();

        WorkManager.getInstance().enqueue(runWork);
    }

    public static void stop() {
        WorkManager.getInstance().cancelAllWorkByTag(OwnPushRunner.TAG);
        WorkManager.getInstance().cancelAllWorkByTag(OwnPushRunChecker.TAG);
    }
}
