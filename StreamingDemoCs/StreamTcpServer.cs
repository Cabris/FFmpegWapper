using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;

namespace StreamingDemo
{
    class StreamTcpServer
    {
        private TcpListener tcpListener;
        private Thread listenThread;
        public List<TcpClient> clients = new List<TcpClient>();
        List<BufferedStream> bStreams = new List<BufferedStream>();
        bool isListening = true;
        List<string> pathes = new List<string>();

        // Use this for initialization
        public void Start()
        {
            this.tcpListener = new TcpListener(IPAddress.Any, 8888);
            this.listenThread = new Thread(new ThreadStart(ListenForClients));
            this.listenThread.Start();
        }

        void ListenForClients()
        {
            //Debug.Log("ListenForClients0");
            this.tcpListener.Start();
            string ip = ((IPEndPoint)tcpListener.LocalEndpoint).Address.ToString();
            //Debug.Log("ListenForClients1: "+ip);
            string sHostName = Dns.GetHostName();
            IPHostEntry ipE = Dns.GetHostByName(sHostName);
            IPAddress[] IpA = ipE.AddressList;
            for (int i = 0; i < IpA.Length; i++)
            {
                string path = IpA[i].ToString() + ":8888";
                string s = String.Format("IP Address [{0}] {1} ", i, path);
                Console.WriteLine(s);
                pathes.Add(path);
            }

            while (isListening)
            {
                try
                {
                    //blocks until a client has connected to the server
                    TcpClient client = this.tcpListener.AcceptTcpClient();
                    Console.WriteLine("client connected!: #" + clients.Count);
                    ParameterizedThreadStart tStart = new ParameterizedThreadStart(HandleClientComm);
                    //Thread clientThread=new Thread(tStart);
                    //clientThread.Start(client);
                    HandleClientComm(client);
                    break;
                }
                catch (Exception e) { 
                
                }
                //Debug.Log("client");
            }

        }

        private void HandleClientComm(object client)
        {
            TcpClient tcpClient = (TcpClient)client;
            //tcpClient.NoDelay=true;
            tcpClient.SendBufferSize = 600000;
            clients.Add(tcpClient);
            BufferedStream bs = new BufferedStream(tcpClient.GetStream());
            bStreams.Add(bs);
        }

        public int Send(byte[] data)
        {
            int length = -1;
            for (int i = 0; i < bStreams.Count; i++)
            {
                BufferedStream bs = bStreams[i];
                length = data.Length;
                byte[] lengthData = getBytes(length);
                bs.Write(lengthData, 0, lengthData.Length);
                bs.Write(data, 0, data.Length);
                bs.Flush();
            }
            return length;
        }

        byte[] getBytes(int x)
        {
            return BitConverter2.getBytes(x);
        }

        void onDestory()
        {
            isListening = false;
            foreach (BufferedStream bs in bStreams)
                bs.Close();
            bStreams.Clear();
            foreach (TcpClient c in clients)
                c.Close();
            clients.Clear();
            tcpListener.Stop();
            listenThread.Join();
        }

       public void Stop()
        {
            if (isListening)
                onDestory();
        }

    }



}
