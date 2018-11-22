package net.damaex.ownpushtest;

import net.damaex.ownpush.OwnPushApplication;

public class OwnPushTestApplication extends OwnPushApplication {
    @Override
    public void onCreate() {
        super.onCreate();

        setPushHandler(new PushHandler(getApplicationContext()));
        ServiceStarter.start(getApplicationContext());
    }
}
