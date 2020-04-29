using System;
using System.Diagnostics;

namespace BranchPrediction
{
    class Program
    {
        static void Main(string[] args)
        {
            var sw = new Stopwatch();
            sw.Start();
            SameBranch();
            sw.Stop();
            var sameBranchTime = sw.ElapsedMilliseconds;

            sw.Reset();
            sw.Start();
            DifferentBranch();
            sw.Stop();
            var differentBranchTime = sw.ElapsedMilliseconds;

            Console.WriteLine($"different-branches: {differentBranchTime}, same-branch: {sameBranchTime}");
            Console.Read();
        }

        static int DifferentBranch()
        {
            int result = 0;

            for (var i = 0; i < 1000000000; i++)
            {
                if (i > Random(0, 1000000000))
                {
                    result = i;
                }
                else
                {
                    result = i;
                }
            }

            return result;
        }

        static int SameBranch()
        {
            int result = 0;

            for (var i = 1000000000; i < 2000000000; i++)
            {
                if (i > Random(0, 1000000000))
                {
                    result = i;
                }
                else
                {
                    result = i;
                }
            }

            return result;
        }

        static Random random = new Random();

        static int Random(int from, int to)
        {
            return random.Next(from, to);
        }
    }
}
