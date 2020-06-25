using System;
using System.Threading;

namespace TestApp
{
    class Program
    {
        static void Main(string[] args)
        {
          Console.ReadLine();
          Fn1();
          Console.ReadLine();
        }

        static void Fn1()
        {
          Fn2();
        }

        static void Fn2()
        {
          Fn3(0);
        }

        static void Fn3(int index)
        {
          if (index > 30)
          {
            return;
          }
          Fn3(index + 1);
        }
    }
}
