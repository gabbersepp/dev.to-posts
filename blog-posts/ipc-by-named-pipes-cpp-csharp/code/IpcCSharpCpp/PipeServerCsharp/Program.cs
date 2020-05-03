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
          Console.WriteLine(server.ReadLine());
          server.Dispose();
          Console.Read();
        }
    }
}
