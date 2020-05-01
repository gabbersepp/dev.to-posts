using System;

namespace TestApp
{
    class Program
    {
        static void Main(string[] args)
        {
          Console.WriteLine($"System.Environment.Is64BitOperatingSystem: {System.Environment.Is64BitOperatingSystem}");
          Console.WriteLine($"System.Environment.Is64BitProcess: {System.Environment.Is64BitProcess}");

          Console.Read();
        }
    }
}
