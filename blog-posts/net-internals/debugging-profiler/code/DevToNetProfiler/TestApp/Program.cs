using System;
using System.Threading;

namespace TestApp
{
    class Program
    {
        static void Main(string[] args)
        {
          Console.Read();

          try
          {
            throw new Exception();
          }
          catch
          {

          }

          Console.WriteLine($"from app:\tthread id: {AppDomain.GetCurrentThreadId()}");
          Console.Read();
        }
    }
}
