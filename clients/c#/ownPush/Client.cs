using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace ownPush
{
    class Client
    {
        private Socket p_client;
        private string p_host;
        private static int p_port = 7951;

        class StateObject
        {
            // Client socket.  
            public Socket workSocket = null;
            // Size of receive buffer.  
            public const int BufferSize = 256;
            // Receive buffer.  
            public byte[] buffer = new byte[BufferSize];
            // Received data string.  
            public StringBuilder sb = new StringBuilder();
        }

        private ManualResetEvent connectDone = new ManualResetEvent(false);
        private ManualResetEvent sendDone = new ManualResetEvent(false);
        private ManualResetEvent receiveDone = new ManualResetEvent(false);

        public delegate void DataReceivedEventHandler(object sender, string data);
        public event DataReceivedEventHandler DataReceived;

        public delegate void ConnectedEventHandler(object sender);
        public event ConnectedEventHandler ConnectionEstablished;

        public delegate void LogHandler(object sender, string data);
        public event LogHandler WriteToLog;

        public delegate void ConnectionStateHandler(object sender, bool connected);
        public event ConnectionStateHandler ConnectionStateChanged;

        public Client(string host)
        {
            p_host = host;
        }

        public void Connect()
        {
            if(p_client == null || !p_client.Connected)
            {
                IPHostEntry ipHostInfo = Dns.GetHostEntry(p_host);
                IPAddress ipAddress = ipHostInfo.AddressList[0];
                IPEndPoint remoteEP = new IPEndPoint(ipAddress, p_port);

                p_client = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

                p_client.BeginConnect(remoteEP, new AsyncCallback(ConnectCallback), p_client);
                connectDone.WaitOne();
            }
        }

        public void Disconnect()
        {
            if (p_client != null && p_client.Connected)
            {
                p_client.Shutdown(SocketShutdown.Both);
                p_client.Close();
                ConnectionStateChanged?.Invoke(this, p_client.Connected);
            }
        }

        public void Send(string data)
        {
            if (p_client.Connected)
            {
                Send(p_client, data);
            }
        }

        private void ConnectCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.  
                Socket client = (Socket)ar.AsyncState;

                // Complete the connection.  
                client.EndConnect(ar);
                Log("Socket connected to " + client.RemoteEndPoint.ToString());
                Receive(client);

                ConnectionEstablished?.Invoke(this);
            }
            catch (Exception e)
            {
                Log(e.ToString());
            }

            // Signal that the connection has been made.  
            connectDone.Set();

            ConnectionStateChanged?.Invoke(this, p_client.Connected);
        }

        private void Receive(Socket client)
        {
            try
            {
                // Create the state object.  
                StateObject state = new StateObject
                {
                    workSocket = client
                };

                // Begin receiving the data from the remote device.  
                client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReceiveCallback), state);
            }
            catch (Exception e)
            {
                Log(e.ToString());
            }
        }
        
        private void ReceiveCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the state object and the client socket   
                // from the asynchronous state object.  
                StateObject state = (StateObject)ar.AsyncState;
                Socket client = state.workSocket;

                if (!client.Connected || client.Available == 0)
                {
                    // Connection is terminated, either by force or willingly
                    ConnectionStateChanged?.Invoke(this, false);
                    return;
                }

                // Read data from the remote device.  
                int bytesRead = client.EndReceive(ar);

                if (bytesRead > 0)
                {
                    // There might be more data, so store the data received so far.  
                    state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));

                    // Get the rest of the data.  
                    client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReceiveCallback), state);
                }
                else
                {
                    // All the data has arrived; put it in response.  
                    if (state.sb.Length > 1)
                    {
                        DataReceived?.Invoke(this, state.sb.ToString());
                    }
                    // Signal that all bytes have been received.  
                    receiveDone.Set();
                    Receive(client);
                }
            }
            catch (Exception e)
            {
                Log(e.ToString());
            }
        }

        private void Send(Socket client, String data)
        {
            // Convert the string data to byte data using ASCII encoding.  
            byte[] byteData = Encoding.ASCII.GetBytes(data);

            // Begin sending the data to the remote device.  
            client.BeginSend(byteData, 0, byteData.Length, 0, new AsyncCallback(SendCallback), client);
        }

        private void SendCallback(IAsyncResult ar)
        {
            try
            {
                // Retrieve the socket from the state object.  
                Socket client = (Socket)ar.AsyncState;

                // Complete sending the data to the remote device.  
                int bytesSent = client.EndSend(ar);
                Log("Sent " + bytesSent + " bytes to server.");

                // Signal that all bytes have been sent.  
                sendDone.Set();
            }
            catch (Exception e)
            {
                Log(e.ToString());
            }
        }

        private void Log(string data)
        {
            WriteToLog?.Invoke(this, data);
        }
    }
}
