using System;

namespace TestApp
{
  class Program
  {
    static void Main(string[] args)
    {
      Console.ReadLine();

      StructFn(new TestStruct { Int1 = 101, Int2 = 102, Int3 = 103 });
      StructFn(new OneFieldStruct { Int1 = 100 });
      IntArrayFn(new[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
      StringFn("Hello from C#");
      IntFn(1234);

      Console.ReadLine();
    }

    static void IntArrayFn(int[] intArray)
    {
    }
    static void StringFn(string str)
    {
    }

    static void IntFn(int i1)
    {
    }

    static void StructFn(TestStruct t)
    {
    }

    static void StructFn(OneFieldStruct t)
    {
    }
  }

  struct TestStruct
  {
    public int Int1;
    public int Int2;
    public int Int3;
  }

  struct OneFieldStruct
  {
    public int Int1;
  }
}
