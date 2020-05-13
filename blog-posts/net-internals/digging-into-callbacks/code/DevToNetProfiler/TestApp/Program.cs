using System;
using System.Threading;

namespace TestApp
{
    class Program
    {
        static void Main(string[] args)
        {
          new Program();

          try
          {
            throw new CoronaException();
          }
          catch
          {
          }

          Console.Read();
        }
    }

    class CoronaException : Exception
    {
    }
}
