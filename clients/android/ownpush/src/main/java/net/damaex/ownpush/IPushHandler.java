package net.damaex.ownpush;

public interface IPushHandler {
    void handleMessage(String message);

    void handleConnectionStatus(boolean connected);
}
