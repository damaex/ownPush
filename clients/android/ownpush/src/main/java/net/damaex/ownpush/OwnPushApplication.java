package net.damaex.ownpush;

import android.app.Application;

public class OwnPushApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
    }

    public void setPushHandler(IPushHandler handler) {
        pushHandler = handler;
    }

    IPushHandler pushHandler;
}
