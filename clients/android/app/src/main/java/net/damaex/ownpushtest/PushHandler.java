package net.damaex.ownpushtest;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.content.ContextWrapper;
import android.os.Build;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;

import net.damaex.ownpush.IPushHandler;

class PushHandler extends ContextWrapper implements IPushHandler {

    final String CHANNEL_ID = "ownPushChannel";

    public PushHandler(Context base) {
        super(base);
        createNotificationChannel();
    }

    public void handleMessage(String message) {
        showNotification(message.hashCode(), createNotification(getString(R.string.notification_message_title), message));
    }

    public void handleConnectionStatus(boolean connected) {
        showNotification(1, createNotification(getString(R.string.notification_status_title), connected ? getString(R.string.notification_status_connected) : getString(R.string.notification_status_disconnected)));
    }

    private Notification createNotification(String title, String message) {
        NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(this, CHANNEL_ID)
                .setSmallIcon(R.mipmap.ic_launcher)
                .setContentTitle(title)
                .setContentText(message)
                .setDefaults(~Notification.DEFAULT_VIBRATE)
                .setPriority(NotificationCompat.PRIORITY_DEFAULT);

        return mBuilder.build();
    }

    private void showNotification(int id, Notification notification) {
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);

        // notificationId is a unique int for each notification that you must define
        notificationManager.notify(id, notification);
    }

    private void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence name = getString(R.string.channel_name);
            String description = getString(R.string.channel_description);
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }
}
