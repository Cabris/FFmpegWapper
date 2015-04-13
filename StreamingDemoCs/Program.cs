using System.Linq;
using System.Collections;
using System.Text;
using System.Timers;
using System;
using System.Threading;
using System.Runtime.InteropServices;

namespace StreamingDemo
{
    class Program
    {
        static void Main(string[] args)
        {
            ClientSystem client = new ClientSystem();
            client.StartTest();
            Console.WriteLine("press any key to exit");
            Console.ReadKey();
        }
    }

    class ClientSystem {
        IntPtr srcP;
        EncoderH264 encoder;
        StreamTcpServer server;
        int inW = 800, inH = 480;
        int outWidth = 800, outHeight = 480;
        int fps = 30;
        int bitRate = 2000000;
        byte[] demoFrame;
        public ClientSystem() {
            int src_size = inW * inH * 4;//rgba
            demoFrame = new byte[src_size];
            bool outputFile = true;
            srcP = Marshal.AllocHGlobal(src_size);
            encoder = new EncoderH264(srcP, inW, inH, outputFile);
            encoder.OutWidth = outWidth;
            encoder.OutHeight = outHeight;
            encoder.Fps = fps;
            encoder.BitRate = bitRate;
            encoder.Prepare();
            server = new StreamTcpServer();
            server.Start();
        }

        public void StartTest(){
            Console.WriteLine("start test");
            Console.WriteLine("waiting for connect, please enter the ip&port to APP and press 'connect' ");
            while (server.clients.Count == 0) {
            }
            encoder.StartEncoder();
            float length = 5f;//5 sec test
            for (int i = 0; i < length * fps; i++) {
                createDummyFrame(inW, inH, demoFrame, (int)(length * fps), i);
                Marshal.Copy(demoFrame, 0, srcP, demoFrame.Length);
                Console.Write("frame#"+i+", ");
                byte[] dec= encoder.Encoding();
                server.Send(dec);
            }
            encoder.StopEncoder();
            server.Stop();
            Marshal.FreeHGlobal(srcP);
            Console.WriteLine("stop test");
        }

        private void createDummyFrame(int srcW, int srcH, byte[] src, int fc, int c)//fate frame
        {
            int index = 0;
            for (int y = 0; y < srcH; y++)
            {
                for (int x = 0; x < srcW; x++)
                {
                    src[index++] = (byte)(x + c * 255 / fc); // R
                    src[index++] = (byte)(128 + y - c * 255 / fc); // G
                    src[index++] = (byte)(64 - x + c * 255 / fc); // B
                    src[index++] = (byte)(255); // A
                }
            }
        }
    }
}
