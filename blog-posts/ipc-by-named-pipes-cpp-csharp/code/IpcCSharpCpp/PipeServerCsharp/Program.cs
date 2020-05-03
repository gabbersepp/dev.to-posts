using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PipeServerCsharp
{
    class Program
    {
        static void Main(string[] args)
        {
          var server = new PipeServer();
          server.Init();
          server.SendData("Hello from c#");

          Console.WriteLine($"read from pipe client: {server.ReadLine()}");
          server.Dispose();
        }
    }
}
