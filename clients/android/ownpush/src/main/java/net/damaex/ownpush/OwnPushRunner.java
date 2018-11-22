package net.damaex.ownpush;

import android.content.Context;
import android.content.Intent;
import android.support.annotation.NonNull;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import javax.net.ssl.SSLSocketFactory;

import androidx.work.Worker;
import androidx.work.WorkerParameters;

class OwnPushRunner extends Worker {

    static final String HOST        = "id_host";
    static final String CLIENTID    = "id_clientid";
    static final String SECRET      = "id_secret";
    static final String CERTIFICATE = "id_certificate";
    static final String TAG         = "OwnPushRunner";

    public OwnPushRunner(@NonNull Context appContext, @NonNull WorkerParameters workerParams) {
        super(appContext, workerParams);
    }

    @NonNull
    @Override
    public Result doWork() {
        Log.d("OwnPush", "Runner started");

        int certificateResourceID = getInputData().getInt(OwnPushRunner.CERTIFICATE, 0);

        SSLSocketFactory socketFactory;

        if (certificateResourceID == 0) {
            socketFactory = (SSLSocketFactory) SSLSocketFactory.getDefault();
        } else {
            socketFactory = CustomSSLSocketFactory.create(getApplicationContext(), certificateResourceID);
        }

        Client pushClient = new Client(
                new Client.OwnPushHandler() {
                    @Override
                    public void onPushReceived(String message) {
                        handlePushMessage(message);
                    }

                    @Override
                    public void connectionStatus(boolean connected) {
                        handleConnectionStatusChanged(connected);
                    }
                },
                getInputData().getString(OwnPushRunner.HOST),
                getInputData().getString(OwnPushRunner.CLIENTID),
                getInputData().getString(OwnPushRunner.SECRET),
                socketFactory
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
        Intent intent = new Intent(getApplicationContext(), PushReceiver.class);
        intent.setAction(getApplicationContext().getString(R.string.OWNPUSH_RECEIVER_FILTER));
        intent.putExtra(getApplicationContext().getString(R.string.OWNPUSH_RECEIVER_TYPE), BroadcastType.MESSAGE);
        intent.putExtra(getApplicationContext().getString(R.string.OWNPUSH_RECEIVER_MESSAGE), message);
        getApplicationContext().sendBroadcast(intent);
    }

    private void handleConnectionStatusChanged(boolean connected) {
        Intent intent = new Intent(getApplicationContext(), PushReceiver.class);
        intent.setAction(getApplicationContext().getString(R.string.OWNPUSH_RECEIVER_FILTER));
        intent.putExtra(getApplicationContext().getString(R.string.OWNPUSH_RECEIVER_TYPE), BroadcastType.CONNECTION_STATUS);
        intent.putExtra(getApplicationContext().getString(R.string.OWNPUSH_RECEIVER_CONNECTED), connected);
        getApplicationContext().sendBroadcast(intent);
    }
}
