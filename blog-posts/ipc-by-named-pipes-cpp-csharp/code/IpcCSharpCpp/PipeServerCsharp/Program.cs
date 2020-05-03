using System;
using System.IO;
using System.IO.Pipes;
using System.Threading.Tasks;

namespace PipeServerCsharp
{
    class Program
    {
        static void Main(string[] args)
        {
          var instances = new Task[5];

          for (int i = 0; i < instances.Length; i++)
          {
            instances[i] = Task.Run(() =>
            {
              var namedPipeServer = new NamedPipeServerStream("my-very-cool-pipe-example", PipeDirection.InOut, 5, PipeTransmissionMode.Byte);
              var streamReader = new StreamReader(namedPipeServer);
              namedPipeServer.WaitForConnection();

              var writer = new StreamWriter(namedPipeServer);
              writer.Write("Hello from c#");
              writer.Write((char)0);
              writer.Flush();
              namedPipeServer.WaitForPipeDrain();

              Console.WriteLine($"read from pipe client: {streamReader.ReadLine()}");
              namedPipeServer.Dispose();
            });
          }

          Task.WaitAll(instances);
        }
    }
}
