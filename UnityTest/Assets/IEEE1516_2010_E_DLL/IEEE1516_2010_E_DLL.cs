using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using AOT; // MonopInvokeCallback
using System.Runtime.InteropServices;
using System;
using System.Text;
using TMPro;
using UnityEngine.Events;

using System.Threading;


[System.Serializable]
public class MyInteractionEvent : UnityEvent<int> { }


public class IEEE1516_2010_E_DLL : MonoBehaviour
{
    //Все callback'и расположены в файле...
    //ieee1516\DLLtoUnity\RTI1516EAmbassadorLContent.h

    /// Background thread for TcpServer workload.  
    private Thread My_Thread;
    private Mutex mutexObj;
    bool stop = false;


    public MyInteractionEvent ReceiveInteractionEvent;
    public static List<int> allEvent;
   //public  Action<int> HealthChangedAction;


   //имя класса для создаваемого объекта
   public TMP_InputField IP;

    //public TextMeshProUGUI text1;
    public TMP_InputField FedarationName;
    public TMP_InputField FedarateName;

    
    
   








    //Получить хендл класса по имени (ambassador->getObjectClassHandle(L"HLAobjectRoot.ObjectClass0"))
    [DllImport("DLLtoUnity")]
    public static extern int GetObjectClassHandle(StringBuilder className, int length);

    //Получить хендл атрибута по имени (ambassador->getAttributeHandle(objectClassHandle, L"Attribute0"))
    [DllImport("DLLtoUnity")]
    public static extern int GetAttributeHandle(StringBuilder className, int length, StringBuilder attributeName);

    //Получить хендл экземпляра объекта по имени  getObjectInstanceHandle
    //[DllImport("DLLtoUnity")]
    //public static extern int GetObjectInstanceHandle(StringBuilder ObjectInstanceName, int length);

    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    public static extern int GetObjectInstanceHandle(StringBuilder ObjectInstanceName, int length);
    
    //опубликовать объект и атрибуты (через запятую) (ambassador->publishObjectClassAttributes (...)
    [DllImport("DLLtoUnity")]
    public static extern int PublishObjectClassAttributes(StringBuilder className, int length, StringBuilder attributeName);

    //подписка на получение событий этого класса объекта ( ambassador->subscribeObjectClassAttributes(objectClassHandle, attributes))
    [DllImport("DLLtoUnity")]
    public static extern int SubscribeObjectClassAttributes(StringBuilder className, int length, StringBuilder attributeName);

    //зарегистрировать имя  (ambassador->reserveObjectInstanceName(L"objectInstanceName1"))
    [DllImport("DLLtoUnity")]
    public static extern int ReserveObjectInstanceName(StringBuilder objectInstanceName, int length);

    //создание и регистрация нового экземпляра объекта класса (objectInstanceHandle = ambassador->registerObjectInstance(objectClassHandle, L"objectInstanceName1")) +-возврат хендла?
    [DllImport("DLLtoUnity")]
    public static extern int RegisterObjectInstance(StringBuilder objectClassName, int length, StringBuilder objectInstanceName);

    //изменение значений атрибутов экземпляра объекта (ambassador->updateAttributeValues(objectInstanceHandle, attributeValues, tag))
    [DllImport("DLLtoUnity")]
    public static extern int UpdateAttributeValues(StringBuilder objectClassName, int length, StringBuilder ObjectInstanceName, StringBuilder attributeName, StringBuilder value);


    //add 2
    //получение класса интеракции (InteractionClass0Handle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0"))
    [DllImport("DLLtoUnity")]
    public static extern int GetInteractionClassHandle(StringBuilder interactionClassName, int length);

    //получение класса параметров интеракции (class0Parameter0Handle = ambassador->getParameterHandle(InteractionClass0Handle, L"Parameter0"))
    [DllImport("DLLtoUnity")]
    public static extern int GetParameterHandle(StringBuilder interactionClassName, int length, StringBuilder parametrName);


    //подписка на эту интеракцию (ambassador->subscribeInteractionClass(InteractionClass0Handle))
    [DllImport("DLLtoUnity")]
    public static extern int SubscribeInteractionClass(StringBuilder interactionClassName, int length);


    //публикация интеракции (ambassador->publishInteractionClass(InteractionClass0Handle))
    [DllImport("DLLtoUnity")]
    public static extern int PublishInteractionClass(StringBuilder interactionClassName, int length);


    //отправить интеракцию ( ambassador->sendInteraction(InteractionClass0Handle, parameterValues, ambassador->getFederateHandle().encode())
    [DllImport("DLLtoUnity")]
    public static extern int SendInteraction(StringBuilder interactionClassName, int length, StringBuilder parametrNames, StringBuilder values);

