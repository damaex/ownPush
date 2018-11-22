package net.damaex.ownpush;

import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.InetSocketAddress;

import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;

class TcpClient {

    // message to send to the server
    private String mServerMessage;
    // sends message received notifications
    private final OnClientHandler mMessageListener;
    // the host to connect to
    private final String mHost;
    // the port to connect to
    private final int mPort;
    // while this is true, the server will continue running
    private boolean mRun = false;
    // used to send messages
    private PrintWriter mBufferOut;
    // used to read messages from the server
    private BufferedReader mBufferIn;

    private static int CONNECT_TIMEOUT = 5000;

    private final SSLSocketFactory mSocketFactory;

    /**
     * Constructor of the class. OnMessagedReceived listens for the messages received from server
     */
    TcpClient(OnClientHandler listener, String host, int port, SSLSocketFactory socketFactory) {
        mMessageListener = listener;
        mHost = host;
        mPort = port;
        mSocketFactory = socketFactory;
    }

    /**
     * Sends the message entered by client to the server
     *
     * @param message text entered by client
     */
    void sendMessage(String message) {
        if (mBufferOut != null && !mBufferOut.checkError()) {
            mBufferOut.println(message);
            mBufferOut.flush();
        }
    }

    /**
     * Close the connection and release the members
     */
    void stopClient() {
        mRun = false;

        if (mBufferOut != null) {
            mBufferOut.flush();
            mBufferOut.close();
        }

        mBufferIn = null;
        mBufferOut = null;
        mServerMessage = null;
    }

    void run() {

        mRun = true;

        try {
            //here you must put your computer's IP address.
            InetAddress serverAddress = InetAddress.getByName(mHost);

            Log.e("TCP Client", "C: Connecting...");

            //create a ssl socket to make the connection with the server
            SSLSocket socket = (SSLSocket) mSocketFactory.createSocket();
            socket.connect(new InetSocketAddress(serverAddress, mPort), CONNECT_TIMEOUT);

            try {

                //sends the message to the server
                mBufferOut = new PrintWriter(new BufferedWriter(new OutputStreamWriter(socket.getOutputStream())), true);

                //receives the message which the server sends back
                mBufferIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));

                mMessageListener.onClientConnected();

                //in this while the client listens for the messages sent by the server
                while (mRun) {

                    mServerMessage = mBufferIn.readLine();

                    if (mServerMessage != null && mMessageListener != null) {
                        //call the method messageReceived from MyActivity class
                        mMessageListener.onMessageReceived(mServerMessage);
                    }

                }

                Log.e("RESPONSE FROM SERVER", "S: Received Message: '" + mServerMessage + "'");

            } catch (Exception e) {

                Log.e("TCP", "S: Error", e);

            } finally {
                mRun = false;

                //the socket must be closed. It is not possible to reconnect to this socket
                //after it is closed, which means a new socket instance has to be created.
                socket.close();
                mMessageListener.onClientDisconnected();
            }

        } catch (Exception e) {

            Log.e("TCP", "C: Error", e);
            mMessageListener.onClientDisconnected();

        }

    }

    /**
     * Declare the interface. The method messageReceived(String message) will must be implemented
     * in the MyActivity class at on asyncTask doInBackground
     */
    public interface OnClientHandler {
        void onMessageReceived(String message);

        void onClientConnected();

        void onClientDisconnected();
    }
}