using System;
using System.Threading;

namespace TestApp
{
    class Program
    {
        static void Main(string[] args)
        {
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