    //выйти из федерации resignFederationExecution(rti1516e::ResignAction resignAction)
    [DllImport("DLLtoUnity")]
    public static extern int ResignFederationExecution(StringBuilder returnstring, int length);


    //сообщаем о желании безусловной отдачи владения другому федерату (толкаем-отдаем права) - unconditionalAttributeOwnershipDivestiture
    [DllImport("DLLtoUnity")]
    public static extern int UnconditionalAttributeOwnershipDivestiture(StringBuilder string1, int length1, StringBuilder string2, StringBuilder string3);

    //сообщаем о желании согласованной отдачи владения другому федерату (толкаем-отдаем права) - negotiatedAttributeOwnershipDivestiture
    [DllImport("DLLtoUnity")]
    public static extern int NegotiatedAttributeOwnershipDivestiture(StringBuilder string1, int length1, StringBuilder string2, StringBuilder string3);

    //подтверждаем отдачу владения (толкаем-отдаем права) - confirmDivestiture
    [DllImport("DLLtoUnity")]
    public static extern int ConfirmDivestiture(StringBuilder string1, int length1, StringBuilder string2, StringBuilder string3);






    //Навязчиво просим отдать права нам (тянем-требуем права) - attributeOwnershipAcquisition
    [DllImport("DLLtoUnity")]
    public static extern int AttributeOwnershipAcquisition(StringBuilder string1, int length1);

    //Пытаемся спросить отдать права нам, если сейчас они никому не пренадлежат, если комуто принадлежат надо навязчиво требовать (тянем права) - attributeOwnershipAcquisitionIfAvailable
    [DllImport("DLLtoUnity")]
    public static extern int AttributeOwnershipAcquisitionIfAvailable(StringBuilder string1, int length1);

    //отказать в передаче владения, даже если нас навязчиво попросили - attributeOwnershipReleaseDenied
    [DllImport("DLLtoUnity")]
    public static extern int AttributeOwnershipReleaseDenied(StringBuilder string1, int length1);

    //разрешить передачу владения - attributeOwnershipDivestitureIfWanted
    [DllImport("DLLtoUnity")]
    public static extern int AttributeOwnershipDivestitureIfWanted(StringBuilder string1, int length1);

    //прекратить согласование передачи прав - cancelNegotiatedAttributeOwnershipDivestiture
    [DllImport("DLLtoUnity")]
    public static extern int CancelNegotiatedAttributeOwnershipDivestiture(StringBuilder string1, int length1);

    //cancelAttributeOwnershipAcquisition
    [DllImport("DLLtoUnity")]
    public static extern int CancelAttributeOwnershipAcquisition(StringBuilder string1, int length1);

    //видимо узнать кому сейчас паринадлежат права - queryAttributeOwnership
    [DllImport("DLLtoUnity")]
    public static extern int queryAttributeOwnership(StringBuilder string1, int length1);

    //видимо узнать, нам сейчас сейчас паринадлежат права или нет - isAttributeOwnedByFederate
    [DllImport("DLLtoUnity")]
    public static extern int isAttributeOwnedByFederate(StringBuilder string1, int length1);





    //вызов обновления федерата, все сообщения будут возвращены поздже в соответствующих callback'ах
    [DllImport("DLLtoUnity")]
    public static extern int evokeCallback(double dT);

    //вызов тестовой функции тетстирования интеракции
    [DllImport("DLLtoUnity")]
    public static extern int TestInteraction(StringBuilder myString, int length);

    //вызов тестовой функции тетстирования объекта и аттрибута
    [DllImport("DLLtoUnity")]
    public static extern int TestObjects(StringBuilder myString, int length, StringBuilder className, StringBuilder attributeName, StringBuilder objectInstanceName);

    //вызов тестовой функции тетстирования объекта и аттрибута
    [DllImport("DLLtoUnity")]
    public static extern int SetValueAttributeObject(StringBuilder myString, int length, StringBuilder objectInstanceName, StringBuilder attributeName, StringBuilder attributeValue);



    //вызов функции запроса состава федерации, все сообщения будут возвращены поздже в соответствующих callback'ах
    [DllImport("DLLtoUnity")]
    public static extern int ListFederationExecutions(StringBuilder myString, int length);

    //вызов функции подключения к федерации
    [DllImport("DLLtoUnity")]
    public static extern int Connect(StringBuilder myString, int length);

    //вызов функции создания федерата (этого)
    [DllImport("DLLtoUnity")]
    public static extern int CreateFederationExecution(StringBuilder myString1, int length, StringBuilder myString2);

    //вызов функции присоединения созданного федерата к федерации
    [DllImport("DLLtoUnity")]
    public static extern int JoinFederationExecution(StringBuilder myString, int length);

