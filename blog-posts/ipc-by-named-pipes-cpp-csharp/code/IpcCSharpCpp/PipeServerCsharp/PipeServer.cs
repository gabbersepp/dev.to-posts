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
      streamReader = new StreamReader(namedPipeServer);
    }

    public void Init()
    {
      namedPipeServer.WaitForConnection();
    }

    public void SendData(string data)
    {
      var writer = new StreamWriter(namedPipeServer);
      writer.Write(data);
      // attention: sending char not int! int = 4 bytes!
      writer.Write((char)0);
      writer.Flush();
      namedPipeServer.WaitForPipeDrain();
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
