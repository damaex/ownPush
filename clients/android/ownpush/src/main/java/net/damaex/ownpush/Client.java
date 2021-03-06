package net.damaex.ownpush;

import com.google.gson.Gson;

import java.nio.charset.Charset;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import javax.net.ssl.SSLSocketFactory;

class Client {
    private static final int OWNPUSH_PORT = 7951;
    private final String mHost;
    private final String mClientID;
    private final String mSecret;
    private boolean mIsRunning = false;

    private TcpClient mTcpClient = null;
    private final OwnPushHandler mPushHandler;

    private final SSLSocketFactory mSocketFactory;

    Client(OwnPushHandler pushHandler, String host, String clientID, String secret, SSLSocketFactory socketFactory) {
        mPushHandler = pushHandler;
        mHost = host;
        mClientID = clientID;
        mSecret = secret;
        mSocketFactory = socketFactory;
    }

    void start() {
        mTcpClient = new TcpClient(new TcpClient.OnClientHandler() {
            @Override
            public void onMessageReceived(String message) {
                handleMessage(message);
            }

            @Override
            public void onClientConnected() {
                mPushHandler.connectionStatus(true);
                mIsRunning = true;
                sendRequest();
            }

            @Override
            public void onClientDisconnected() {
                mIsRunning = false;
                mPushHandler.connectionStatus(false);
            }
        }, mHost, OWNPUSH_PORT, mSocketFactory);

        new Thread()
        {
            public void run() {
                mTcpClient.run();
            }
        }.start();
        mIsRunning = true;
    }

    private void stop() {
        mTcpClient.stopClient();
        mIsRunning = false;
    }

    boolean isRunning() {
        return mIsRunning;
    }

    private void handleMessage(String message) {
        int length = Integer.parseInt(message.substring(0, message.indexOf(":")));
        String data = message.substring(message.indexOf(":"));

        if(length == data.length()) {
            ConnectionObject co = this.toObject(message);

            switch (co.purpose) {
                case CHALLENGE: {
                    String hash = this.SHA_512(co.data + this.mSecret);
                    ConnectionObject answer = new ConnectionObject(ConnectionObject.Purpose.LOGIN, hash);
                    this.sendMessage(answer);
                    break;
                }
                case PUSH: {
                    mPushHandler.onPushReceived(co.data);
                    break;
                }
                case RESET: {
                    //TODO reconnect
                    break;
                }
                default: {

                }
            }
        } else {
            this.stop();
            //TODO disconnect
        }
    }

    private void sendRequest() {
        ConnectionObject co =  new ConnectionObject(ConnectionObject.Purpose.REQUEST, this.mClientID);
        this.sendMessage(co);
    }

    private String SHA_512(String passwordToHash) {
        String generatedPassword = null;
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-512");
            byte[] bytes = md.digest(passwordToHash.getBytes(Charset.forName("UTF-8")));
            StringBuilder sb = new StringBuilder();

            for (byte aByte : bytes) {
                sb.append(Integer.toString((aByte & 0xff) + 0x100, 16).substring(1));
            }

            generatedPassword = sb.toString();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }

        return generatedPassword;
    }

    private void sendMessage(ConnectionObject co) {
        String data = this.fromObject(co);
        mTcpClient.sendMessage(String.valueOf(data.length()) + ":" + data);
    }

    private String fromObject(ConnectionObject co) throws NullPointerException {
        Gson gson = new Gson();
        String data = gson.toJson(co);

        if (data == null || data.isEmpty())
            throw new NullPointerException();

        return data;
    }

    private ConnectionObject toObject(String data) throws NullPointerException {
        Gson gson = new Gson();
        ConnectionObject obj = gson.fromJson(data, ConnectionObject.class);

        if (obj == null)
            throw new NullPointerException();

        return obj;
    }

    interface OwnPushHandler {
        void onPushReceived(String message);

        void connectionStatus(boolean connected);
    }
}
