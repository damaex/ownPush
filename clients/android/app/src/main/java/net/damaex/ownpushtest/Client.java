package net.damaex.ownpushtest;

public class Client {
    private static final int OWNPUSH_PORT = 7951;
    private String mHost;
    private String mClientID;
    private String mSecret;
    private TcpClient mTcpClient = null;

    public Client(String host, String clientID, String secret) {
        mHost = host;
        mClientID = clientID;
        mSecret = secret;
    }

    public void start() {
        mTcpClient = new TcpClient(new TcpClient.OnClientHandler() {
            @Override
            //here the messageReceived method is implemented
            public void onMessageReceived(String message) {
                //this method calls the onProgressUpdate
                handleMessage(message);
            }

            @Override
            public void onClientConnected() {

            }

            @Override
            public void onClientDisconnected() {

            }
        }, mHost, OWNPUSH_PORT);
        mTcpClient.run();
    }

    public void stop() {
        mTcpClient.stopClient();
    }

    private void handleMessage(String message) {

    }
}