    //вызов функции регистрации точки синхронизации, все сообщения будут возвращены поздже в соответствующих callback'ах
    [DllImport("DLLtoUnity")]
    public static extern int RegisterFederationSynchronizationPoint(StringBuilder myString, int length);

    //вызов функции синхронизации, все сообщения будут возвращены поздже в соответствующих callback'ах
    [DllImport("DLLtoUnity")]
    public static extern int SynchronizationPointAchieved(StringBuilder myString, int length);

    //callback для Debug'а
    private delegate void DebugCallback(IntPtr message, int color, int size);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterDebugCallback(DebugCallback callback);
    //вывод сообщения из debug'а DLL'ки
    private static void DebugLog(IntPtr message, int color, int size)
    {
        string debugString = Marshal.PtrToStringAnsi(message, size);
        Debug.Log("c# debug:" + debugString);
    }
    //end callback для Debug'а

    //callback для сообщений о потери соединения (connectionLost)
    private delegate void ConnectionLostCallback(IntPtr message, int size);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterConnectionLostCallback(ConnectionLostCallback callback);
    //Обработчик события потери соединения
    private static void ConnectionLost(IntPtr message, int size)
    {
        Debug.Log("c# callback:" + "ConnectionLost");
        string debugString = Marshal.PtrToStringAnsi(message, size);
        Debug.Log("c# debug:" + debugString);
    }
    //end callback для сообщений о потери соединения (connectionLost)


    //callback для сообщений информации о выполнении федерации (reportFederationExecutions)
    private delegate void reportFederationExecutionsCallback(IntPtr message1, IntPtr message2, int size1, int size2);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReportFederationExecutionsCallback(reportFederationExecutionsCallback callback);
    //обработчик 
    private static void ReportFederationExecutions(IntPtr message1, IntPtr message2, int size1, int size2)
    {
        Debug.Log("c# callback:" + "ReportFederationExecutions");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        Debug.Log("c# debug:federationExecutionName:" + debugString1);
        Debug.Log("c# debug:logicalTimeImplementationName:" + debugString2);
    }
    //end callback для сообщений информации о выполнении федерации (reportFederationExecutions)

    //callback для сообщений информации о успешности регистрации точки синхронизации (synchronizationPointRegistrationSucceeded)
    private delegate void synchronizationPointRegistrationSucceededCallback(IntPtr message1,  int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterSynchronizationPointRegistrationSucceededCallback(synchronizationPointRegistrationSucceededCallback callback);
    //Обработчик
    private static void SynchronizationPointRegistrationSucceeded(IntPtr message1,  int size1)
    {
        Debug.Log("c# callback:" + "SynchronizationPointRegistrationSucceeded");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:label:" + debugString1);
    }
    //end callback для сообщений информации о успешности регистрации точки синхронизации (synchronizationPointRegistrationSucceeded)


    //callback для сообщений информации о провале регистрации точки синхронизации (synchronizationPointRegistrationFailedCallback)
    private delegate void synchronizationPointRegistrationFailedCallback(IntPtr message1, IntPtr message2, int size1, int size2);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterSynchronizationPointRegistrationFailedCallback(synchronizationPointRegistrationFailedCallback callback);
    //Обработчик
    private static void SynchronizationPointRegistrationFailed(IntPtr message1, IntPtr message2, int size1, int size2)
    {
        Debug.Log("c# callback:" + "SynchronizationPointRegistrationFailed");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        Debug.Log("c# debug:label:" + debugString1);
        Debug.Log("c# debug:synchronizationPointFlag:" + debugString2);
    }
    //end callback для сообщений информации о провале регистрации точки синхронизации (synchronizationPointRegistrationFailedCallback)

    //callback для сообщений информации о установке синхронизации (announceSynchronizationPointCallback)
    private delegate void announceSynchronizationPointCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAnnounceSynchronizationPointCallback(announceSynchronizationPointCallback callback);
    //Обработчик
    private static void AnnounceSynchronizationPoint(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "AnnounceSynchronizationPoint");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:label:" + debugString1);
    }
    //end callback для сообщений информации о установке синхронизации (announceSynchronizationPointCallback)

