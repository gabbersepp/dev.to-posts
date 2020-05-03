using System;
using System.IO;
using System.IO.Pipes;

namespace PipeServerCsharp
{
  public class PipeServer : IDisposable
  {
    private readonly NamedPipeServerStream namedPipeServer;
    private readonly StreamReader streamReader;

    public PipeServer()
    {
      namedPipeServer = new NamedPipeServerStream("my-very-cool-pipe-example", PipeDirection.InOut, 1, PipeTransmissionMode.Message);
      namedPipeServer.WaitForConnection();
      streamReader = new StreamReader(namedPipeServer);
    }

    public string ReadLine()
    {
      return streamReader.ReadLine();
    }

    public void Dispose()
    {
      namedPipeServer.Close();
    }
  }
}
