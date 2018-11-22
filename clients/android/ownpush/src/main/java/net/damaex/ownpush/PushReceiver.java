package net.damaex.ownpush;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class PushReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        BroadcastType type = (BroadcastType) intent.getSerializableExtra(context.getString(R.string.OWNPUSH_RECEIVER_TYPE));
        OwnPushApplication app = ((OwnPushApplication) context.getApplicationContext());

        if(type != null) {
            switch (type) {
                case MESSAGE:
                    app.pushHandler.handleMessage(intent.getStringExtra(context.getString(R.string.OWNPUSH_RECEIVER_MESSAGE)));
                    break;
                case CONNECTION_STATUS:
                    app.pushHandler.handleConnectionStatus(intent.getBooleanExtra(context.getString(R.string.OWNPUSH_RECEIVER_CONNECTED), false));
                    break;
                default:
                    //just do nothing
                    break;
            }
        }
    }
}
