using System;
using System.Runtime.InteropServices;
using System.Text;

namespace ConsoleApp1
{
  public class CustomMarshaler : ICustomMarshaler
  {
    void ICustomMarshaler.CleanUpManagedData(object ManagedObj)
    {
    }

    void ICustomMarshaler.CleanUpNativeData(IntPtr pNativeData)
    {
      Marshal.FreeHGlobal(pNativeData + sizeof(int));
      Marshal.FreeHGlobal(pNativeData);
    }

    int ICustomMarshaler.GetNativeDataSize()
    {
      return -1;
    }

    IntPtr ICustomMarshaler.MarshalManagedToNative(object ManagedObj)
    {
      var casted = ManagedObj as TestFnParams;
      if (casted == null)
      {
        return IntPtr.Zero;
      }

      var bytes = Encoding.UTF8.GetBytes(casted.PropertyB);
      var ptr = Marshal.AllocHGlobal(sizeof(int) + Marshal.SizeOf(typeof(IntPtr)));
      var strPtr = Marshal.AllocHGlobal(bytes.Length + 1);
      var strPtr2 = Marshal.StringToCoTaskMemAnsi(casted.PropertyB);
      Marshal.WriteInt32(ptr, casted.PropertyA);
      Marshal.Copy(bytes, 0, strPtr, bytes.Length);
      Marshal.WriteByte(strPtr + bytes.Length, 0);
      Marshal.WriteIntPtr(ptr + sizeof(int), strPtr);
      return ptr;
    }

    object ICustomMarshaler.MarshalNativeToManaged(IntPtr pNativeData)
    {
      throw new NotImplementedException();
    }

    public static ICustomMarshaler GetInstance(string cookie)
    {
      return new CustomMarshaler();
    }
  }

  class Program
  {
    [DllImport("Dll1.dll", CallingConvention = CallingConvention.StdCall)]
    public static extern void TestFunction([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(CustomMarshaler))] TestFnParams p);

    static void Main(string[] args)
    {
      var obj = new TestFnParams
      {
        PropertyA = 100,
        PropertyB = "Hello from managed object"
      };
      TestFunction(obj);
      Console.Read();
    }
  }

  public class TestFnParams
  {
    public int PropertyA { get; set; }
    public string PropertyB { get; set; }
  }
}
