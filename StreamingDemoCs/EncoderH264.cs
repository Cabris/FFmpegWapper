using System.Linq;
using System.Collections;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;
using System;
using System.Diagnostics;

namespace StreamingDemo
{
    class EncoderH264
    {
        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void startEncoder(int srcW, int srcH, int decW, int decH, int bitrate, int framerate);

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern int encode(IntPtr resP, int res_size, IntPtr decP, out int dec_size);

        [DllImport("FFmpegCppWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void stopEncoder();

        int outWidth, outHeight, fps;
        int srcW, srcH;
        int bitRate = 800000;

        byte[] src, dec;
        int src_size, dec_size;
        IntPtr srcP, decP;

        Stopwatch stopWatch;
        bool isStarted = false;
        bool isStoped = false;
        FileStream fs;
        Object obj;
        bool debug = false;

        public EncoderH264(IntPtr srcP, int srcW, int srcH, bool isDebug)
        {
            this.srcP = srcP;
            this.src_size = srcW * srcH * 4;
            this.debug = isDebug;
            this.srcW = srcW;
            this.srcH = srcH;
            this.obj = this;
        }


        public void Prepare()
        {
            dec = new byte[outWidth * outHeight * 3 / 2];
            dec_size = dec.Length;
            decP = Marshal.AllocHGlobal(dec_size);
            Marshal.Copy(dec, 0, decP, dec_size);
            stopWatch = new Stopwatch();
        }

        public void StartEncoder()
        {
            lock (obj)
            {
                try
                {
                    if (debug)
                    {
                        string path = @"MyTest_h264.mp4";
                        fs = new FileStream(path, FileMode.Create, FileAccess.Write, FileShare.Write);
                    }
                    startEncoder(srcW, srcH, outWidth, outHeight, bitRate, fps);
                    isStarted = true;
                }
                catch (Exception e)
                {
                    throw e;
                }
            }
        }

        public byte[] Encoding()
        {
            lock (obj)
            {
                byte[] encoded = new byte[0];
                if (!isStarted || isStoped)
                    throw new Exception("wrong state");
                try
                {
                    stopWatch.Reset();
                    stopWatch.Start();
                    int r = encode(srcP, src_size, decP, out dec_size);
                    stopWatch.Stop();
                    TimeSpan ts = stopWatch.Elapsed;
                    string elapsedTime = String.Format("{0:00}", ts.Milliseconds);
                    Console.WriteLine("Encode Time: " + elapsedTime+"ms");

                    if (r > 0)
                    {
                        Marshal.Copy(decP, dec, 0, dec_size);
                        encoded = new byte[dec_size];
                        Buffer.BlockCopy(dec, 0, encoded, 0, dec_size);
                        if (debug)
                            fs.Write(dec, 0, dec_size);
                    }
                    else
                    {
                        Console.WriteLine("encode fial");
                    }
                }
                catch (Exception e)
                {
                    throw e;
                }
                return encoded;
            }
        }

        public void StopEncoder()
        {
            lock (obj)
            {
                if (isStoped || !isStarted)
                    return;
                isStoped = true;
                if (debug)
                {
                    byte[] endcode = new byte[] { 0, 0, 1, 0xb7 };
                    fs.Write(endcode, 0, endcode.Length);
                    fs.Close();
                }
                Marshal.FreeHGlobal(decP);
                stopEncoder();
            }
        }

        public int OutWidth
        {
            get
            {
                return outWidth;
            }
            set
            {
                outWidth = value;
            }
        }

        public int OutHeight
        {
            get
            {
                return outHeight;
            }
            set
            {
                outHeight = value;
            }
        }

        public int Fps
        {
            get
            {
                return fps;
            }
            set
            {
                fps = value;
            }
        }

        public int BitRate
        {
            get
            {
                return bitRate;
            }
            set
            {
                if (value > 0)
                    bitRate = value;
            }
        }
    }
}
