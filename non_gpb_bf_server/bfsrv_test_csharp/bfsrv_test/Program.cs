﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Threading;
using System.Security.Cryptography;

namespace bfsrv_test
{

    class Program
    {
        public enum eBFcmds
        {
            ver_info_cmd = 0x01,
            get_power_cmd = 0x02,
            set_power_cmd = 0x03,
            get_volume_cmd = 0x04,
            set_volume_nofade_cmd = 0x05,
            set_volume_fade_cmd = 0x06,
            get_mute_cmd = 0x07,
            set_mute_cmd = 0x08,
            get_audio_mode_cmd = 0x09,
            set_audio_mode_cmd = 0x0A,
            get_audio_source_cmd = 0x0B,
            set_audio_source_cmd = 0x0C,
            get_dsp_parameters_cmd = 0x0D,
            set_dsp_parameters_cmd = 0x0E,
            get_audio_signal_level_cmd = 0x0F,
            get_ntc_values_cmd = 0x10,
            set_dsp_tone_touch_ceff_cmd = 0x11,
        };

        static byte[] toSend = new byte[100];
        static byte[] toRecv = new byte[100];
        static byte toSendCount = 0;
        static byte toRecvCount = 0;
        static byte seq = 0;
        static eBFcmds cmd;

        static UInt16[] crc16tab = new UInt16[]
{
                0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
                0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
                0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
                0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
                0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
                0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
                0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
                0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
                0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
                0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
                0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
                0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
                0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
                0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
                0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
                0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
                0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
                0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
                0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
                0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
                0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
                0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
                0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
                0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
                0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
                0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
                0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
                0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
                0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
                0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
                0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
                0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
        };

        public static ushort ComputeChecksum(byte[] bytes, int len)
        {
            ushort crc = 0xFFFF;
            for (int ii = 0; ii < len; ii++)
            {
                UInt16 index = (byte)((crc >> 8) ^ bytes[ii]);
                crc = (ushort)((crc << 8) ^ crc16tab[index & 0x00FF]);
            }
            return crc;
        }

        static void createHeader(byte cmd, byte seq, byte[] data, byte data_size)
        {
            toSend[0] = 0x55;
            toSend[1] = 0xAA;
            toSend[2] = seq;
            toSend[3] = cmd;
            toSend[4] = data_size;
            for (byte i = 5; i < data_size + 5; i++)
            {
                toSend[i] = data[i - 5];
            }
            ushort crc16 = ComputeChecksum(toSend, data_size + 5);
            toSend[data_size + 5] = (byte)(crc16 & 0xFF);
            toSend[data_size + 6] = (byte)((crc16 >> 8) & 0xFF);


        }
        //0x55, 0xaa, 0x0, 0x10, 0x4, 0x41, 0x3b, 0x29, 0x2a, 0xa4, 0xf3
        static bool checkHeader(byte bytes_read)
        {
            ushort crc16 = ComputeChecksum(toRecv, bytes_read - 2);
            byte c1 = toRecv[bytes_read - 1];
            byte c2 = toRecv[bytes_read - 2];
            ushort rec_crc16 = (ushort)(((ushort)(c1 << 8)) | c2); 
            if (rec_crc16 != crc16)
            {
                Console.WriteLine("CRC16 - Error!");
                return false;
            }
            if ((toRecv[0] != 0x55) ||
                    (toRecv[1] != 0xAA))
            {
                Console.WriteLine("Incorrect message sign - Error!");
                return false;
            }
            if (toRecv[2] != seq)
            {
                Console.WriteLine("Incorrect message seq - Error!");
                return false;
            }
            return true;
        }


