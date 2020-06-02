using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace App
{
    static class Program
    {
        private static NotifyIcon notifyIcon;

        [DllImport("Dll1.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetHook();
    
        [DllImport("Dll1.dll", CallingConvention = CallingConvention.StdCall)] 
        public static extern void RemoveHook();

        private static void SetText(bool? disabled = null)
        {
            notifyIcon.Text = "Disable XButton App";

            if (disabled == true)
            {
                notifyIcon.Text += " [Disabled]";
            } 
            else if (disabled == false)
            {
                notifyIcon.Text += " [Enabled]";
            }
        }

        private static void InitIcon()
        {
            notifyIcon = new NotifyIcon();
            notifyIcon.Icon = new Icon("example.ico");
            SetText();
            notifyIcon.Visible = true;
            notifyIcon.ContextMenuStrip = GetContextMenu();
        }

        private static ContextMenuStrip GetContextMenu()
        {
            var cm = new ContextMenuStrip();
            var item = new ToolStripMenuItem();
            item.Text = "Disable buttons";
            item.Click += DisableClick;
            cm.Items.Add(item);
            
            item = new ToolStripMenuItem();
            item.Text = "Enable buttons";
            item.Click += EnableClick;
            cm.Items.Add(item);
            cm.Items.Add(new ToolStripSeparator());

            item = new ToolStripMenuItem();
            item.Text = "Close";
            item.Click += CloseClick;
            cm.Items.Add(item);

            return cm;
        }

        private static void CloseClick(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private static void EnableClick(object sender, EventArgs e)
        {
            SetText(false);
            RemoveHook();
        }

        private static void DisableClick(object sender, EventArgs e)
        {
            SetText(true);
            SetHook();
        }

        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            InitIcon();
            Application.Run();
            notifyIcon.Dispose();
        }
    }
}
