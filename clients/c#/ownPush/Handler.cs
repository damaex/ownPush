using Newtonsoft.Json;
using System;

namespace ownPush
{
    class Handler
    {
        private Client p_ownPushClient;
        private string p_clientID;
        private string p_secret;

        public delegate void LogHandler(object sender, string data);
        public event LogHandler WriteToLog;

        public delegate void ConnectionStateHandler(object sender, bool connected);
        public event ConnectionStateHandler ConnectionStateChanged;

        public Handler(string host, string clientID, string secret)
        {
            p_clientID = clientID;
            p_secret = secret;

            p_ownPushClient = new Client(host);
            p_ownPushClient.DataReceived += ownPushClient_dataReceived;
            p_ownPushClient.ConnectionEstablished += ownPushClient_connectionEstablished;
            p_ownPushClient.WriteToLog += ownPushClient_WriteToLog;
        }

        private void ownPushClient_WriteToLog(object sender, string data)
        {
            WriteToLog?.Invoke(sender, data);
        }

        public void Start()
        {
            p_ownPushClient.Connect();
        }

        public void Stop()
        {
            p_ownPushClient.Disconnect();
        }

        private void ownPushClient_dataReceived(object sender, string data)
        {
            ConnectionObject co = JsonConvert.DeserializeObject<ConnectionObject>(data);
        }

        private void ownPushClient_connectionEstablished(object sender)
        {
            SendRequest();
        }

        private void SendRequest()
        {
            ConnectionObject co = new ConnectionObject(Purpose.REQUEST, p_clientID);
            string json = JsonConvert.SerializeObject(co);
            p_ownPushClient.Send(json);
        }
    }
}
