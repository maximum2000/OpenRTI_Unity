using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using AOT; // MonopInvokeCallback
using System.Runtime.InteropServices;
using System;
using System.Text;
using TMPro;



public class TestDLL : MonoBehaviour
{
    [DllImport("DLLtoUnity")]
    public static extern int evokeCallback(double dT);


    [DllImport("DLLtoUnity")]
    public static extern int ListFederationExecutions(StringBuilder myString, int length);

    [DllImport("DLLtoUnity")]
    public static extern int StartRTI(StringBuilder myString, int length);

    [DllImport("DLLtoUnity")]
    public static extern int RegisterFederationSynchronizationPoint(StringBuilder myString, int length);

    [DllImport("DLLtoUnity")]
    public static extern int SynchronizationPointAchieved(StringBuilder myString, int length);



    //[DllImport("DLLtoUnity")]
    // public static extern int GetSynchronizationPointAchievedFlag(StringBuilder myString, int length);
    //[DllImport("DLLtoUnity")]
    //public static extern int GetFederationSynchronizedFlag(StringBuilder myString, int length);
    //[DllImport("DLLtoUnity")]
    //public static extern int GetSynchronizationPointFlag(StringBuilder myString, int length);
    //[DllImport("DLLtoUnity")]
    //public static extern int GetConnectionLost(StringBuilder myString, int length);

    public TextMeshProUGUI text1;

    //!!!
    //Debug
    private delegate void DebugCallback(IntPtr message, int color, int size);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterDebugCallback(DebugCallback callback);
   
    private static void DebugLog(IntPtr message, int color, int size)
    {
        string debugString = Marshal.PtrToStringAnsi(message, size);
        Debug.Log(debugString);
    }

    void OnDestroy()
    {
        RegisterDebugCallback(null);
        RegisterÑonnectionLostCallback(null);
        RegisterReportFederationExecutionsCallback(null);
        RegisterSynchronizationPointRegistrationSucceededCallback(null);
        RegisterSynchronizationPointRegistrationFailedCallback(null);
        RegisterAnnounceSynchronizationPointCallback(null);
        RegisterFederationSynchronizedCallback(null);
    }
    //!!!


    //!!!
    //connectionLost
    private delegate void ñonnectionLostCallback(IntPtr message, int size);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterÑonnectionLostCallback(ñonnectionLostCallback callback);

    private static void ConnectionLost(IntPtr message, int size)
    {
        string debugString = Marshal.PtrToStringAnsi(message, size);
        Debug.Log(debugString);
    }
    //!!!


    //!!!
    //reportFederationExecutions
    private delegate void reportFederationExecutionsCallback(IntPtr message1, IntPtr message2, int size1, int size2);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReportFederationExecutionsCallback(reportFederationExecutionsCallback callback);

    private static void ReportFederationExecutions(IntPtr message1, IntPtr message2, int size1, int size2)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
    }
    //!!!

    //!!!
    //synchronizationPointRegistrationSucceeded
    private delegate void synchronizationPointRegistrationSucceededCallback(IntPtr message1,  int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterSynchronizationPointRegistrationSucceededCallback(synchronizationPointRegistrationSucceededCallback callback);

    private static void SynchronizationPointRegistrationSucceeded(IntPtr message1,  int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //!!!


    //!!!
    //synchronizationPointRegistrationFailed
    private delegate void synchronizationPointRegistrationFailedCallback(IntPtr message1, IntPtr message2, int size1, int size2);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterSynchronizationPointRegistrationFailedCallback(synchronizationPointRegistrationFailedCallback callback);

    private static void SynchronizationPointRegistrationFailed(IntPtr message1, IntPtr message2, int size1, int size2)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
    }
    //!!!

    //!!!
    //announceSynchronizationPoint
    private delegate void announceSynchronizationPointCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAnnounceSynchronizationPointCallback(announceSynchronizationPointCallback callback);

    private static void AnnounceSynchronizationPoint(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //!!!

    //!!!
    //federationSynchronized
    private delegate void federationSynchronizedCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterFederationSynchronizedCallback(federationSynchronizedCallback callback);

    private static void FederationSynchronized(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //!!!


    public void Step6()
    {
        StringBuilder str2 = new StringBuilder(100);
        SynchronizationPointAchieved(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("SynchronizationPointAchieved: " + myString2);

        text1.text = myString2;
    }

    public void Step3()
    {
        StringBuilder str2 = new StringBuilder(100);
        RegisterFederationSynchronizationPoint(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("RegisterFederationSynchronizationPoint: " + myString2);

        text1.text = myString2;
    }



    public void Step0()
    {
        evokeCallback(0.1);
    }

    public void Step00()
    {
        StringBuilder str2 = new StringBuilder(100);
        ListFederationExecutions(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("ListFederationExecutions: " + myString2);

        text1.text = myString2;
    }

    public void  Step1()
    {
        //ðåãèñòðàöèÿ callback-ôóíêöèé
        {
            RegisterDebugCallback(DebugLog);
            RegisterÑonnectionLostCallback(ConnectionLost);
            RegisterReportFederationExecutionsCallback(ReportFederationExecutions);
            RegisterSynchronizationPointRegistrationSucceededCallback(SynchronizationPointRegistrationSucceeded);
            RegisterSynchronizationPointRegistrationFailedCallback(SynchronizationPointRegistrationFailed);
            RegisterAnnounceSynchronizationPointCallback(AnnounceSynchronizationPoint);
            RegisterFederationSynchronizedCallback(FederationSynchronized);
        }


        StringBuilder str2 = new StringBuilder(100);
        StartRTI(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("test: " + myString2);

        text1.text = myString2;
    }

    // Start is called before the first frame update
    void Start()
    {
        

        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}


