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
    //��� callback'� ����������� � �����...
    //ieee1516\DLLtoUnity\RTI1516EAmbassadorLContent.h

    //��� ������ ��� ������������ �������
    public TMP_InputField IP;

    public TextMeshProUGUI text1;
    public TMP_InputField FedarationName;
    public TMP_InputField FedarateName;

    //��� ������ ��� ������������ �������
    public TMP_InputField ObjectClassName;
    //��� �������� � ������
    public TMP_InputField ObjectAttributeName;
    //��� ������������ ���������� ������
    public TMP_InputField ObjectInstanceName;
    //����� �������� ��������
    public TMP_InputField ObjectAttributeValue;
    


    //����� ���������� ��������, ��� ��������� ����� ���������� ������ � ��������������� callback'��
    [DllImport("DLLtoUnity")]
    public static extern int evokeCallback(double dT);

    //����� �������� ������� ������������� ����������
    [DllImport("DLLtoUnity")]
    public static extern int TestInteraction(StringBuilder myString, int length);

    //����� �������� ������� ������������� ������� � ���������
    [DllImport("DLLtoUnity")]
    public static extern int TestObjects(StringBuilder myString, int length, StringBuilder className, StringBuilder attributeName, StringBuilder objectInstanceName);

    //����� �������� ������� ������������� ������� � ���������
    [DllImport("DLLtoUnity")]
    public static extern int SetValueAttributeObject(StringBuilder myString, int length, StringBuilder objectInstanceName, StringBuilder attributeName, StringBuilder attributeValue);



    //����� ������� ������� ������� ���������, ��� ��������� ����� ���������� ������ � ��������������� callback'��
    [DllImport("DLLtoUnity")]
    public static extern int ListFederationExecutions(StringBuilder myString, int length);

    //����� ������� ����������� � ���������
    [DllImport("DLLtoUnity")]
    public static extern int Connect(StringBuilder myString, int length);

    //����� ������� �������� �������� (�����)
    [DllImport("DLLtoUnity")]
    public static extern int CreateFederationExecution(StringBuilder myString1, int length, StringBuilder myString2);

    //����� ������� ������������� ���������� �������� � ���������
    [DllImport("DLLtoUnity")]
    public static extern int JoinFederationExecution(StringBuilder myString, int length);

    //����� ������� ����������� ����� �������������, ��� ��������� ����� ���������� ������ � ��������������� callback'��
    [DllImport("DLLtoUnity")]
    public static extern int RegisterFederationSynchronizationPoint(StringBuilder myString, int length);

    //����� ������� �������������, ��� ��������� ����� ���������� ������ � ��������������� callback'��
    [DllImport("DLLtoUnity")]
    public static extern int SynchronizationPointAchieved(StringBuilder myString, int length);

    //callback ��� Debug'�
    private delegate void DebugCallback(IntPtr message, int color, int size);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterDebugCallback(DebugCallback callback);
    //����� ��������� �� debug'� DLL'��
    private static void DebugLog(IntPtr message, int color, int size)
    {
        string debugString = Marshal.PtrToStringAnsi(message, size);
        Debug.Log(debugString);
    }
    //end callback ��� Debug'�

    //callback ��� ��������� � ������ ���������� (connectionLost)
    private delegate void ConnectionLostCallback(IntPtr message, int size);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterConnectionLostCallback(ConnectionLostCallback callback);
    //���������� ������� ������ ����������
    private static void ConnectionLost(IntPtr message, int size)
    {
        string debugString = Marshal.PtrToStringAnsi(message, size);
        Debug.Log(debugString);
    }
    //end callback ��� ��������� � ������ ���������� (connectionLost)


    //callback ��� ��������� ���������� � ���������� ��������� (reportFederationExecutions)
    private delegate void reportFederationExecutionsCallback(IntPtr message1, IntPtr message2, int size1, int size2);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReportFederationExecutionsCallback(reportFederationExecutionsCallback callback);
    //���������� 
    private static void ReportFederationExecutions(IntPtr message1, IntPtr message2, int size1, int size2)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
    }
    //end callback ��� ��������� ���������� � ���������� ��������� (reportFederationExecutions)

    //callback ��� ��������� ���������� � ���������� ����������� ����� ������������� (synchronizationPointRegistrationSucceeded)
    private delegate void synchronizationPointRegistrationSucceededCallback(IntPtr message1,  int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterSynchronizationPointRegistrationSucceededCallback(synchronizationPointRegistrationSucceededCallback callback);
    //����������
    private static void SynchronizationPointRegistrationSucceeded(IntPtr message1,  int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //end callback ��� ��������� ���������� � ���������� ����������� ����� ������������� (synchronizationPointRegistrationSucceeded)


    //callback ��� ��������� ���������� � ������� ����������� ����� ������������� (synchronizationPointRegistrationFailedCallback)
    private delegate void synchronizationPointRegistrationFailedCallback(IntPtr message1, IntPtr message2, int size1, int size2);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterSynchronizationPointRegistrationFailedCallback(synchronizationPointRegistrationFailedCallback callback);
    //����������
    private static void SynchronizationPointRegistrationFailed(IntPtr message1, IntPtr message2, int size1, int size2)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
    }
    //end callback ��� ��������� ���������� � ������� ����������� ����� ������������� (synchronizationPointRegistrationFailedCallback)

    //callback ��� ��������� ���������� � ��������� ������������� (announceSynchronizationPointCallback)
    private delegate void announceSynchronizationPointCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAnnounceSynchronizationPointCallback(announceSynchronizationPointCallback callback);
    //����������
    private static void AnnounceSynchronizationPoint(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //end callback ��� ��������� ���������� � ��������� ������������� (announceSynchronizationPointCallback)

    //callback ��� ��������� � ����� ����, ��� ��� ��������� ���������������� (federationSynchronized)
    private delegate void federationSynchronizedCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterFederationSynchronizedCallback(federationSynchronizedCallback callback);
    //����������
    private static void FederationSynchronized(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //end callback ��� ��������� � ����� ����, ��� ��� ��������� ���������������� (federationSynchronized)

    ///////////////////////////////////////




    //callback ��� ��������� ���������� � �������������� ����� ������� (objectInstanceNameReservationSucceeded)
    private delegate void objectInstanceNameReservationSucceededCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterObjectInstanceNameReservationSucceededCallback(objectInstanceNameReservationSucceededCallback callback);
    //����������
    private static void ObjectInstanceNameReservationSucceeded(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //callback ��� ��������� ���������� � �������������� ����� ������� (objectInstanceNameReservationSucceeded)

    //callback ��� ��������� ���������� � �������������� ����� ������� (objectInstanceNameReservationFailed)
    private delegate void objectInstanceNameReservationFailedCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterObjectInstanceNameReservationFailed(objectInstanceNameReservationFailedCallback callback);
    //����������
    private static void ObjectInstanceNameReservationFailed(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //callback ��� ��������� ���������� � �������������� ����� ������� (objectInstanceNameReservationFailed)


    //callback ��� ��������� � �������� ������ ������� (discoverObjectInstance)
    private delegate void discoverObjectInstanceCallback(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterDiscoverObjectInstance(discoverObjectInstanceCallback callback);
    //����������
    private static void DiscoverObjectInstance(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        string debugString3 = Marshal.PtrToStringAnsi(message3, size3);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
        Debug.Log(debugString3);
    }
    //callback ��� ��������� � �������� ������ ������� (discoverObjectInstance)

    //callback ��� ��������� � ��������� �������� (reflectAttributeValues)
    private delegate void reflectAttributeValuesCallback(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReflectAttributeValues(reflectAttributeValuesCallback callback);
    //����������
    private static void ReflectAttributeValues(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        string debugString3 = Marshal.PtrToStringAnsi(message3, size3);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
        Debug.Log(debugString3);
    }
    //callback ��� ��������� � ��������� �������� (reflectAttributeValues)

    //callback ��� ��������� � ��������� ���������� (receiveInteraction)
    private delegate void receiveInteractionCallback(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReceiveInteractionCallback(receiveInteractionCallback callback);
    //����������
    private static void ReceiveInteraction(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        string debugString3 = Marshal.PtrToStringAnsi(message3, size3);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
        Debug.Log(debugString3);
    }
    //callback ��� ��������� � ��������� ���������� (receiveInteraction)


    //callback ��� ��������� � �������� ������� (removeObjectInstance)
    private delegate void removeObjectInstanceCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterRemoveObjectInstanceCallback(removeObjectInstanceCallback callback);
    //����������
    private static void RemoveObjectInstance(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //callback ��� ��������� � �������� ������� (removeObjectInstance)






    void OnDestroy()
    {
        RegisterDebugCallback(null);
        RegisterConnectionLostCallback(null);
        RegisterReportFederationExecutionsCallback(null);
        RegisterSynchronizationPointRegistrationSucceededCallback(null);
        RegisterSynchronizationPointRegistrationFailedCallback(null);
        RegisterAnnounceSynchronizationPointCallback(null);
        RegisterFederationSynchronizedCallback(null);

        RegisterObjectInstanceNameReservationSucceededCallback(null);
        RegisterObjectInstanceNameReservationFailed(null);
        RegisterDiscoverObjectInstance(null);
        RegisterReflectAttributeValues(null);
        RegisterReceiveInteractionCallback(null);
        RegisterRemoveObjectInstanceCallback(null);
    }


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

    public void  Step1a()
    {
        //����������� ���� callbeck ������� (��������)
        {
            RegisterDebugCallback(DebugLog);
            RegisterConnectionLostCallback(ConnectionLost);
            RegisterReportFederationExecutionsCallback(ReportFederationExecutions);
            RegisterSynchronizationPointRegistrationSucceededCallback(SynchronizationPointRegistrationSucceeded);
            RegisterSynchronizationPointRegistrationFailedCallback(SynchronizationPointRegistrationFailed);
            RegisterAnnounceSynchronizationPointCallback(AnnounceSynchronizationPoint);
            RegisterFederationSynchronizedCallback(FederationSynchronized);


            RegisterObjectInstanceNameReservationSucceededCallback(ObjectInstanceNameReservationSucceeded);
            RegisterObjectInstanceNameReservationFailed(ObjectInstanceNameReservationFailed);
            RegisterDiscoverObjectInstance(DiscoverObjectInstance);
            RegisterReflectAttributeValues(ReflectAttributeValues);
            RegisterReceiveInteractionCallback(ReceiveInteraction);
            RegisterRemoveObjectInstanceCallback(RemoveObjectInstance);
        }


        //StringBuilder str2 = new StringBuilder(100);
        StringBuilder str2 = new StringBuilder(IP.text, 100);
        Connect(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("Connect: " + myString2);

        text1.text = myString2;
    }

    public void  Step1b()
    {
        StringBuilder str2 = new StringBuilder(FedarationName.text, 100);
        string filename = Application.streamingAssetsPath + "/fdd_test.xml";
        StringBuilder str3 = new StringBuilder(filename, 100);

        CreateFederationExecution(str2, str2.Capacity, str3);
        string myString2 = str2.ToString();
        Debug.Log("CreateFederationExecution: " + myString2);

        text1.text = myString2;
    }

    public void  Step1c()
    {
        StringBuilder str2 = new StringBuilder(FedarateName.text, 100);
        JoinFederationExecution(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("JoinFederationExecution: " + myString2);

        text1.text = myString2;
    }

    public void  StepTest()
    {
        StringBuilder str2 = new StringBuilder( 100);
        TestInteraction(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("TestInteraction: " + myString2);
        text1.text = myString2;
    }

    public void  StepTestObjects()
    {
        StringBuilder str2 = new StringBuilder( 100);

        StringBuilder className = new StringBuilder(ObjectClassName.text, 100);
        StringBuilder attributeName = new StringBuilder(ObjectAttributeName.text, 100);
        StringBuilder objectInstanceName = new StringBuilder(ObjectInstanceName.text, 100);


        TestObjects(str2, str2.Capacity, className, attributeName, objectInstanceName);
        string myString2 = str2.ToString();
        Debug.Log("TestObjects: " + myString2);
        text1.text = myString2;
    }


    public void StepSetValue ()
    {

        StringBuilder str2 = new StringBuilder(100);

        StringBuilder objectInstanceName = new StringBuilder(ObjectClassName.text, 100);
        StringBuilder attributeName = new StringBuilder(ObjectAttributeName.text, 100);
        StringBuilder attributeValue = new StringBuilder(ObjectAttributeValue.text, 100);

        SetValueAttributeObject(str2, str2.Capacity, objectInstanceName, attributeName, attributeValue);
        string myString2 = str2.ToString();
        Debug.Log("TestObjects: " + myString2);
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