        //args[0] - com port name  for example "COM4"
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("COM<number> <command> [parameters]");
                return;
            }
            SerialPort serialPort = new SerialPort(args[0], 57600, Parity.Even, 8, StopBits.One);
            serialPort.Handshake = Handshake.None;
            serialPort.Open();
            byte dat_size = 0;
            switch (args[1])
            {
                case "ver_info_cmd":
                {
                    cmd = eBFcmds.ver_info_cmd;
                    byte[] dat = new byte[1];
                    createHeader((byte)eBFcmds.ver_info_cmd, seq, dat, dat_size);
                    toRecvCount = 12;
                    break;
                }
                case "get_power_cmd":
                {
                    cmd = eBFcmds.get_power_cmd;
                    byte[] dat = new byte[1];
                    createHeader((byte)eBFcmds.get_power_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "set_power_cmd":
                {
                    cmd = eBFcmds.set_power_cmd;
                    byte[] dat = new byte[1];
                    dat_size = 1;
                    dat[0] = (byte)Convert.ToByte(args[2], 16);
                    createHeader((byte)eBFcmds.set_power_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "get_volume_cmd":
                {
                    cmd = eBFcmds.get_volume_cmd;
                    byte[] dat = new byte[1];
                    createHeader((byte)eBFcmds.get_volume_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "set_volume_nofade_cmd":
                {
                    cmd = eBFcmds.set_volume_nofade_cmd;
                    byte[] dat = new byte[1];
                    dat_size = 1;
                    dat[0] = (byte)Convert.ToByte(args[2], 16);
                    createHeader((byte)eBFcmds.set_volume_nofade_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "set_volume_fade_cmd":
                {
                    cmd = eBFcmds.set_volume_fade_cmd;
                    byte[] dat = new byte[3];
                    dat_size = 3;
                    dat[0] = (byte)Convert.ToByte(args[2], 16);     // target volume
                    UInt16 duration = (byte)Convert.ToUInt16(args[3], 16);
                    dat[1] = (byte)(duration & 0xFF);
                    dat[2] = (byte)(duration >> 8);
                    createHeader((byte)eBFcmds.set_volume_fade_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "get_mute_cmd":
                {
                    cmd = eBFcmds.get_mute_cmd;
                    byte[] dat = new byte[1];
                    createHeader((byte)eBFcmds.get_mute_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "set_mute_cmd":
                {
                    cmd = eBFcmds.set_mute_cmd;
                    byte[] dat = new byte[1];
                    dat_size = 1;
                    dat[0] = (byte)Convert.ToByte(args[2], 16);
                    createHeader((byte)eBFcmds.set_mute_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "get_audio_mode_cmd":
                {
                    cmd = eBFcmds.get_audio_mode_cmd;
                    byte[] dat = new byte[1];
                    createHeader((byte)eBFcmds.get_audio_mode_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "set_audio_mode_cmd":
                {
                    cmd = eBFcmds.set_audio_mode_cmd;
                    byte[] dat = new byte[1];
                    dat_size = 1;
                    dat[0] = (byte)Convert.ToByte(args[2], 16);
                    createHeader((byte)eBFcmds.set_audio_mode_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "get_audio_source_cmd":
                {
                    cmd = eBFcmds.get_audio_source_cmd;
                    byte[] dat = new byte[1];
                    createHeader((byte)eBFcmds.get_audio_source_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "set_audio_source_cmd":
                {
                    cmd = eBFcmds.set_audio_source_cmd;
                    byte[] dat = new byte[1];
                    dat_size = 1;
                    dat[0] = (byte)Convert.ToByte(args[2], 16);
                    createHeader((byte)eBFcmds.set_audio_source_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "get_dsp_parameters_cmd":
                {
                    cmd = eBFcmds.get_dsp_parameters_cmd;
                    byte[] dat = new byte[1];
                    createHeader((byte)eBFcmds.get_dsp_parameters_cmd, seq, dat, dat_size);
                    toRecvCount = 10;
                    break;
                }
                case "set_dsp_parameters_cmd":
                {
                    cmd = eBFcmds.set_dsp_parameters_cmd;
                    byte[] dat = new byte[3];
                    dat_size = 3;
                    dat[0] = (byte)Convert.ToByte(args[2], 16);
                    dat[1] = (byte)Convert.ToByte(args[3], 16);
                    dat[2] = (byte)Convert.ToByte(args[4], 16);
                    createHeader((byte)eBFcmds.set_dsp_parameters_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }
                case "get_audio_signal_level_cmd":
                {
                    cmd = eBFcmds.get_audio_signal_level_cmd;
                    byte[] dat = new byte[1];
                    createHeader((byte)eBFcmds.get_audio_signal_level_cmd, seq, dat, dat_size);
                    toRecvCount = 11;
                    break;
                } 
                case "get_ntc_values_cmd":
                {
                    cmd = eBFcmds.get_ntc_values_cmd;
                    byte[] dat = new byte[1];
                    createHeader((byte)eBFcmds.get_ntc_values_cmd, seq, dat, dat_size);
                    toRecvCount = 11;
                    break;
                }
                case "set_dsp_tone_touch_ceff_cmd":
                {
                    cmd = eBFcmds.set_dsp_tone_touch_ceff_cmd;
                    byte[] dat = new byte[9];
                    dat_size = 9;
                    double gx = (double)Convert.ToDouble(args[2]);
                    double gy = (double)Convert.ToDouble(args[3]);
                    double gz = (double)Convert.ToDouble(args[4]);
                    string str_gx_frac;
                    string str_gx_int;
                    bool gx_sign = false;
                    if (gx < 0)
                    {
                        str_gx_frac = args[2].Substring(3);
                        str_gx_int = args[2].Substring(1, 1);
                        gx_sign = true;
                    }
                    else
                    {
                        str_gx_frac = args[2].Substring(2);
                        str_gx_int = args[2].Substring(0, 1);
                    }
                    UInt16 gx_frac = (UInt16)Convert.ToUInt16(str_gx_frac);
                    byte gx_int = (byte)Convert.ToByte(str_gx_int);
                    dat[0] = gx_int;
                    if (gx_sign)
                    {
                        dat[0] = (byte)(dat[0] | 0x80);
                    }
                    dat[1] = (byte)(gx_frac & 0xFF);
                    dat[2] = (byte)(gx_frac >> 8);

                    string str_gy_frac;
                    string str_gy_int;
                    bool gy_sign = false;
                    if (gy < 0)
                    {
                        str_gy_frac = args[3].Substring(3);
                        str_gy_int = args[3].Substring(1, 1);
                        gy_sign = true;
                    }
                    else
                    {
                        str_gy_frac = args[3].Substring(2);
                        str_gy_int = args[3].Substring(0, 1);
                    }
                    UInt16 gy_frac = (UInt16)Convert.ToUInt16(str_gy_frac);
                    byte gy_int = (byte)Convert.ToByte(str_gy_int);
                    dat[3] = gy_int;
                    if (gy_sign)
                    {
                        dat[3] = (byte)(dat[3] | 0x80);
                    }
                    dat[4] = (byte)(gy_frac & 0xFF);
                    dat[5] = (byte)(gy_frac >> 8);

                    string str_gz_frac;
                    string str_gz_int;
                    bool gz_sign = false;
                    if (gz < 0)
                    {
                        str_gz_frac = args[4].Substring(3);
                        str_gz_int = args[4].Substring(1, 1);
                        gz_sign = true;
                    }
                    else
                    {
                        str_gz_frac = args[4].Substring(2);
                        str_gz_int = args[4].Substring(0, 1);
                    }
                    UInt16 gz_frac = (UInt16)Convert.ToUInt16(str_gz_frac);
                    byte gz_int = (byte)Convert.ToByte(str_gz_int);
                    dat[6] = gz_int;
                    if (gz_sign)
                    {
                        dat[6] = (byte)(dat[6] | 0x80);
                    }
                    dat[7] = (byte)(gz_frac & 0xFF);
                    dat[8] = (byte)(gz_frac >> 8);
                    createHeader((byte)eBFcmds.set_dsp_tone_touch_ceff_cmd, seq, dat, dat_size);
                    toRecvCount = 8;
                    break;
                }


                defaut:
                {
                        Console.WriteLine("Unknown command- Error!");
                        return;
                        break;
                }
            }
            toSendCount = (byte)(dat_size + 7);

            serialPort.Write(toSend,0, toSendCount);
            DateTime start = DateTime.Now;
            DateTime now;// = DateTime.Now;
            int timeout = 0;

            while ((serialPort.BytesToRead < toRecvCount) && (timeout < 100))
            {
                now = DateTime.Now;
                TimeSpan span = now - start;
                timeout = (int)span.TotalMilliseconds;
            }

            if (serialPort.BytesToRead < toRecvCount)
            {
                Console.WriteLine("Could not receive response in timeout. Exit");
                return;
            }
            serialPort.Read(toRecv,0, toRecvCount);
            serialPort.Close();
            Console.Write("Sent:     ");

            for (int ii = 0; ii < toSendCount; ii++)
            {
                Console.Write("0x{0} ", toSend[ii].ToString("X2"));
            }
            Console.WriteLine("");
            //0x55, 0xaa, 0x0, 0x1, 0x5, 0x1, 0x0, 0x2, 0x1, 0x1, 0x52, 0x2e
            //toRecv[0] = 0x55;
            //toRecv[1] = 0xAA;
            //toRecv[2] = 0x00;
            //toRecv[3] = 0x01;
            //toRecv[4] = 0x05;
            //toRecv[5] = 0x01;
            //toRecv[6] = 0x00;
            //toRecv[7] = 0x02;
            //toRecv[8] = 0x01;
            //toRecv[9] = 0x01;
            //toRecv[10] = 0x52;
            //toRecv[11] = 0x2E;

            Console.Write("Received: ");

            for (int ii = 0; ii < toRecvCount; ii++)
            {
                Console.Write("0x{0} ", toRecv[ii].ToString("X2"));
            }
            Console.WriteLine("");
            switch(cmd)
            {
                case eBFcmds.ver_info_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.ver_info_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: sw ver: {0}.{1}.{2}.{3}; hw ver:{4};", toRecv[5], toRecv[6], toRecv[7], toRecv[8], toRecv[9]);
                        }
                        break;
                    }
                case eBFcmds.get_power_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.get_power_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Power mode: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.set_power_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.set_power_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Return status: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.get_volume_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.get_volume_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Volume: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.set_volume_nofade_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.set_volume_nofade_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Return status: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.set_volume_fade_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.set_volume_fade_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Return status: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.get_mute_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.get_mute_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Mute: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.set_mute_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.set_mute_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Return status: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.get_audio_mode_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.get_audio_mode_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            if (toRecv[5] == 1)
                            {
                                Console.WriteLine("Message OK: Audio mode: entertainment;");
                            }
                            else
                            {
                                Console.WriteLine("Message OK: Audio mode: massage;");
                            }
                        }
                        break;
                    }
                case eBFcmds.set_audio_mode_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.set_audio_mode_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Return status: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.get_audio_source_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.get_audio_source_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            //Console.WriteLine("Message OK: Mute: {0};", toRecv[5]);
                            switch(toRecv[5])
                            {
                                case 0:
                                    Console.WriteLine("Message OK: Current audio source: wifi-bt");
                                    break;
                                case 1:
                                    Console.WriteLine("Message OK: Current audio source: sd-card");
                                    break;
                                case 2:
                                    Console.WriteLine("Message OK: Current audio source: spdif");
                                    break;
                                case 3:
                                    Console.WriteLine("Message OK: Current audio source: aux");
                                    break;
                                default:
                                    Console.WriteLine("Unknow audio source - error!");
                                    break;
                            }
                        }
                        break;
                    }
                case eBFcmds.set_audio_source_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.set_audio_source_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Return status: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.get_dsp_parameters_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.get_dsp_parameters_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Loudness: {0}; Treble:{1}; Bass{2};", toRecv[5], toRecv[6], toRecv[7]);
                        }
                        break;
                    }
                case eBFcmds.set_dsp_parameters_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.set_dsp_parameters_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Return status: {0};", toRecv[5]);
                        }
                        break;
                    }
                case eBFcmds.get_audio_signal_level_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.get_audio_signal_level_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: WifiBT:-{0}dB; SDcard:-{1}dB; SPDIF:-{2}dB; Aux:-{3}dB;", toRecv[5], toRecv[6], toRecv[7], toRecv[8]);
                        }
                        break;
                    }
                case eBFcmds.get_ntc_values_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.get_ntc_values_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Amp1:{0}C; Amp2:{1}C; WfLeft:{2}dB; WfRigth:{3}C;", toRecv[5], toRecv[6], toRecv[7], toRecv[8]);
                        }
                        break;
                    }
                case eBFcmds.set_dsp_tone_touch_ceff_cmd:
                    {
                        if (checkHeader(toRecvCount))
                        {
                            if (toRecv[3] != (byte)eBFcmds.set_dsp_tone_touch_ceff_cmd)
                            {
                                Console.WriteLine("Incorrect message command - Error!");
                                return;
                            }
                            Console.WriteLine("Message OK: Return status: {0};", toRecv[5]);
                        }
                        break;
                    }
                default:
                    Console.WriteLine();
                    break;

            }
            seq++;
        }
    }
}


