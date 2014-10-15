using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.IO;
using System.Threading;

namespace ConsoleApplication1
{
    class Program
    {
        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void startEncoder(int srcW, int srcH, int decW, int decH, int bitrate,int framerate);

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int encode(IntPtr resP, int res_size, IntPtr decP, out int dec_size);

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void stopEncoder();

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void startDecoder(int width, int height);

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int decode(IntPtr resP, int res_size, IntPtr decP, out int dec_size);

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void stopDecoder();

        Timer t;

      public  void Start()
        {
            t = new Timer(new TimerCallback(Send2));
            t.Change(0, 1000 / 60);
        }

      public void Stop()
      {
            t.Change(Timeout.Infinite,Timeout.Infinite);
            t.Dispose();
        }

        void Send2(object s)
        {
            Console.WriteLine("s");
        }

        static void Main(string[] args)
        {
            Program p = new Program();
            p.Start();
            Console.ReadKey(); //Pause
            p.Stop();
            return;
            int srcW = 1280, srcH = 720;
            int decW = 1280, decH = 720;
            int fps = 30;
            int length = 1;
            int fc = length * fps;
            string path = @"MyTest_h264.mp4";
            FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write, FileShare.Write);

            startEncoder(srcW, srcH, decW, decH, 800000, fps);

            byte[] src = new byte[srcW * srcH*3];
            int res_size = src.Length;
            createDummyFrame(srcW, srcH, src, fc, 0);
            IntPtr resP = Marshal.AllocHGlobal(res_size);
            Marshal.Copy(src, 0, resP, res_size);
           
            byte[] dec = new byte[decW * decH*3/2];
            int dec_size = dec.Length;
            IntPtr decP = Marshal.AllocHGlobal(dec_size);
            Marshal.Copy(dec, 0, decP, dec_size);

            Stopwatch stopWatch = new Stopwatch();
            stopWatch.Start();
            for (int c = 0; c < fc; c++)
            {
                createDummyFrame(srcW, srcH, src, fc, c);
                Marshal.Copy(src, 0, resP, src.Length);
                if (encode(resP, res_size, decP, out dec_size) > 0)
                {
                    Marshal.Copy(decP, dec, 0, dec_size);
                    Console.WriteLine("Write frame {0,3} (size={1,7})**" , c, dec_size);
                    fs.Write(dec, 0, dec_size);
                    createTempFrameFile("f_"+c+".txt",dec,dec_size);
                }
                else
                {
                    Console.WriteLine("encode fial");
                }
            }
            stopWatch.Stop();
            TimeSpan ts = stopWatch.Elapsed;
            stopEncoder();
            // Format and display the TimeSpan value.
            string elapsedTime = String.Format("{0:00}:{1:00}:{2:00}.{3:00}",
                ts.Hours, ts.Minutes, ts.Seconds,
                ts.Milliseconds / 10);
            Console.WriteLine("RunTime " + elapsedTime);
            Console.WriteLine("press any key to exit");
       
            byte[] endcode = new byte[]{ 0, 0, 1, 0xb7 };
            fs.Write(endcode, 0, endcode.Length);
            fs.Close();
            Marshal.FreeHGlobal(resP);
            Marshal.FreeHGlobal(decP);
            Console.ReadKey(); //Pause
        }

        private static void createTempFrameFile(string fileName,byte[] data,int size) {
            FileStream fs = new FileStream(fileName, FileMode.Create, FileAccess.Write, FileShare.Write);
            fs.Write(data, 0, size);
            fs.Close();
        }

        private static void createDummyFrame(int srcW, int srcH, byte[] src, int fc, int c)
        {
            int index = 0;
            for (int y = 0; y < srcH; y++)
            {
                for (int x = 0; x < srcW; x++)
                {
                    src[index++] = (byte)(x + c * 255 / fc); // B
                    src[index++] = (byte)(128 + y - c * 255 / fc); // G
                    src[index++] = (byte)(64 - x + c * 255 / fc); // R
                }
            }
        }
    }
}
