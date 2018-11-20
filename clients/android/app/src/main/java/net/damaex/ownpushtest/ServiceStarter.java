package net.damaex.ownpushtest;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import net.damaex.ownpush.OwnPush;

public class ServiceStarter extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        OwnPush.start(
                context.getString(R.string.OwnPushHost),
                context.getString(R.string.OwnPushClientID),
                context.getString(R.string.OwnPushSecret)
        );
    }
}
