using System;
using System.Windows.Forms;

namespace ownPushClient
{
    public partial class FrmMain : Form
    {
        public FrmMain()
        {
            InitializeComponent();
        }

        private void WriteToLog(string value)
        {
            if (InvokeRequired)
            {
                this.Invoke(new Action<string>(WriteToLog), new object[] { value });
                return;
            }

            txtLog.Text += value + Environment.NewLine;
        }
    }
}
