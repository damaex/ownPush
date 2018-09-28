using Newtonsoft.Json;
using System;
using System.Text;
using WatsonTcp;

namespace ownPush
{
    class Handler
    {
        private WatsonTcpClient p_client;
        private static readonly int p_port = 7951;

        private readonly string p_host;
        private readonly string p_clientID;
        private readonly string p_secret;

        public delegate void LogHandler(object sender, string data);
        public event LogHandler WriteToLog;

        public delegate void ConnectionStateHandler(object sender, bool connected);
        public event ConnectionStateHandler ConnectionStateChanged;

        public Handler(string host, string clientID, string secret)
        {
            p_clientID = clientID;
            p_secret = secret;
            p_host = host;            
        }

        private bool ServerDisconnected()
        {
            ConnectionStateChanged?.Invoke(this, false);
            p_client = null;
            return true;
        }

        public void Start()
        {
            if (p_client == null)
                p_client = new WatsonTcpClient(p_host, p_port, ServerConnected, ServerDisconnected, MessageReceived, true);
        }

        public void Stop()
        {
            if (p_client != null)
                p_client.Dispose();
        }

        private bool MessageReceived(byte[] data)
        {
            return HandleData(JsonConvert.DeserializeObject<ConnectionObject>(Encoding.UTF8.GetString(data)));
        }

        private bool HandleData(ConnectionObject co)
        {
            WriteToLog?.Invoke(this, co.ToString());

            switch(co.purpose)
            {
                case Purpose.CHALLENGE:
                    break;
                case Purpose.PUSH:
                    //TODO PUSH received, handle and show
                    break;
                case Purpose.RESET:
                    //TODO RESET received -> reconnect
                    break;
            }

            return true;
        }

        private bool ServerConnected()
        {
            ConnectionStateChanged?.Invoke(this, true);
            SendRequest();
            return true;
        }

        private void SendRequest()
        {
            ConnectionObject co = new ConnectionObject(Purpose.REQUEST, p_clientID);
            string json = JsonConvert.SerializeObject(co);
            p_client.Send(Encoding.UTF8.GetBytes(json));
        }
    }
}
