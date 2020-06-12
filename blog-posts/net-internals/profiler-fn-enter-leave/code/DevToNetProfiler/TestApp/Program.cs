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
            Fn1();
          }
          catch
          {
          }

          Console.Read();
        }

        static void Fn1()
        {
          Fn2();
        }

        static void Fn2()
        {
          Fn3();
        }

        static void Fn3()
        {
          throw new CatBowlEmptyException();
        }
    }

    class CoronaException : Exception
    {
    }
}