    //callback для сообщений о факте того, что вся федерация синхронизирована (federationSynchronized)
    private delegate void federationSynchronizedCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterFederationSynchronizedCallback(federationSynchronizedCallback callback);
    //Обработчик
    private static void FederationSynchronized(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "FederationSynchronized");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:label:" + debugString1);
    }
    //end callback для сообщений о факте того, что вся федерация синхронизирована (federationSynchronized)

    //callback для сообщений информации о резервировании имени объекта (objectInstanceNameReservationSucceeded)
    private delegate void objectInstanceNameReservationSucceededCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterObjectInstanceNameReservationSucceededCallback(objectInstanceNameReservationSucceededCallback callback);
    //Обработчик
    private static void ObjectInstanceNameReservationSucceeded(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "ObjectInstanceNameReservationSucceeded");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:name:" + debugString1);
    }
    //callback для сообщений информации о резервировании имени объекта (objectInstanceNameReservationSucceeded)

    //callback для сообщений информации о резервировании имени объекта (objectInstanceNameReservationFailed)
    private delegate void objectInstanceNameReservationFailedCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterObjectInstanceNameReservationFailed(objectInstanceNameReservationFailedCallback callback);
    //Обработчик
    private static void ObjectInstanceNameReservationFailed(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "ObjectInstanceNameReservationFailed");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:name:" + debugString1);
    }
    //callback для сообщений информации о резервировании имени объекта (objectInstanceNameReservationFailed)


    //callback для сообщений о создании нового объекта (discoverObjectInstance)
    private delegate void discoverObjectInstanceCallback(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterDiscoverObjectInstance(discoverObjectInstanceCallback callback);
    //Обработчик
    private static void DiscoverObjectInstance(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3)
    {
        Debug.Log("c# callback:" + "DiscoverObjectInstance");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        string debugString3 = Marshal.PtrToStringAnsi(message3, size3);
        Debug.Log("c# debug:objectClassHandle:" + debugString1);
        Debug.Log("c# debug:objectInstanceHandle:" + debugString2);
        Debug.Log("c# debug:objectInstanceName:" + debugString3);
    }
    //callback для сообщений о создании нового объекта (discoverObjectInstance)

    //callback для сообщений о изменении атрибута (reflectAttributeValues)
    private delegate void reflectAttributeValuesCallback(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReflectAttributeValues(reflectAttributeValuesCallback callback);
    //Обработчик
    private static void ReflectAttributeValues(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3)
    {
        Debug.Log("c# callback:" + "ReflectAttributeValues");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        string debugString3 = Marshal.PtrToStringAnsi(message3, size3);
        Debug.Log("c# debug:objectInstanceHandle:" + debugString1);
        Debug.Log("c# debug:attributeHandle:" + debugString2);
        Debug.Log("c# debug:value:" + debugString3);
    }
    //callback для сообщений о изменении атрибута (reflectAttributeValues)

    //callback для сообщений о получении интеракции (receiveInteraction)
    private delegate void receiveInteractionCallback(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReceiveInteractionCallback(receiveInteractionCallback callback);
    //Обработчик
    private static void ReceiveInteraction(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3)
    {
        Debug.Log("c# callback:" + "ReceiveInteraction");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        string debugString3 = Marshal.PtrToStringAnsi(message3, size3);
        Debug.Log("c# debug:objectInstanceHandle:" + debugString1);
        Debug.Log("c# debug:parameterHandle:" + debugString2);
        Debug.Log("c# debug:value:" + debugString3);

        //int temp = 2;
        //ReceiveInteractionEvent.Invoke(temp);
        //HealthChangedAction?.Invoke(2);
    }
    //callback для сообщений о получении интеракции (receiveInteraction)


    //callback для сообщений о удалении объекта (removeObjectInstance)
    private delegate void removeObjectInstanceCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterRemoveObjectInstanceCallback(removeObjectInstanceCallback callback);
    //Обработчик
    private static void RemoveObjectInstance(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "RemoveObjectInstance");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:objectInstanceHandle:" + debugString1);
    }
    //callback для сообщений о удалении объекта (removeObjectInstance)


    //virtual void requestAttributeOwnershipAssumption(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& offeredAttributes,    rti1516e::VariableLengthData const& tag)
    private delegate void requestAttributeOwnershipAssumptionCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterRequestAttributeOwnershipAssumptionCallback(requestAttributeOwnershipAssumptionCallback callback);
    //Обработчик
    private static void RequestAttributeOwnershipAssumption(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "RequestAttributeOwnershipAssumption");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:" + debugString1);
    }
    //virtual void requestAttributeOwnershipAssumption

    //virtual void requestDivestitureConfirmation(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& releasedAttributes)
    private delegate void requestDivestitureConfirmationCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterRequestDivestitureConfirmationCallback(requestDivestitureConfirmationCallback callback);
    //Обработчик
    private static void RequestDivestitureConfirmation(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "RequestDivestitureConfirmation");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:" + debugString1);
    }
    //virtual void requestDivestitureConfirmation

    //virtual void attributeOwnershipAcquisitionNotification(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& securedAttributes,    rti1516e::VariableLengthData const& tag)
    private delegate void attributeOwnershipAcquisitionNotificationCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAttributeOwnershipAcquisitionNotificationCallback(attributeOwnershipAcquisitionNotificationCallback callback);
    //Обработчик
    private static void AttributeOwnershipAcquisitionNotification(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "AttributeOwnershipAcquisitionNotification");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:" + debugString1);
    }
    //virtual void attributeOwnershipAcquisitionNotification

    //virtual void attributeOwnershipUnavailable(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& attributes)
    private delegate void attributeOwnershipUnavailableCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAttributeOwnershipUnavailableCallback(attributeOwnershipUnavailableCallback callback);
    //Обработчик
    private static void AttributeOwnershipUnavailable(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "AttributeOwnershipUnavailable");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:" + debugString1);
    }
    //virtual void attributeOwnershipUnavailable

    //virtual void requestAttributeOwnershipRelease(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& candidateAttributes,    rti1516e::VariableLengthData const& tag)
    private delegate void requestAttributeOwnershipReleaseCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterRequestAttributeOwnershipReleaseCallback(requestAttributeOwnershipReleaseCallback callback);
    //Обработчик
    private static void RequestAttributeOwnershipRelease(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "RequestAttributeOwnershipRelease");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:" + debugString1);
    }
    //virtual void requestAttributeOwnershipRelease

    //virtual void confirmAttributeOwnershipAcquisitionCancellation(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& attributes)
    private delegate void confirmAttributeOwnershipAcquisitionCancellationCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterConfirmAttributeOwnershipAcquisitionCancellationCallback(confirmAttributeOwnershipAcquisitionCancellationCallback callback);
    //Обработчик
    private static void ConfirmAttributeOwnershipAcquisitionCancellation(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "ConfirmAttributeOwnershipAcquisitionCancellation");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:" + debugString1);
    }
    //virtual void confirmAttributeOwnershipAcquisitionCancellation

    //virtual void informAttributeOwnership(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandle attribute,    rti1516e::FederateHandle owner)
    private delegate void informAttributeOwnershipCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterInformAttributeOwnershipCallback(informAttributeOwnershipCallback callback);
    //Обработчик
    private static void InformAttributeOwnership(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "InformAttributeOwnership");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:" + debugString1);
    }
    //virtual void informAttributeOwnership



    //virtual void attributeIsNotOwned(rti1516e::ObjectInstanceHandle objectInstanceHandle, rti1516e::AttributeHandle attribute)
    private delegate void attributeIsNotOwnedCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAttributeIsNotOwnedCallback(attributeIsNotOwnedCallback callback);
    //Обработчик
    private static void AttributeIsNotOwned(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "AttributeIsNotOwned");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:" + debugString1);
    }
    //virtual void attributeIsNotOwned



    private delegate void attributeIsOwnedByRTICallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAttributeIsOwnedByRTICallback(attributeIsOwnedByRTICallback callback);
    //Обработчик
    private static void AttributeIsOwnedByRTI(IntPtr message1, int size1)
    {
        Debug.Log("c# callback:" + "AttributeIsOwnedByRTI");
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log("c# debug:" + debugString1);
    }

    //-----------------------------------------------------------------------------------------------

    public void RegisterCallback()
    {
        //регистрация всех callbeck функций (привязка)
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


            RegisterRequestAttributeOwnershipAssumptionCallback(RequestAttributeOwnershipAssumption);
            RegisterRequestDivestitureConfirmationCallback(RequestDivestitureConfirmation);
            RegisterAttributeOwnershipAcquisitionNotificationCallback(AttributeOwnershipAcquisitionNotification);
            RegisterAttributeOwnershipUnavailableCallback(AttributeOwnershipUnavailable);
            RegisterRequestAttributeOwnershipReleaseCallback(RequestAttributeOwnershipRelease);
            RegisterConfirmAttributeOwnershipAcquisitionCancellationCallback(ConfirmAttributeOwnershipAcquisitionCancellation);
            RegisterInformAttributeOwnershipCallback(InformAttributeOwnership);
            RegisterAttributeIsNotOwnedCallback(AttributeIsNotOwned);
            RegisterAttributeIsOwnedByRTICallback(AttributeIsOwnedByRTI);
        }


        
    }

    public void connect()
    {
        //StringBuilder str2 = new StringBuilder(100);
        StringBuilder str2 = new StringBuilder(IP.text, 100);
        Connect(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("c# debug:" + "Connect: " + myString2);
        //text1.text = myString2;
    }


    public void createFederationExecution()
    {
        StringBuilder str2 = new StringBuilder(FedarationName.text, 100);
        string filename = Application.streamingAssetsPath + "/fdd_test.xml";
        StringBuilder str3 = new StringBuilder(filename, 100);
        CreateFederationExecution(str2, str2.Capacity, str3);
        string myString2 = str2.ToString();
        Debug.Log("c# debug:" + "CreateFederationExecution: " + myString2);
        //text1.text = myString2;
    }


    public void joinFederationExecution()
    {
        StringBuilder str2 = new StringBuilder(FedarateName.text, 100);
        JoinFederationExecution(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("c# debug:" + "JoinFederationExecution: " + myString2);
        //text1.text = myString2;
    }


    public void registerFederationSynchronizationPoint()
    {
        StringBuilder str2 = new StringBuilder(100);
        RegisterFederationSynchronizationPoint(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("c# debug:" + "RegisterFederationSynchronizationPoint: " + myString2);
        //text1.text = myString2;
    }


    public void synchronizationPointAchieved()
    {
        StringBuilder str2 = new StringBuilder(100);
        SynchronizationPointAchieved(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("c# debug:" + "SynchronizationPointAchieved: " + myString2);
        //text1.text = myString2;
    }

    public void listFederationExecutions()
    {
        StringBuilder str2 = new StringBuilder(100);
        ListFederationExecutions(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("c# debug:" + "ListFederationExecutions: " + myString2);
        //text1.text = myString2;
    }

    public void CallEvokeCallback()
    {
        evokeCallback(0.1);
    }

    public TMP_InputField getObjectClassHandle_1;
    public void getObjectClassHandle()
    {
        StringBuilder className = new StringBuilder(getObjectClassHandle_1.text, 100); //"HLAobjectRoot.ObjectClass0"
        GetObjectClassHandle(className, className.Capacity);
        string myString = className.ToString();
        Debug.Log("c# debug:" + "->GetObjectClassHandle: " + myString);
        evokeCallback(0.1);
    }

    public TMP_InputField getAttributeHandle_1;
    public TMP_InputField getAttributeHandle_2;
    public void getAttributeHandle()
    {
        StringBuilder className = new StringBuilder(getAttributeHandle_1.text, 100); //"HLAobjectRoot.ObjectClass0"
        StringBuilder attributeName = new StringBuilder(getAttributeHandle_2.text, 100); //"Attribute0"
        GetAttributeHandle(className, className.Capacity, attributeName);
        string myString = className.ToString();
        Debug.Log("c# debug:" + "->GetAttributeHandle: " + myString);
        evokeCallback(0.1);
    }

    public TMP_InputField getObjectInstanceHandle_1;
    public void getObjectInstanceHandle()
    {
        StringBuilder ObjectInstanceName = new StringBuilder(getObjectInstanceHandle_1.text, 100); //"Valve124
        GetObjectInstanceHandle(ObjectInstanceName, ObjectInstanceName.Capacity);
        string myString = ObjectInstanceName.ToString();
        Debug.Log("c# debug:" + "->ObjectInstanceName: " + myString);
        evokeCallback(0.1);
    }

    public TMP_InputField publishObjectClassAttributes_1;
    public TMP_InputField publishObjectClassAttributes_2;
    public void publishObjectClassAttributes()
    {
        StringBuilder className = new StringBuilder(publishObjectClassAttributes_1.text, 100); //"HLAobjectRoot.ObjectClass0"
        StringBuilder attributeName = new StringBuilder(publishObjectClassAttributes_2.text, 100); //"Attribute0"
        PublishObjectClassAttributes(className, className.Capacity, attributeName);
        string myString = className.ToString();
        Debug.Log("c# debug:" + "->PublishObjectClassAttributes: " + myString);
        evokeCallback(0.1);
    }

    public TMP_InputField subscribeObjectClassAttributes_1;
    public TMP_InputField subscribeObjectClassAttributes_2;
    public void subscribeObjectClassAttributes()
    {
        StringBuilder className = new StringBuilder(subscribeObjectClassAttributes_1.text, 100); //"HLAobjectRoot.ObjectClass0"
        StringBuilder attributeName = new StringBuilder(subscribeObjectClassAttributes_2.text, 100); //"Attribute0"
        SubscribeObjectClassAttributes(className, className.Capacity, attributeName);
        string myString = className.ToString();
        Debug.Log("c# debug:" + "->SubscribeObjectClassAttributes: " + myString);
        evokeCallback(0.1);
    }

    public TMP_InputField reserveObjectInstanceName_1;
    public void reserveObjectInstanceName()
    {
        StringBuilder objectInstanceName = new StringBuilder(reserveObjectInstanceName_1.text, 100); //"Valve444"
        ReserveObjectInstanceName(objectInstanceName, objectInstanceName.Capacity);
        string myString = objectInstanceName.ToString();
        Debug.Log("c# debug:" + "->ReserveObjectInstanceName: " + myString);
        evokeCallback(0.1);
    }

    public TMP_InputField registerObjectInstance_1;
    public TMP_InputField registerObjectInstance_2;
    public void registerObjectInstance()
    {
        StringBuilder objectClassName = new StringBuilder(registerObjectInstance_1.text, 100); //"HLAobjectRoot.ObjectClass0"
        StringBuilder objectInstanceName = new StringBuilder(registerObjectInstance_2.text, 100); //"Valve444"
        RegisterObjectInstance(objectClassName, objectClassName.Capacity, objectInstanceName);
        string myString = objectClassName.ToString();
        Debug.Log("c# debug:" + "->RegisterObjectInstance: " + myString);
        evokeCallback(0.1);
    }

    public TMP_InputField updateAttributeValues_1;
    public TMP_InputField updateAttributeValues_2;
    public TMP_InputField updateAttributeValues_3;
    public TMP_InputField updateAttributeValues_4;
    public void updateAttributeValues()
    {
        StringBuilder objectClassName = new StringBuilder(updateAttributeValues_1.text, 100); //"HLAobjectRoot.ObjectClass0"
        StringBuilder ObjectInstanceName = new StringBuilder(updateAttributeValues_2.text, 100); //"Valve444"
        StringBuilder attributeName = new StringBuilder(updateAttributeValues_3.text, 100); //"Attribute0"
        StringBuilder value = new StringBuilder(updateAttributeValues_4.text, 100); //"Max54321"
        UpdateAttributeValues(objectClassName, objectClassName.Capacity, ObjectInstanceName, attributeName, value);
        string myString = objectClassName.ToString();
        Debug.Log("c# debug:" + "->UpdateAttributeValues: " + myString);
        evokeCallback(0.1);
    }


    public TMP_InputField getInteractionClassHandle_1;
    public void getInteractionClassHandle()
    {
        StringBuilder interactionClassName = new StringBuilder(getInteractionClassHandle_1.text, 100); //"HLAinteractionRoot.InteractionClass0"
        GetInteractionClassHandle(interactionClassName, interactionClassName.Capacity);
        Debug.Log("c# debug:" + "->GetInteractionClassHandle: " + interactionClassName.ToString());
        evokeCallback(0.1);
    }

    public TMP_InputField getParameterHandle_1;
    public TMP_InputField getParameterHandle_2;
    public void getParameterHandle()
    {
        StringBuilder interactionClassName = new StringBuilder(getParameterHandle_1.text, 100); //"HLAinteractionRoot.InteractionClass0"
        StringBuilder parametrName = new StringBuilder(getParameterHandle_2.text, 100); //"Parameter0"
        GetParameterHandle(interactionClassName, interactionClassName.Capacity, parametrName);
        Debug.Log("c# debug:" + "->GetParameterHandle: " + interactionClassName.ToString());
        evokeCallback(0.1);
    }

    public TMP_InputField subscribeInteractionClass_1;
    public void subscribeInteractionClass()
    {
        StringBuilder interactionClassName = new StringBuilder(subscribeInteractionClass_1.text, 100); //"HLAinteractionRoot.InteractionClass0"
        SubscribeInteractionClass(interactionClassName, interactionClassName.Capacity);
        Debug.Log("c# debug:" + "->SubscribeInteractionClass: " + interactionClassName.ToString());
        evokeCallback(0.1);
    }

    public TMP_InputField publishInteractionClass_1;
    public void publishInteractionClass()
    {
        StringBuilder interactionClassName = new StringBuilder(publishInteractionClass_1.text, 100); //"HLAinteractionRoot.InteractionClass0"
        PublishInteractionClass(interactionClassName, interactionClassName.Capacity);
        Debug.Log("c# debug:" + "->PublishInteractionClass: " + interactionClassName.ToString());
        evokeCallback(0.1);
    }

    public TMP_InputField sendInteraction_1;
    public TMP_InputField sendInteraction_2;
    public TMP_InputField sendInteraction_3;
    public void sendInteraction()
    {
        StringBuilder interactionClassName = new StringBuilder(sendInteraction_1.text, 100); //"HLAinteractionRoot.InteractionClass0"
        StringBuilder parametrName = new StringBuilder(sendInteraction_2.text, 100); //"Parameter0"
        StringBuilder parametrValue = new StringBuilder(sendInteraction_3.text, 100); //"900.123"
        SendInteraction(interactionClassName, interactionClassName.Capacity, parametrName, parametrValue);
        Debug.Log("c# debug:" + "->SendInteraction: " + interactionClassName.ToString());
        evokeCallback(0.1);
    }

    public void resignFederationExecution()
    {
        StringBuilder returnstring = new StringBuilder(100);
        ResignFederationExecution(returnstring, returnstring.Capacity);
        Debug.Log("c# debug:" + "->resignFederationExecution: " + returnstring.ToString());
    }
//--------------------------------------------------------------------------------






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

        RegisterRequestAttributeOwnershipAssumptionCallback(null);
        RegisterRequestDivestitureConfirmationCallback(null);
        RegisterAttributeOwnershipAcquisitionNotificationCallback(null);
        RegisterAttributeOwnershipUnavailableCallback(null);
        RegisterRequestAttributeOwnershipReleaseCallback(null);
        RegisterConfirmAttributeOwnershipAcquisitionCancellationCallback(null);
        RegisterInformAttributeOwnershipCallback(null);
        RegisterAttributeIsNotOwnedCallback(null);
        RegisterAttributeIsOwnedByRTICallback(null);
    }



    public void testV2()
    {
        
        

       

        
        
        
        
       
       

        /*
        {
            StringBuilder teststring = new StringBuilder("0123456789", 100);
            UnconditionalAttributeOwnershipDivestiture(teststring, teststring.Capacity, teststring, teststring);
            NegotiatedAttributeOwnershipDivestiture(teststring, teststring.Capacity, teststring, teststring);
            ConfirmDivestiture(teststring, teststring.Capacity, teststring, teststring);

            AttributeOwnershipAcquisition(teststring, teststring.Capacity);
            AttributeOwnershipAcquisitionIfAvailable(teststring, teststring.Capacity);
            AttributeOwnershipReleaseDenied(teststring, teststring.Capacity);
            AttributeOwnershipDivestitureIfWanted(teststring, teststring.Capacity);
            CancelNegotiatedAttributeOwnershipDivestiture(teststring, teststring.Capacity);
            CancelAttributeOwnershipAcquisition(teststring, teststring.Capacity);
            queryAttributeOwnership(teststring, teststring.Capacity);
            isAttributeOwnedByFederate(teststring, teststring.Capacity);
        }
        */
    }

























    /*
    public void  StepTest()
    {
        StringBuilder str2 = new StringBuilder( 100);
        TestInteraction(str2, str2.Capacity);
        string myString2 = str2.ToString();
        Debug.Log("c# debug:" + "TestInteraction: " + myString2);
        //text1.text = myString2;
    }

    public void  StepTestObjects()
    {
        StringBuilder str2 = new StringBuilder( 100);

        StringBuilder className = new StringBuilder(ObjectClassName.text, 100);
        StringBuilder attributeName = new StringBuilder(ObjectAttributeName.text, 100);
        StringBuilder objectInstanceName = new StringBuilder(ObjectInstanceName.text, 100);


        TestObjects(str2, str2.Capacity, className, attributeName, objectInstanceName);
        string myString2 = str2.ToString();
        Debug.Log("c# debug:" + "TestObjects: " + myString2);
       // text1.text = myString2;
    }


    public void StepSetValue ()
    {

        StringBuilder str2 = new StringBuilder(100);

        StringBuilder objectInstanceName = new StringBuilder(ObjectClassName.text, 100);
        StringBuilder attributeName = new StringBuilder(ObjectAttributeName.text, 100);
        StringBuilder attributeValue = new StringBuilder(ObjectAttributeValue.text, 100);

        SetValueAttributeObject(str2, str2.Capacity, objectInstanceName, attributeName, attributeValue);
        string myString2 = str2.ToString();
        Debug.Log("c# debug:" + "TestObjects: " + myString2);
       // text1.text = myString2;
    }
    */


    void Start()
    {
    }

        // Start is called before the first frame update
    public void StartThread()
    {
        allEvent = new List<int>();

        mutexObj = new Mutex();
        My_Thread = new Thread(new ThreadStart(Thread_loop));
        My_Thread.IsBackground = true;
        My_Thread.Start();

        // mutexObj.Dispose();
        // My_Thread.Abort();

    }

    // Update is called once per frame
    void Update()
    {
        return;

        GetReadedBytes();

        //List<bool> allEvent
        for (int i=0; i < allEvent.Count; i++)
        {
            int z = allEvent[i];
            if (ReceiveInteractionEvent != null)
            {
                ReceiveInteractionEvent.Invoke(z);
            }
        }
            
    }


    private void Thread_loop()
    {
        while (stop==false)
        {
            evokeCallback(0.1);
            getObjectClassHandle();
        }
    }

    public List<int> GetReadedBytes()
    {
        if (stop == true)
        {
            return new List<int>();
        }

        mutexObj.WaitOne();
        List<int> ReadedBytesDeepCopy = new List<int>();
        foreach (byte b in allEvent)
        {
            ReadedBytesDeepCopy.Add(b);
        }
        allEvent.Clear();
        mutexObj.ReleaseMutex();
        return ReadedBytesDeepCopy;
    }

}


