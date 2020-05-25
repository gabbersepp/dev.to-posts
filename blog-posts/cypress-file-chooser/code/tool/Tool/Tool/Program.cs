using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;

namespace Tool
{
    class Program
    {
      [DllImport("user32.dll", SetLastError = true)]
      public static extern IntPtr FindWindowEx(IntPtr parentHandle, IntPtr childAfter, string className, string windowTitle);

      [DllImport("user32.dll", CharSet = CharSet.Auto)]
      public static extern IntPtr SendMessage(IntPtr hWnd, int Msg, IntPtr wParam, StringBuilder lParam);

      [STAThread]
      static void Main(string[] args)
      {
        Thread.Sleep(1000);

        IntPtr fileChooserHandle = FindHandle(IntPtr.Zero, null, "Öffnen");

        var comboboxExHandle = FindHandle(fileChooserHandle, "ComboBoxEx32", null);
        var comboboxHandle = FindHandle(comboboxExHandle, "ComboBox", null);
        var editHandle = FindHandle(comboboxHandle, "Edit", null);
        var btnHandle = FindWindowEx(fileChooserHandle, IntPtr.Zero, "Button", null);

        // WM_SETTEXT
        SendMessage(editHandle, 0x000C, IntPtr.Zero, new StringBuilder(args[0]));

        // LeftButtonDown
        SendMessage(btnHandle, 513, IntPtr.Zero, null);
        // LeftButtonUp
        SendMessage(btnHandle, 514, IntPtr.Zero, null);
      }

      static IntPtr FindHandle(IntPtr parentHandle, string className, string title)
      {
        IntPtr handle = IntPtr.Zero;

        for (var i = 0; i < 50; i++)
        {
          handle = FindWindowEx(parentHandle, IntPtr.Zero, className, title);

          if (handle == IntPtr.Zero)
          {
            Thread.Sleep(100);
          }
          else
          {
            break;
          }
        }

        return handle;
      }
    }
}
