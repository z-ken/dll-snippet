using System;
using System.Runtime.InteropServices;

namespace dlltestcsharp
{
    class Program
    {
        public static void func(IntPtr imageData, int len)
        {
            byte[] bArray = new byte[len];

            Marshal.Copy(imageData, bArray, 0, len);
        }

        public static void result(String[] results, int size)
        {
            Console.WriteLine(results);
        }

        static void Main(string[] args)
        {
            //C++ DLL will send byte[](actually char[]) back to "func" method
            setupListener(new AddFuncListener(func));
            call();

            //C++ DLL will send string[] back to "result" method
            setupResultListener(new ResultFuncListener(result));
            callResult();
        }

        [DllImport("dlltest.dll", EntryPoint = "call", CallingConvention = CallingConvention.Cdecl)]
        public static extern void call();
        //setup call-back listener
        public delegate void AddFuncListener(IntPtr imageData, int len);
        [DllImport("dlltest.dll", EntryPoint = "setupListener", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void setupListener(AddFuncListener listener);

        [DllImport("dlltest.dll", EntryPoint = "callResult", CallingConvention = CallingConvention.Cdecl)]
        public static extern void callResult();
        //setup call-back listener
        public delegate void ResultFuncListener([MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 1)][In, Out] String[] results, int size);
        [DllImport("dlltest.dll", EntryPoint = "setupResultListener", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void setupResultListener(ResultFuncListener listener);
    }
}
