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
    //Все callback'и расположены в файле...
    //ieee1516\DLLtoUnity\RTI1516EAmbassadorLContent.h

    //имя класса для создаваемого объекта
    public TMP_InputField IP;

    public TextMeshProUGUI text1;
    public TMP_InputField FedarationName;
    public TMP_InputField FedarateName;

    //имя класса для создаваемого объекта
    public TMP_InputField ObjectClassName;
    //имя атрибута в классе
    public TMP_InputField ObjectAttributeName;
    //имя создаваемого экземпляра класса
    public TMP_InputField ObjectInstanceName;
    //новое значение атрибута
    public TMP_InputField ObjectAttributeValue;






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
        Debug.Log(debugString);
    }
    //end callback для Debug'а

    //callback для сообщений о потери соединения (connectionLost)
    private delegate void ConnectionLostCallback(IntPtr message, int size);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterConnectionLostCallback(ConnectionLostCallback callback);
    //Обработчик события потери соединения
    private static void ConnectionLost(IntPtr message, int size)
    {
        string debugString = Marshal.PtrToStringAnsi(message, size);
        Debug.Log(debugString);
    }
    //end callback для сообщений о потери соединения (connectionLost)


    //callback для сообщений информации о выполнении федерации (reportFederationExecutions)
    private delegate void reportFederationExecutionsCallback(IntPtr message1, IntPtr message2, int size1, int size2);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReportFederationExecutionsCallback(reportFederationExecutionsCallback callback);
    //обработчик 
    private static void ReportFederationExecutions(IntPtr message1, IntPtr message2, int size1, int size2)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
    }
    //end callback для сообщений информации о выполнении федерации (reportFederationExecutions)

    //callback для сообщений информации о успешности регистрации точки синхронизации (synchronizationPointRegistrationSucceeded)
    private delegate void synchronizationPointRegistrationSucceededCallback(IntPtr message1,  int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterSynchronizationPointRegistrationSucceededCallback(synchronizationPointRegistrationSucceededCallback callback);
    //Обработчик
    private static void SynchronizationPointRegistrationSucceeded(IntPtr message1,  int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //end callback для сообщений информации о успешности регистрации точки синхронизации (synchronizationPointRegistrationSucceeded)


    //callback для сообщений информации о провале регистрации точки синхронизации (synchronizationPointRegistrationFailedCallback)
    private delegate void synchronizationPointRegistrationFailedCallback(IntPtr message1, IntPtr message2, int size1, int size2);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterSynchronizationPointRegistrationFailedCallback(synchronizationPointRegistrationFailedCallback callback);
    //Обработчик
    private static void SynchronizationPointRegistrationFailed(IntPtr message1, IntPtr message2, int size1, int size2)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
    }
    //end callback для сообщений информации о провале регистрации точки синхронизации (synchronizationPointRegistrationFailedCallback)

    //callback для сообщений информации о установке синхронизации (announceSynchronizationPointCallback)
    private delegate void announceSynchronizationPointCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAnnounceSynchronizationPointCallback(announceSynchronizationPointCallback callback);
    //Обработчик
    private static void AnnounceSynchronizationPoint(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //end callback для сообщений информации о установке синхронизации (announceSynchronizationPointCallback)

    //callback для сообщений о факте того, что вся федерация синхронизирована (federationSynchronized)
    private delegate void federationSynchronizedCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterFederationSynchronizedCallback(federationSynchronizedCallback callback);
    //Обработчик
    private static void FederationSynchronized(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //end callback для сообщений о факте того, что вся федерация синхронизирована (federationSynchronized)

    ///////////////////////////////////////




    //callback для сообщений информации о резервировании имени объекта (objectInstanceNameReservationSucceeded)
    private delegate void objectInstanceNameReservationSucceededCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterObjectInstanceNameReservationSucceededCallback(objectInstanceNameReservationSucceededCallback callback);
    //Обработчик
    private static void ObjectInstanceNameReservationSucceeded(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //callback для сообщений информации о резервировании имени объекта (objectInstanceNameReservationSucceeded)

    //callback для сообщений информации о резервировании имени объекта (objectInstanceNameReservationFailed)
    private delegate void objectInstanceNameReservationFailedCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterObjectInstanceNameReservationFailed(objectInstanceNameReservationFailedCallback callback);
    //Обработчик
    private static void ObjectInstanceNameReservationFailed(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //callback для сообщений информации о резервировании имени объекта (objectInstanceNameReservationFailed)


    //callback для сообщений о создании нового объекта (discoverObjectInstance)
    private delegate void discoverObjectInstanceCallback(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterDiscoverObjectInstance(discoverObjectInstanceCallback callback);
    //Обработчик
    private static void DiscoverObjectInstance(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        string debugString3 = Marshal.PtrToStringAnsi(message3, size3);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
        Debug.Log(debugString3);
    }
    //callback для сообщений о создании нового объекта (discoverObjectInstance)

    //callback для сообщений о изменении атрибута (reflectAttributeValues)
    private delegate void reflectAttributeValuesCallback(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReflectAttributeValues(reflectAttributeValuesCallback callback);
    //Обработчик
    private static void ReflectAttributeValues(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        string debugString3 = Marshal.PtrToStringAnsi(message3, size3);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
        Debug.Log(debugString3);
    }
    //callback для сообщений о изменении атрибута (reflectAttributeValues)

    //callback для сообщений о получении интеракции (receiveInteraction)
    private delegate void receiveInteractionCallback(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterReceiveInteractionCallback(receiveInteractionCallback callback);
    //Обработчик
    private static void ReceiveInteraction(IntPtr message1, int size1, IntPtr message2, int size2, IntPtr message3, int size3)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        string debugString2 = Marshal.PtrToStringAnsi(message2, size2);
        string debugString3 = Marshal.PtrToStringAnsi(message3, size3);
        Debug.Log(debugString1);
        Debug.Log(debugString2);
        Debug.Log(debugString3);
    }
    //callback для сообщений о получении интеракции (receiveInteraction)


    //callback для сообщений о удалении объекта (removeObjectInstance)
    private delegate void removeObjectInstanceCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterRemoveObjectInstanceCallback(removeObjectInstanceCallback callback);
    //Обработчик
    private static void RemoveObjectInstance(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //callback для сообщений о удалении объекта (removeObjectInstance)


    //virtual void requestAttributeOwnershipAssumption(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& offeredAttributes,    rti1516e::VariableLengthData const& tag)
    private delegate void requestAttributeOwnershipAssumptionCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterRequestAttributeOwnershipAssumptionCallback(requestAttributeOwnershipAssumptionCallback callback);
    //Обработчик
    private static void RequestAttributeOwnershipAssumption(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //virtual void requestAttributeOwnershipAssumption

    //virtual void requestDivestitureConfirmation(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& releasedAttributes)
    private delegate void requestDivestitureConfirmationCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterRequestDivestitureConfirmationCallback(requestDivestitureConfirmationCallback callback);
    //Обработчик
    private static void RequestDivestitureConfirmation(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //virtual void requestDivestitureConfirmation

    //virtual void attributeOwnershipAcquisitionNotification(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& securedAttributes,    rti1516e::VariableLengthData const& tag)
    private delegate void attributeOwnershipAcquisitionNotificationCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAttributeOwnershipAcquisitionNotificationCallback(attributeOwnershipAcquisitionNotificationCallback callback);
    //Обработчик
    private static void AttributeOwnershipAcquisitionNotification(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //virtual void attributeOwnershipAcquisitionNotification

    //virtual void attributeOwnershipUnavailable(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& attributes)
    private delegate void attributeOwnershipUnavailableCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAttributeOwnershipUnavailableCallback(attributeOwnershipUnavailableCallback callback);
    //Обработчик
    private static void AttributeOwnershipUnavailable(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //virtual void attributeOwnershipUnavailable

    //virtual void requestAttributeOwnershipRelease(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& candidateAttributes,    rti1516e::VariableLengthData const& tag)
    private delegate void requestAttributeOwnershipReleaseCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterRequestAttributeOwnershipReleaseCallback(requestAttributeOwnershipReleaseCallback callback);
    //Обработчик
    private static void RequestAttributeOwnershipRelease(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //virtual void requestAttributeOwnershipRelease

    //virtual void confirmAttributeOwnershipAcquisitionCancellation(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandleSet const& attributes)
    private delegate void confirmAttributeOwnershipAcquisitionCancellationCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterConfirmAttributeOwnershipAcquisitionCancellationCallback(confirmAttributeOwnershipAcquisitionCancellationCallback callback);
    //Обработчик
    private static void ConfirmAttributeOwnershipAcquisitionCancellation(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //virtual void confirmAttributeOwnershipAcquisitionCancellation

    //virtual void informAttributeOwnership(rti1516e::ObjectInstanceHandle objectInstanceHandle,    rti1516e::AttributeHandle attribute,    rti1516e::FederateHandle owner)
    private delegate void informAttributeOwnershipCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterInformAttributeOwnershipCallback(informAttributeOwnershipCallback callback);
    //Обработчик
    private static void InformAttributeOwnership(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //virtual void informAttributeOwnership



    //virtual void attributeIsNotOwned(rti1516e::ObjectInstanceHandle objectInstanceHandle, rti1516e::AttributeHandle attribute)
    private delegate void attributeIsNotOwnedCallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAttributeIsNotOwnedCallback(attributeIsNotOwnedCallback callback);
    //Обработчик
    private static void AttributeIsNotOwned(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }
    //virtual void attributeIsNotOwned



    private delegate void attributeIsOwnedByRTICallback(IntPtr message1, int size1);
    [DllImport("DLLtoUnity", CallingConvention = CallingConvention.Cdecl)]
    private static extern void RegisterAttributeIsOwnedByRTICallback(attributeIsOwnedByRTICallback callback);
    //Обработчик
    private static void AttributeIsOwnedByRTI(IntPtr message1, int size1)
    {
        string debugString1 = Marshal.PtrToStringAnsi(message1, size1);
        Debug.Log(debugString1);
    }




    



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
        {
            StringBuilder className = new StringBuilder("HLAobjectRoot.ObjectClass0", 100);
            GetObjectClassHandle(className, className.Capacity);
            string myString = className.ToString();
            Debug.Log("->GetObjectClassHandle: " + myString);
            evokeCallback(0.1);
        }

        {
            StringBuilder className = new StringBuilder("HLAobjectRoot.ObjectClass0", 100);
            StringBuilder attributeName = new StringBuilder("Attribute0", 100);
            GetAttributeHandle(className, className.Capacity, attributeName);
            string myString = className.ToString();
            Debug.Log("->GetAttributeHandle: " + myString);
            evokeCallback(0.1);
        }

        {
            StringBuilder ObjectInstanceName = new StringBuilder("Valve124", 100);
            GetObjectInstanceHandle(ObjectInstanceName, ObjectInstanceName.Capacity);
            string myString = ObjectInstanceName.ToString();
            Debug.Log("->ObjectInstanceName: " + myString);
            evokeCallback(0.1);
        }

        {
            StringBuilder className = new StringBuilder("HLAobjectRoot.ObjectClass0", 100);
            StringBuilder attributeName = new StringBuilder("Attribute0", 100);
            PublishObjectClassAttributes(className, className.Capacity, attributeName);
            string myString = className.ToString();
            Debug.Log("->PublishObjectClassAttributes: " + myString);
            evokeCallback(0.1);
        }

        {
            StringBuilder className = new StringBuilder("HLAobjectRoot.ObjectClass0", 100);
            StringBuilder attributeName = new StringBuilder("Attribute0", 100);
            SubscribeObjectClassAttributes(className, className.Capacity, attributeName);
            string myString = className.ToString();
            Debug.Log("->SubscribeObjectClassAttributes: " + myString);
            evokeCallback(0.1);
        }

        {
            StringBuilder objectInstanceName = new StringBuilder("Valve444", 100);
            ReserveObjectInstanceName(objectInstanceName, objectInstanceName.Capacity);
            string myString = objectInstanceName.ToString();
            Debug.Log("->ReserveObjectInstanceName: " + myString);
            evokeCallback(0.1);
        }

        {
            StringBuilder objectClassName = new StringBuilder("HLAobjectRoot.ObjectClass0", 100);
            StringBuilder objectInstanceName = new StringBuilder("Valve444", 100);
            RegisterObjectInstance(objectClassName, objectClassName.Capacity, objectInstanceName);
            string myString = objectClassName.ToString();
            Debug.Log("->RegisterObjectInstance: " + myString);
            evokeCallback(0.1);
        }

        {
            StringBuilder objectClassName = new StringBuilder("HLAobjectRoot.ObjectClass0", 100);
            StringBuilder ObjectInstanceName = new StringBuilder("Valve444", 100);
            StringBuilder attributeName = new StringBuilder("Attribute0", 100);
            StringBuilder value = new StringBuilder("Max54321", 100);
            UpdateAttributeValues(objectClassName, objectClassName.Capacity, ObjectInstanceName, attributeName, value);
            string myString = objectClassName.ToString();
            Debug.Log("->UpdateAttributeValues: " + myString);
            evokeCallback(0.1);
        }

        {
            //GetInteractionClassHandle(StringBuilder interactionClassName, int length);
        }
        {
            //GetParameterHandle(StringBuilder interactionClassName, int length, StringBuilder parametrName);
        }
        {
            //SubscribeInteractionClass(StringBuilder interactionClassName, int length);
        }
        {
            //PublishInteractionClass(StringBuilder interactionClassName, int length);
        }
        {
            //SendInteraction(StringBuilder interactionClassName, int length, StringBuilder parametrNames, StringBuilder values);
        }
        {    
            //ResignFederationExecution(StringBuilder returnstring, int length);
        }
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


