using System;
using System.Windows.Forms;

namespace ownPushClient
{
    public partial class FrmMain : Form
    {
        private ownPush.Handler ownPushHandler;

        public FrmMain()
        {
            InitializeComponent();
        }

        private void WriteToLog(string value)
        {
            if (this.InvokeRequired)
            {
                this.BeginInvoke(new Action<string>(WriteToLog), new object[] { value });
                return;
            }

            txtLog.Text += value + Environment.NewLine;
        }

        private void BtnConnect_Click(object sender, EventArgs e)
        {
            btnConnect.Enabled = false;

            if (ownPushHandler == null)
            {
                ownPushHandler = new ownPush.Handler(txtIP.Text, txtClientID.Text, txtSecret.Text);

                ownPushHandler.ConnectionStateChanged += OwnPushHandler_ConnectionStateChanged;
                ownPushHandler.WriteToLog += OwnPushHandler_WriteToLog;

                ownPushHandler.Start();
            }
        }

        private void OwnPushHandler_WriteToLog(object sender, string data)
        {
            WriteToLog(data);
        }

        private void ButtonEnabler(bool connected)
        {
            btnConnect.Enabled = !connected;
            btnDisconnect.Enabled = connected;
        }

        private void OwnPushHandler_ConnectionStateChanged(object sender, bool connected)
        {
            if (this.InvokeRequired)
            {
                this.BeginInvoke(new Action<object, bool>(OwnPushHandler_ConnectionStateChanged), new object[] { sender, connected });
                return;
            }

            if (connected)
            {
                ButtonEnabler(true);
            }
            else
            {
                ownPushHandler = null;
                ButtonEnabler(false);
            }
        }

        private void BtnDisconnect_Click(object sender, EventArgs e)
        {
            btnDisconnect.Enabled = false;
            if (ownPushHandler != null)
            {
                ownPushHandler.Stop();
            }
        }
    }
}
