using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace ConsoleApplication1
{
    class Program
    {
        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void doTest();

        //[DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        //private static extern void doTest2(IntPtr resP,out int resSize);

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void startEncoder(int srcW, int srcH, int decW, int decH, int bitrate);

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int encode(IntPtr resP, int res_size, IntPtr decP, out int dec_size);

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void stopEncoder();

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void streamTest(int w, int h, int br);

        static void Main(string[] args)
        {
            startEncoder(1920, 1080, 1280, 720, 400000);
            byte[] res = new byte[5];
            res[0] = Convert.ToByte('a');
            res[1] = Convert.ToByte('b');
            res[2] = Convert.ToByte('c');
            res[3] = Convert.ToByte('d');
            res[4] = Convert.ToByte('e');
            IntPtr resP = Marshal.AllocHGlobal(res.Length);
            Marshal.Copy(res, 0, resP, res.Length);
            int res_size = res.Length;

            byte[] dec = new byte[5];
            IntPtr decP = Marshal.AllocHGlobal(dec.Length);
            Marshal.Copy(dec, 0, decP, dec.Length);
            int dec_size = dec.Length;

            if (encode(resP, res_size, decP, out dec_size) > 0)
            {
                byte[] data = new byte[dec_size];
                Marshal.Copy(resP, data, 0, dec_size);
                for (int i = 0; i < data.Length; i++)
                    Console.WriteLine("data:" + Convert.ToChar(data[i]));
            }
            else
            {
                Console.WriteLine("encode fial");
            }
            stopEncoder();
            Marshal.FreeHGlobal(resP);
            Marshal.FreeHGlobal(decP);
            Stopwatch stopWatch = new Stopwatch();
            stopWatch.Start();
            streamTest(1280, 720, 1500000);
            stopWatch.Stop();
            TimeSpan ts = stopWatch.Elapsed;
            // Format and display the TimeSpan value.
            string elapsedTime = String.Format("{0:00}:{1:00}:{2:00}.{3:00}",
                ts.Hours, ts.Minutes, ts.Seconds,
                ts.Milliseconds / 10);
            Console.WriteLine("RunTime " + elapsedTime);

            Console.WriteLine("press any key to exit");
            Console.ReadKey(); //Pause
        }
    }
}
