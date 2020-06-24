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
          Fn3();
        }

        static void Fn3()
        {

        }
    }
}
