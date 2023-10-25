/***************************************************************************
FirstDLL.cpp  -  IEEE1516-2010 (HLA Evolved) for Unity C#
used in company projects https://Lcontent.ru
based on OpenRTI
-------------------
begin                : 30 сентября 2022
copyright            : (C) 2022 by Гаммер Максим Дмитриевич (maximum2000)
email                : MaxGammer@gmail.com
***************************************************************************/

//https://stackoverflow.com/questions/49793560/build-c-plugin-for-unity
//https://github.com/onox/OpenRTI.git

//короче так.... читаем XML
//на каждый объект мы знаем rti1516e::ObjectClassHandle objectClassHandle = ambassador->getObjectClassHandle(className); //L"HLAobjectRoot.ObjectClass0"
//таким образом мы запоминаем соответствие имени из XML ("HLAobjectRoot.ObjectClass0") с полученыным дискриптором
//аналогично мы при публикации класса и подписывании на изменения значений атрибутов инстансов.... ambassador->getAttributeHandle(objectClassHandle, attributeName)); // L"Attribute0"
//мы заполняем соответствие имени атирибута из XML ("Attribute0") с полученным дискриптором
//теперь мы знаем соответствие
//а)Имени класса с дискриптором
//б)Имени аттрибута класса с дискриптором
//....далее в callback'е   discoverObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,rti1516e::ObjectClassHandle objectClassHandle,const std::wstring& objectInstanceName)
//    мы получаем дискриптор инстанса (objectInstanceHandle), имя созданного экземпляра объекта, дискриптор класса (objectClassHandle) и запоминаем связь
//теперь мы знаем соответствие
//д) Имени созданного объекта (экземпляра класса) с дискриптором объекта и дискриптором класса
//....далее в callback'е reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle, const rti1516e::AttributeHandleValueMap& attributeHandleValueMap ...)
//мы получаем значение изменившейся переменной и можем точно ее ассоциировать с дискриптором объекта (ObjectInstanceHandle) и дискриптором атрибута (AttributeHandleValueMap)
//+/- еще знаем какой федерат обновил ее (info.hasProducingFederate)

//+ разобратся с void unconditionalAttributeOwnershipDivestiture(const rti1516e::ObjectInstanceHandle& objectInstanceHandle,const rti1516e::AttributeHandleSet& attributeHandleSet) надо ее или так можем писать?
/*
    не реализованы:
    * ambassador.deleteObjectInstance(objectInstanceHandle, toVariableLengthData("tag"));
    * ambassador.unsubscribeObjectClass(subscribedObjectClass);
*/

//M:/GIT2/ieee1516/UnityTest/Assets/Plugins
//C:/WORK/OpenRTI_Unity/UnityTest/Assets/Plugins


#include "FirstDLL.h"


//---------------------------------------------------------------


#ifdef __cplusplus
extern "C"
#endif

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD  reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        //MessageBoxA(NULL, "DLL_PROCESS_ATTACH", "test", NULL);
        break;
    case DLL_THREAD_ATTACH:
        //MessageBoxA(NULL, "DLL_THREAD_ATTACH", "test", NULL);
        break;
    case DLL_THREAD_DETACH:
        //MessageBoxA(NULL, "DLL_THREAD_DETACH", "test", NULL);
        break;
    case DLL_PROCESS_DETACH:
        //MessageBoxA(NULL, "DLL_PROCESS_DETACH", "test", NULL);
        break;
    }
    return TRUE;
}

//------------------------[    ПОДКЛЮЧЕНИЕ    ]----------------------------------------------------------

//подключиться к серверу федерации
int MyConnect(std::wstring IP)
{
    IP = L"";
    LastErrorString = L"";
    FederationName = L"";
    FederateName = L"";

    try
    {
        ambassador->connect(L"rti://127.0.0.1"); //thread:// L"rti://127.0.0.1"
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << LastErrorString << L"\"" << std::endl;
        return 1;
    }
    catch (...) 
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << LastErrorString << std::endl;
        return 1;
    }
    return 0;
}
//первичное подключение к серверу (RTInode)
int Connect(char* myString, int length)
{
    std::wstringstream cls1;
    cls1 << myString;
    IP  = cls1.str();

    //OpenRTI::RTI1516ESimpleAmbassador ambassador;
    ambassador = new OpenRTI::RTI1516EAmbassadorLContent();
    ambassador->setUseDataUrlObjectModels(false);

    /*
    When I call "connect" function with computer name as its input (i.e ambassador->connect("rti://rtinodeSystem")) the federate can join to rti(rtinode in TCP mode),
    but if I replace computer name with its IP (i.e ambassador->connect("rti://192.168.1.4")) it cannot. Why this happen?
    Is the syntax when we use the IP address something else(I am sure that the "rtinodeSystem"s IP is "192.168.1.4")?
    Maybe  turn off IPV6_V6ONLY ?
    */

    int ret = MyConnect(IP);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }

    //std::cout << "Max Gammer Test 2" << std::endl;
    std::string s("ok");
    strcpy_s(myString, length, s.c_str());
    return 0;
}

//создать федерат
int MyCreateFederationExecution(std::wstring name, std::wstring filename)
{
    LastErrorString = L"";
    FederationName = name;

    try
    {
        ambassador->createFederationExecution(FederationName, filename); // L"M:/GIT2/ieee1516/UnityTest/Assets/Plugins/fdd_test.xml"
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << LastErrorString << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << LastErrorString << std::endl;
        return 1;
    }
    return 0;
}
//создание федерации
int CreateFederationExecution(char* myString1, int length1, char* myString2)
{
    std::wstringstream cls1;
    cls1 << myString1;
    std::wstring name = cls1.str();

    std::wstringstream cls2;
    cls2 << myString2;
    std::wstring file = cls2.str();

    ambassador->SendLog(L"DEBUG: CreateFederationExecution:IP=" + IP, 0);
    ambassador->SendLog(L"DEBUG: CreateFederationExecution:name=" + name, 0);

    // create, must work
    int ret = MyCreateFederationExecution(name, file);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString1, length1, s.c_str());
        return 1;
    }
    std::string s("ok");
    strcpy_s(myString1, length1, s.c_str());
    return 0;
}

//присоедениться к федерации
int MyJoinFederationExecution(std::wstring name)
{
    LastErrorString = L"";
    FederateName = name;

    try
    {
        //подключаемс к федерации TEST1 как федерат с именем "federate"
        ambassador->joinFederationExecution(FederateName,FederationName );
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << LastErrorString << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << LastErrorString << std::endl;
        return 1;
    }
    return 0;
}
//подключение нового федерата
int JoinFederationExecution(char* myString, int length)
{
    std::wstringstream cls;
    cls << myString;
    std::wstring name = cls.str();

    ambassador->SendLog(L"DEBUG: JoinFederationExecution:name=" + name, 0);

    // join must work
    int ret = MyJoinFederationExecution(name);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }
    std::string s("ok");
    strcpy_s(myString, length, s.c_str());
    return 0;
}

//отправить сообщение о получении списка федератов
int MyListFederationExecutions()
{
    LastErrorString = L"";

    try
    {
        ambassador->mylistFederationExecutions();
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << LastErrorString << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << LastErrorString << std::endl;
        return 1;
    }
    return 0;
}
//отправить сообщение о получении списка федератов, ответ будет в callback'е reportFederationExecutions...
int ListFederationExecutions(char* myString, int length)
{
    ambassador->evokeCallback(1.0);
    int ret = MyListFederationExecutions();

    ambassador->SendLog(L"DEBUG: ListFederationExecutions call", 0);

    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }
    std::string s("ok");
    strcpy_s(myString, length, s.c_str());
    return 0;
}

//ambassador->resignFederationExecution(rti1516e::CANCEL_THEN_DELETE_THEN_DIVEST);
//ambassador->destroyFederationExecution(getFederationExecution());


//------------------------[    СИНХРОНИЗАЦИЯ    ]----------------------------------------------------------

//Зарегистрировать точку синхронизации (результат узнать в GetSynchronizationPointFlag)
int MyRegisterFederationSynchronizationPoint()
{
    LastErrorString = L"";

    try
    {
        ambassador->registerFederationSynchronizationPoint(L"PPP", rti1516e::VariableLengthData());
        
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << LastErrorString << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << LastErrorString << std::endl;
        return 1;
    }
    return 0;
}

//Зарегистрировать точку синхронизации (результат узнать в GetSynchronizationPointFlag)
int RegisterFederationSynchronizationPoint(char* myString, int length)
{
    int ret = MyRegisterFederationSynchronizationPoint();
    ambassador->SendLog(L"DEBUG: RegisterFederationSynchronizationPoint call", 0);

    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }
    std::string s("ok");
    strcpy_s(myString, length, s.c_str());
    return 0;
}

//отправить сообщение о установлении точки синхронизации
int MySynchronizationPointAchieved()
{
    LastErrorString = L"";

    try
    {
        ambassador->synchronizationPointAchieved(L"PPP");
        ambassador->SendLog(L"DEBUG: synchronizationPointAchieved call", 0);
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << LastErrorString << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << LastErrorString << std::endl;
        return 1;
    }
    return 0;
}

//отправить сообщение о установлении точки синхронизации
int SynchronizationPointAchieved(char* myString, int length)
{
    ambassador->evokeCallback(1.0);
    int ret = MySynchronizationPointAchieved();
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }
    std::string s("ok");
    strcpy_s(myString, length, s.c_str());
    return 0;
}

//----------------------------------------------------------------------------------




//обновить федерат
int evokeCallback(double dT)
{
    ambassador->evokeCallback(dT);
    ambassador->SendLog(L"DEBUG: evokeCallback call", 0);
    return 0;
}

//сервисные функции---------------------------------------------
inline rti1516e::VariableLengthData toVariableLengthData(const char* s)
{
    rti1516e::VariableLengthData variableLengthData;
    if (s)
    {
        variableLengthData.setData(s, (unsigned long)strlen(s));
    }
    return variableLengthData;
}

inline rti1516e::VariableLengthData toVariableLengthData(const OpenRTI::Clock& c)
{
    // May be at some time make this endian safe
    rti1516e::VariableLengthData variableLengthData;
    variableLengthData.setData(&c, (unsigned long)sizeof(c));
    return variableLengthData;
}

inline rti1516e::VariableLengthData toVariableLengthData(const std::wstring& s)
{
    rti1516e::VariableLengthData variableLengthData;
    variableLengthData.setData(s.data(), (unsigned long)(sizeof(std::wstring::value_type) * s.size()));
    return variableLengthData;
}


//------------------------------NEW------------------------------------------------


//Получить хендл класса по имени (ambassador->getObjectClassHandle(L"HLAobjectRoot.ObjectClass0"))
rti1516e::ObjectClassHandle TempObjectClassHandle;
int MyGetObjectClassHandle(std::wstring className)
{
    LastErrorString = L"";
    //получение класса объекта
    try
    {
        TempObjectClassHandle = ambassador->getObjectClassHandle(className); //L"HLAobjectRoot.ObjectClass0"
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
    return 0;
}
int GetObjectClassHandle(char* myString, int length)
{
    std::wstringstream cls;
    cls << myString;
    std::wstring className = cls.str();
    ambassador->SendLog(L"DEBUG: GetObjectClassHandle:name=" + className, 0);
    int ret = MyGetObjectClassHandle(className);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }
    std::wstring classHandleW = TempObjectClassHandle.toString();
    std::string s(classHandleW.begin(), classHandleW.end());
    strcpy_s(myString, length, s.c_str());
    return 0;
}


//Получить хендл атрибута по имени (ambassador->getAttributeHandle(objectClassHandle, L"Attribute0"))
rti1516e::AttributeHandle TempAttribute;
int MyGetAttributeHandle(std::wstring className, std::wstring attributeName)
{
    LastErrorString = L"";
    //получение класса объекта
    try
    {
        TempAttribute = ambassador->getAttributeHandle(ambassador->getObjectClassHandle(className), attributeName); // L"Attribute0"
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
    return 0;
}
int GetAttributeHandle(char* myString1, int length1, char* myString2)
{
    std::wstringstream cls1;
    cls1 << myString1;
    std::wstring className = cls1.str();
    std::wstringstream cls2;
    cls2 << myString2;
    std::wstring attributeName = cls2.str();
    ambassador->SendLog(L"DEBUG: GetAttributeHandle:className=" + className + L" attributeName:" + attributeName, 0);
    int ret = MyGetAttributeHandle(className, attributeName);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString1, length1, s.c_str());
        return 1;
    }
    std::wstring attributeHandleW = TempAttribute.toString();
    std::string s(attributeHandleW.begin(), attributeHandleW.end());
    strcpy_s(myString1, length1, s.c_str());
    return 0;
}


//Получить хендл экземпляра объекта по имени  getObjectInstanceHandle
rti1516e::ObjectInstanceHandle TempObjectInstanceHandle;
int MyGetObjectInstanceHandle(std::wstring ObjectInstanceName)
{
    LastErrorString = L"";
    //получение класса объекта
    try
    {
        TempObjectInstanceHandle = ambassador->getObjectInstanceHandle(ObjectInstanceName); // L"Valve12"
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
    return 0;
}
int GetObjectInstanceHandle(char* myString1, int length1)
{
    std::wstringstream cls1;
    cls1 << myString1;
    std::wstring objectInstanceName = cls1.str();
    
    ambassador->SendLog(L"DEBUG: GetObjectInstanceHandle:objectInstanceName=" + objectInstanceName, 0);
    int ret = MyGetObjectInstanceHandle(objectInstanceName);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString1, length1, s.c_str());
        return 1;
    }
    std::wstring objectInstanceNameW = TempObjectInstanceHandle.toString();
    std::string s(objectInstanceNameW.begin(), objectInstanceNameW.end());
    strcpy_s(myString1, length1, s.c_str());
    return 0;
}


//!!!!! Через запятую
//опубликовать объект и атрибуты (через запятую) (ambassador->publishObjectClassAttributes (har* myString1, int length1, char* myString2, int length2))
int MyPublishObjectClassAttributes(std::wstring className, std::wstring attributeName)
{
    LastErrorString = L"";
    //хранение дискриптора аттрибутов
    rti1516e::AttributeHandleSet _attributes;
    //получение класса объекта
    try
    {
        rti1516e::ObjectClassHandle _ObjectClassHandle = ambassador->getObjectClassHandle(className);
        rti1516e::AttributeHandle newattribute = ambassador->getAttributeHandle(_ObjectClassHandle, attributeName); // L"Attribute0"
        _attributes.insert(newattribute);
        ambassador->publishObjectClassAttributes(_ObjectClassHandle, _attributes);
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
    return 0;
}
int PublishObjectClassAttributes(char* myString1, int length1, char* myString2)
{
    std::wstringstream cls1;
    cls1 << myString1;
    std::wstring className = cls1.str();
    std::wstringstream cls2;
    cls2 << myString2;
    std::wstring attributeName = cls2.str();
    ambassador->SendLog(L"DEBUG: PublishObjectClassAttributes:className=" + className + L" attributeName:" + attributeName, 0);
    int ret = MyPublishObjectClassAttributes(className, attributeName);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString1, length1, s.c_str());
        return 1;
    }
    std::string s("ok");;
    strcpy_s(myString1, length1, s.c_str());
    return 0;
}


//!!!!! Через запятую
//подписка на получение событий этого класса объекта ( ambassador->subscribeObjectClassAttributes(objectClassHandle, attributes))
int MySubscribeObjectClassAttributes(std::wstring className, std::wstring attributeName)
{
    LastErrorString = L"";
    //хранение дискриптора аттрибутов
    rti1516e::AttributeHandleSet _attributes;
    //получение класса объекта
    try
    {
        rti1516e::ObjectClassHandle _ObjectClassHandle = ambassador->getObjectClassHandle(className);
        rti1516e::AttributeHandle newattribute = ambassador->getAttributeHandle(_ObjectClassHandle, attributeName); // L"Attribute0"
        _attributes.insert(newattribute);
        ambassador->subscribeObjectClassAttributes(_ObjectClassHandle, _attributes);
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
    return 0;
}
int SubscribeObjectClassAttributes(char* myString1, int length1, char* myString2)
{
    std::wstringstream cls1;
    cls1 << myString1;
    std::wstring className = cls1.str();
    std::wstringstream cls2;
    cls2 << myString2;
    std::wstring attributeName = cls2.str();
    ambassador->SendLog(L"DEBUG: SubscribeObjectClassAttributes:className=" + className + L" attributeName:" + attributeName, 0);
    int ret = MyPublishObjectClassAttributes(className, attributeName);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString1, length1, s.c_str());
        return 1;
    }
    std::string s("ok");;
    strcpy_s(myString1, length1, s.c_str());
    return 0;
}


//зарегистрировать имя  (ambassador->reserveObjectInstanceName(L"objectInstanceName1"))
int MyReserveObjectInstanceName(std::wstring objectInstanceName)
{
    LastErrorString = L"";
    try
    {
        ambassador->reserveObjectInstanceName(objectInstanceName);
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
    return 0;
}
int ReserveObjectInstanceName(char* myString1, int length1)
{
    std::wstringstream cls1;
    cls1 << myString1;
    std::wstring objectInstanceName = cls1.str();

    ambassador->SendLog(L"DEBUG: ReserveObjectInstanceName:objectInstanceName=" + objectInstanceName, 0);
    int ret = MyReserveObjectInstanceName(objectInstanceName);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString1, length1, s.c_str());
        return 1;
    }
    std::string s("ok");;
    strcpy_s(myString1, length1, s.c_str());
    return 0;
}

//создание и регистрация нового экземпляра объекта класса (objectInstanceHandle = ambassador->registerObjectInstance(objectClassHandle, L"objectInstanceName1")) +-возврат хендла?
rti1516e::ObjectInstanceHandle ObjectInstanceHandleForMyRegisterObjectInstance;
int MyRegisterObjectInstance(std::wstring objectClassName, std::wstring objectInstanceName)
{
    LastErrorString = L"";
    //хранение дискриптора аттрибутов
    rti1516e::AttributeHandleSet _attributes;
    //получение класса объекта
    try
    {
        rti1516e::ObjectClassHandle _ObjectClassHandle = ambassador->getObjectClassHandle(objectClassName);
        ObjectInstanceHandleForMyRegisterObjectInstance = ambassador->registerObjectInstance(_ObjectClassHandle, objectInstanceName);
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
    return 0;
}
int RegisterObjectInstance(char* myString1, int length1, char* myString2)
{
    std::wstringstream cls1;
    cls1 << myString1;
    std::wstring className = cls1.str();
    std::wstringstream cls2;
    cls2 << myString2;
    std::wstring objectInstanceName = cls2.str();
    ambassador->SendLog(L"DEBUG: MyRegisterObjectInstance:className=" + className + L" objectInstanceName:" + objectInstanceName, 0);
    int ret = MyRegisterObjectInstance(className, objectInstanceName);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString1, length1, s.c_str());
        return 1;
    }
    std::wstring objectInstanceW = ObjectInstanceHandleForMyRegisterObjectInstance.toString();
    std::string s(objectInstanceW.begin(), objectInstanceW.end());
    strcpy_s(myString1, length1, s.c_str());
    return 0;
}

//изменение значений атрибутов экземпляра объекта (ambassador->updateAttributeValues(objectInstanceHandle, attributeValues, tag))
int MyUpdateAttributeValues(std::wstring ClassName, std::wstring ObjectInstanceName, std::wstring attributeName, std::wstring value)
{
    LastErrorString = L"";
    
    try
    {
        rti1516e::ObjectClassHandle _objectClassHandle = ambassador->getObjectClassHandle(ClassName);
        rti1516e::AttributeHandleSet _attributes;
        rti1516e::AttributeHandle newattribute = ambassador->getAttributeHandle(_objectClassHandle, attributeName); // L"Attribute0"
        _attributes.insert(newattribute);
        rti1516e::AttributeHandleValueMap attributeValues;
        rti1516e::VariableLengthData tag = toVariableLengthData(OpenRTI::Clock::now());
        for (rti1516e::AttributeHandleSet::const_iterator k = _attributes.begin(); k != _attributes.end(); ++k)
        {
            //attributeValues[*k] = toVariableLengthData(ambassador->getAttributeName(_objectClassHandle, *k));
            attributeValues[*k] = toVariableLengthData(value); //L"Xbc1234"
        }
        rti1516e::ObjectInstanceHandle _objectInstanceHandle = ambassador->getObjectInstanceHandle(ObjectInstanceName);
        ambassador->updateAttributeValues(_objectInstanceHandle, attributeValues, tag);
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
    return 0;
}
int UpdateAttributeValues(char* myString1, int length1, char* myString2, char* myString3, char* myString4)
{
    std::wstringstream cls1;
    cls1 << myString1;
    std::wstring className = cls1.str();
    std::wstringstream cls2;
    cls2 << myString2;
    std::wstring objectInstanceName = cls2.str();
    std::wstringstream cls3;
    cls3 << myString3;
    std::wstring attributeName = cls3.str();
    std::wstringstream cls4;
    cls4 << myString4;
    std::wstring attributeValue = cls4.str();

    ambassador->SendLog(L"DEBUG: UpdateAttributeValues:" + className + L"." +  objectInstanceName + L"." + attributeName + L"=" + attributeValue, 0);
    int ret = MyUpdateAttributeValues(className, objectInstanceName, attributeName, attributeValue);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString1, length1, s.c_str());
        return 1;
    }
    std::string s("ok");;
    strcpy_s(myString1, length1, s.c_str());
    return 0;
}






















//получение класса интеракции (InteractionClass0Handle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0"))
int GetInteractionClassHandle(char* myString1, int length1)
{
    return 0;
}

//получение класса параметров интеракции (class0Parameter0Handle = ambassador->getParameterHandle(InteractionClass0Handle, L"Parameter0"))
int GetParameterHandle(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//подписка на эту интеракцию (ambassador->subscribeInteractionClass(InteractionClass0Handle))
int SubscribeInteractionClass(char* myString1, int length1)
{
    return 0;
}

//публикация интеракции (ambassador->publishInteractionClass(InteractionClass0Handle))
int PublishInteractionClass(char* myString1, int length1)
{
    return 0;
}

//отправить интеракцию ( ambassador->sendInteraction(InteractionClass0Handle, parameterValues, ambassador->getFederateHandle().encode())
int SendInteraction(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//выйти из федерации resignFederationExecution(rti1516e::ResignAction resignAction)
int ResignFederationExecution(char* myString1, int length1)
{
    return 0;
}

//сообщаем о желании безусловной отдачи владения другому федерату (толкаем-отдаем права) - unconditionalAttributeOwnershipDivestiture
int UnconditionalAttributeOwnershipDivestiture(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//сообщаем о желании согласованной отдачи владения другому федерату (толкаем-отдаем права) - negotiatedAttributeOwnershipDivestiture
int NegotiatedAttributeOwnershipDivestiture(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//подтверждаем отдачу владения (толкаем-отдаем права) - confirmDivestiture
int ConfirmDivestiture(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//Навязчиво просим отдать права нам (тянем-требуем права) - attributeOwnershipAcquisition
int AttributeOwnershipAcquisition(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//Пытаемся спросить отдать права нам, если сейчас они никому не пренадлежат, если комуто принадлежат надо навязчиво требовать (тянем права) - attributeOwnershipAcquisitionIfAvailable
int AttributeOwnershipAcquisitionIfAvailable(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//отказать в передаче владения, даже если нас навязчиво попросили - attributeOwnershipReleaseDenied
DLLExport int AttributeOwnershipReleaseDenied(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//разрешить передачу владения - attributeOwnershipDivestitureIfWanted
int AttributeOwnershipDivestitureIfWanted(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//прекратить согласование передачи прав - cancelNegotiatedAttributeOwnershipDivestiture
int CancelNegotiatedAttributeOwnershipDivestiture(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//cancelAttributeOwnershipAcquisition
int CancelAttributeOwnershipAcquisition(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//видимо узнать кому сейчас паринадлежат права - queryAttributeOwnership
int queryAttributeOwnership(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}

//видимо узнать, нам сейчас сейчас паринадлежат права или нет - isAttributeOwnedByFederate
int isAttributeOwnedByFederate(char* myString1, int length1, char* myString2, int length2)
{
    return 0;
}





//-----------------------------OLD----------------------------------------------

//------------------[Тест интеракций]--------------------------
//Тест интеракций
int MyTestInteraction()
{
    LastErrorString = L"";

    //хранение класса интеракции и ее параметров
    rti1516e::InteractionClassHandle InteractionClass0Handle;
    rti1516e::ParameterHandle class0Parameter0Handle;

    //получение класса интеракции
    try
    {
        InteractionClass0Handle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0");
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }

    //получение класса параметров интеракции
    try
    {
        class0Parameter0Handle = ambassador->getParameterHandle(InteractionClass0Handle, L"Parameter0");
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...) 
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }

    //подписка на эту интеракцию
    try 
    {
        ambassador->subscribeInteractionClass(InteractionClass0Handle);
    }
    catch (const rti1516e::Exception& e) 
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...) 
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }

    //публикация интеракции
    try 
    {
        ambassador->publishInteractionClass(InteractionClass0Handle);
    }
    catch (const rti1516e::Exception& e) 
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...) 
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }

    //
    ambassador->evokeCallback(1.0);

    //задать значения параметрам интеракции
    rti1516e::ParameterHandleValueMap parameterValues;
    parameterValues[class0Parameter0Handle] = toVariableLengthData("parameter0text");
    //parameterValues[class1Parameter1Handle] = toVariableLengthData("parameter1");

    //отправить интеракцию
    try 
    {
        ambassador->sendInteraction(InteractionClass0Handle, parameterValues, ambassador->getFederateHandle().encode());
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }

    /*
   не реализованы:
   * отписка от интеракции;
   * отмена регистрации интеракции;
   * информация есть в файле примеров interactions / OpenRTI
   */

    return 0;
}
//Тест интеракций
int TestInteraction(char* myString, int length)
{
    //std::wstringstream cls;
    //cls << myString;
    //std::wstring name = cls.str();

    //ambassador->SendLog(name, 0);
    ambassador->SendLog(L"DEBUG: TestInteraction call", 0);

    // join must work
    int ret = MyTestInteraction();
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }
    std::string s("ok");
    strcpy_s(myString, length, s.c_str());
    return 0;
}
//--------------------------------------------------------

//хранение дискриптора класса объекта и его аттрибутов
rti1516e::ObjectClassHandle objectClassHandle;
//хранение дискриптора аттрибутов
rti1516e::AttributeHandleSet attributes;
//хранение дискриптора экземпляра класса объекта
rti1516e::ObjectInstanceHandle objectInstanceHandle;

//----------------[Тест объектов]----------------------------------------
//Тест объектов
int MyTestObjects(std::wstring className, std::wstring attributeName, std::wstring objectInstanceName)
{
    LastErrorString = L"";

    //хранение дискриптора класса объекта и его аттрибутов
    //rti1516e::ObjectClassHandle objectClassHandle;
    //хранение дискриптора аттрибутов
    //rti1516e::AttributeHandleSet attributes;
    //хранение дискриптора экземпляра класса объекта
    //rti1516e::ObjectInstanceHandle objectInstanceHandle;

    //получение класса объекта
    try 
    {
        objectClassHandle = ambassador->getObjectClassHandle(className); //L"HLAobjectRoot.ObjectClass0"
        //запоминаем дескриптор класса объекта
        //objectClassHandle;
        //запоминаем имя класса
        //className;
    } 
    catch (const rti1516e::Exception& e) 
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    } 
    catch (...) 
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }

    //публикация класса объекта и атрибутов объекта
    try 
    {
        rti1516e::AttributeHandle newattribute = ambassador->getAttributeHandle(objectClassHandle, attributeName); // L"Attribute0"
        attributes.insert(newattribute); 
            //запоминаем имя-данные о атрибуте
            //attribute.handle = newattribute;
        ambassador->publishObjectClassAttributes(objectClassHandle, attributes);
    } 
    catch (const rti1516e::Exception& e) 
    {
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    } 
    catch (...) 
    {
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
    

    //подписка на получение событий этого класса объекта
    try 
    {
        ambassador->subscribeObjectClassAttributes(objectClassHandle, attributes);
    } 
    catch (const rti1516e::Exception& e) 
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    } 
    catch (...) 
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }


    //если хотим зарегистрировать имя, тогда ....
    try 
    {
        ambassador->reserveObjectInstanceName(objectInstanceName); //L"objectInstanceName1"
    }
    catch (const rti1516e::Exception& e) 
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...) 
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }

    //
    ambassador->evokeCallback(1.0);
    //

    //создание и регистрация нового экземпляра объекта класса
    try 
    {
        //или регистрируем без имени... ambassador->registerObjectInstance(objectClassHandle)
        //или с именем, но тогда 
        //std::wstring objectInstanceName = L"objectInstanceName1";
        objectInstanceHandle = ambassador->registerObjectInstance(objectClassHandle, objectInstanceName);
            //запоминаем дескриптор экземпляра класса (объекта)
            //temp.objectInstanceHandle = objectInstanceHandle;
            //
    } 
    catch (const rti1516e::Exception& e) 
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    } 
    catch (...) 
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }

    //
    ambassador->evokeCallback(1.0);

    //изменение значений атрибутов экземпляра
    try 
    {
        rti1516e::AttributeHandleValueMap attributeValues;
        rti1516e::VariableLengthData tag = toVariableLengthData(OpenRTI::Clock::now());
        for (rti1516e::AttributeHandleSet::const_iterator k = attributes.begin(); k != attributes.end(); ++k)
        {
            //attributeValues[*k] = toVariableLengthData(ambassador->getAttributeName(objectClassHandle, *k));
            attributeValues[*k] = toVariableLengthData(L"Xbc1234");
        }
        ambassador->updateAttributeValues(objectInstanceHandle, attributeValues, tag);
    } 
    catch (const rti1516e::Exception& e) 
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    } 
    catch (...) 
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }
   
    return 0;

}

//Тест объектов
int TestObjects(char* myString, int length, char* _className, char* _attributeName, char* _objectInstanceName)
{
    //имя класса из XML .. L"HLAobjectRoot.ObjectClass0"
    std::wstringstream cls1;
    cls1 << _className;
    std::wstring className = cls1.str();
    //имя аттрибута из XML .. L"Attribute0"
    std::wstringstream cls2;
    cls2 << _attributeName;
    std::wstring attributeName = cls2.str();
    //имя экземпляра класса (объекта) .. L"objectInstanceName1"
    std::wstringstream cls3;
    cls3 << _objectInstanceName;
    std::wstring objectInstanceName = cls3.str();



    //ambassador->SendLog(name, 0);
    ambassador->SendLog(L"DEBUG: TestObjects call", 0);
    ambassador->SendLog(L"DEBUG: TestObjects:className=" + className, 0);
    ambassador->SendLog(L"DEBUG: TestObjects:attributeName=" + attributeName, 0);
    ambassador->SendLog(L"DEBUG: TestObjects:objectInstanceName=" + objectInstanceName, 0);

    // join must work
    int ret = MyTestObjects(className, attributeName, objectInstanceName);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }
    std::string s("ok");
    strcpy_s(myString, length, s.c_str());
    return 0;
}


//-------------------------------------------------------
//Изменение значений атриубута 
int MySetValueAttributeObject(std::wstring objectInstanceName, std::wstring attributeName, std::wstring attributeValue)
{
    ambassador->evokeCallback(1.0);

    //изменение значений атрибутов экземпляра
    try
    {
        rti1516e::AttributeHandleValueMap attributeValues;
        rti1516e::VariableLengthData tag = toVariableLengthData(OpenRTI::Clock::now());
        for (rti1516e::AttributeHandleSet::const_iterator k = attributes.begin(); k != attributes.end(); ++k)
        {
            //attributeValues[*k] = toVariableLengthData(ambassador->getAttributeName(objectClassHandle, *k));
            attributeValues[*k] = toVariableLengthData(attributeValue);
        }
        ambassador->updateAttributeValues(objectInstanceHandle, attributeValues, tag);
    }
    catch (const rti1516e::Exception& e)
    {
        LastErrorString = e.what();
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return 1;
    }
    catch (...)
    {
        LastErrorString = L"Unknown Exception!";
        std::wcout << L"Unknown Exception!" << std::endl;
        return 1;
    }

    return 0;
}

int SetValueAttributeObject(char* myString, int length, char* _objectInstanceName , char* _attributeName, char* _attributeValue)
{
    
    //имя экземпляра класса (объекта) .. L"objectInstanceName1"
    std::wstringstream cls3;
    cls3 << _objectInstanceName;
    std::wstring objectInstanceName = cls3.str();
    //имя аттрибута из XML .. L"Attribute0"
    std::wstringstream cls2;
    cls2 << _attributeName;
    std::wstring attributeName = cls2.str();
    //изначение
    std::wstringstream cls4;
    cls4 << _attributeValue;
    std::wstring attributeValue = cls4.str();
    
    //ambassador->SendLog(name, 0);
    ambassador->SendLog(L"DEBUG: SetValueAttributeObject call", 0);
    ambassador->SendLog(L"DEBUG: SetValueAttributeObject:objectInstanceName=" + objectInstanceName, 0);
    ambassador->SendLog(L"DEBUG: SetValueAttributeObject:attributeName=" + attributeName, 0);
    ambassador->SendLog(L"DEBUG: SetValueAttributeObject:attributeValue=" + attributeValue, 0);


    // join must work
    int ret = MySetValueAttributeObject(objectInstanceName, attributeName, attributeValue);
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }
    std::string s("ok");
    strcpy_s(myString, length, s.c_str());
    return 0;
}






//--------------------------------------------------------


/*
https://www.nongnu.org/certi/PyHLA/manual/node16.html
https://www.cs.cmu.edu/afs/cs/academic/class/15413-s99/www/hla/doc/rti_synopsis/09-Ownership_Management/Ownership_Management.html


//сообщаем о желании безусловной отдачи владения другому федерату (толкаем-отдаем права)
void
RTIambassadorImplementation::unconditionalAttributeOwnershipDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                                        rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::AttributeNotDefined,
         rti1516e::AttributeNotOwned,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandleVector attributeHandleVector(rti1516AttributeHandleSet);
    _ambassadorInterface->unconditionalAttributeOwnershipDivestiture(objectInstanceHandle, attributeHandleVector);
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotOwned& e) {
    throw rti1516e::AttributeNotOwned(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//сообщаем о желании согласованной отдачи владения другому федерату (толкаем-отдаем права)
void
RTIambassadorImplementation::negotiatedAttributeOwnershipDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                                     rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                                                     rti1516e::VariableLengthData const & rti1516Tag)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::AttributeNotDefined,
         rti1516e::AttributeNotOwned,
         rti1516e::AttributeAlreadyBeingDivested,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandleVector attributeHandleVector(rti1516AttributeHandleSet);
    OpenRTI::_I1516EVariableLengthData tag(rti1516Tag);
    _ambassadorInterface->negotiatedAttributeOwnershipDivestiture(objectInstanceHandle, attributeHandleVector, tag);
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotOwned& e) {
    throw rti1516e::AttributeNotOwned(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeAlreadyBeingDivested& e) {
    throw rti1516e::AttributeAlreadyBeingDivested(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//подтверждаем отдачу владения (толкаем-отдаем права)
void
RTIambassadorImplementation::confirmDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                rti1516e::AttributeHandleSet const& rti1516AttributeHandleSet,
                                                rti1516e::VariableLengthData const& rti1516Tag)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::AttributeNotDefined,
         rti1516e::AttributeNotOwned,
         rti1516e::AttributeDivestitureWasNotRequested,
         rti1516e::NoAcquisitionPending,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandleVector attributeHandleVector(rti1516AttributeHandleSet);
    OpenRTI::_I1516EVariableLengthData tag(rti1516Tag);
    _ambassadorInterface->confirmDivestiture(objectInstanceHandle, attributeHandleVector, tag);
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotOwned& e) {
    throw rti1516e::AttributeNotOwned(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeDivestitureWasNotRequested& e) {
    throw rti1516e::AttributeDivestitureWasNotRequested(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NoAcquisitionPending& e) {
    throw rti1516e::NoAcquisitionPending(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//Навязчиво просим отдать права нам (тянем-требуем права)
void
RTIambassadorImplementation::attributeOwnershipAcquisition(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                           rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                                           rti1516e::VariableLengthData const & rti1516Tag)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::ObjectClassNotPublished,
         rti1516e::AttributeNotDefined,
         rti1516e::AttributeNotPublished,
         rti1516e::FederateOwnsAttributes,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandleVector attributeHandleVector(rti1516AttributeHandleSet);
    OpenRTI::_I1516EVariableLengthData tag(rti1516Tag);
    _ambassadorInterface->attributeOwnershipAcquisition(objectInstanceHandle, attributeHandleVector, tag);
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::ObjectClassNotPublished& e) {
    throw rti1516e::ObjectClassNotPublished(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotPublished& e) {
    throw rti1516e::AttributeNotPublished(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//Пытаемся спросить отдать права нам, если сейчас они никому не пренадлежат, если комуто принадлежат надо навязчиво требовать (тянем права)
void
RTIambassadorImplementation::attributeOwnershipAcquisitionIfAvailable(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                                      rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::ObjectClassNotPublished,
         rti1516e::AttributeNotDefined,
         rti1516e::AttributeNotPublished,
         rti1516e::FederateOwnsAttributes,
         rti1516e::AttributeAlreadyBeingAcquired,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandleVector attributeHandleVector(rti1516AttributeHandleSet);
    _ambassadorInterface->attributeOwnershipAcquisitionIfAvailable(objectInstanceHandle, attributeHandleVector);
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::ObjectClassNotPublished& e) {
    throw rti1516e::ObjectClassNotPublished(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotPublished& e) {
    throw rti1516e::AttributeNotPublished(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateOwnsAttributes& e) {
    throw rti1516e::FederateOwnsAttributes(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeAlreadyBeingAcquired& e) {
    throw rti1516e::AttributeAlreadyBeingAcquired(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//отказать в передаче владения, даже если нас навязчиво попросили
void
RTIambassadorImplementation::attributeOwnershipReleaseDenied(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                             rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet)
  RTI_THROW ((rti1516e::AttributeNotOwned,
         rti1516e::AttributeNotDefined,
         rti1516e::ObjectInstanceNotKnown,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::FederateNotExecutionMember,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandleVector attributeHandleVector(rti1516AttributeHandleSet);
    _ambassadorInterface->attributeOwnershipReleaseDenied(objectInstanceHandle, attributeHandleVector);
  } catch (const OpenRTI::AttributeNotOwned& e) {
    throw rti1516e::AttributeNotOwned(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//разрешить передачу владения
void
RTIambassadorImplementation::attributeOwnershipDivestitureIfWanted(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                                   rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet,
                                                                   rti1516e::AttributeHandleSet & rti1516DivestedAttributeSet)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::AttributeNotDefined,
         rti1516e::AttributeNotOwned,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandleVector attributeHandleVector(rti1516AttributeHandleSet);
    OpenRTI::AttributeHandleVector divestedAttributeHandleVector;
    _ambassadorInterface->attributeOwnershipDivestitureIfWanted(objectInstanceHandle, attributeHandleVector, divestedAttributeHandleVector);
    rti1516DivestedAttributeSet.clear();
    for (OpenRTI::AttributeHandleVector::const_iterator i = divestedAttributeHandleVector.begin(); i != divestedAttributeHandleVector.end(); ++i)
      rti1516DivestedAttributeSet.insert(OpenRTI::_O1516EAttributeHandle(*i));
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotOwned& e) {
    throw rti1516e::AttributeNotOwned(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//? прекратить согласование передачи прав
void
RTIambassadorImplementation::cancelNegotiatedAttributeOwnershipDivestiture(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                                           rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::AttributeNotDefined,
         rti1516e::AttributeNotOwned,
         rti1516e::AttributeDivestitureWasNotRequested,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandleVector attributeHandleVector(rti1516AttributeHandleSet);
    _ambassadorInterface->cancelNegotiatedAttributeOwnershipDivestiture(objectInstanceHandle, attributeHandleVector);
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotOwned& e) {
    throw rti1516e::AttributeNotOwned(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeDivestitureWasNotRequested& e) {
    throw rti1516e::AttributeDivestitureWasNotRequested(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//?
void
RTIambassadorImplementation::cancelAttributeOwnershipAcquisition(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                                 rti1516e::AttributeHandleSet const & rti1516AttributeHandleSet)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::AttributeNotDefined,
         rti1516e::AttributeAlreadyOwned,
         rti1516e::AttributeAcquisitionWasNotRequested,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandleVector attributeHandleVector(rti1516AttributeHandleSet);
    _ambassadorInterface->cancelAttributeOwnershipAcquisition(objectInstanceHandle, attributeHandleVector);
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeAlreadyOwned& e) {
    throw rti1516e::AttributeAlreadyOwned(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeAcquisitionWasNotRequested& e) {
    throw rti1516e::AttributeAcquisitionWasNotRequested(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//видимо узнать кому сейчас паринадлежат права
void
RTIambassadorImplementation::queryAttributeOwnership(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                     rti1516e::AttributeHandle rti1516AttributeHandle)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::AttributeNotDefined,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandle attributeHandle(rti1516AttributeHandle);
    _ambassadorInterface->queryAttributeOwnership(objectInstanceHandle, attributeHandle);
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}

//видимо узнать, нам сейчас сейчас паринадлежат права или нет
bool
RTIambassadorImplementation::isAttributeOwnedByFederate(rti1516e::ObjectInstanceHandle rti1516ObjectInstanceHandle,
                                                        rti1516e::AttributeHandle rti1516AttributeHandle)
  RTI_THROW ((rti1516e::ObjectInstanceNotKnown,
         rti1516e::AttributeNotDefined,
         rti1516e::FederateNotExecutionMember,
         rti1516e::SaveInProgress,
         rti1516e::RestoreInProgress,
         rti1516e::NotConnected,
         rti1516e::RTIinternalError))
{
  try {
    OpenRTI::_I1516EObjectInstanceHandle objectInstanceHandle(rti1516ObjectInstanceHandle);
    OpenRTI::_I1516EAttributeHandle attributeHandle(rti1516AttributeHandle);
    return _ambassadorInterface->isAttributeOwnedByFederate(objectInstanceHandle, attributeHandle);
  } catch (const OpenRTI::ObjectInstanceNotKnown& e) {
    throw rti1516e::ObjectInstanceNotKnown(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::AttributeNotDefined& e) {
    throw rti1516e::AttributeNotDefined(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::FederateNotExecutionMember& e) {
    throw rti1516e::FederateNotExecutionMember(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::SaveInProgress& e) {
    throw rti1516e::SaveInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::RestoreInProgress& e) {
    throw rti1516e::RestoreInProgress(OpenRTI::utf8ToUcs(e.what()));
  } catch (const OpenRTI::NotConnected& e) {
    throw rti1516e::NotConnected(OpenRTI::utf8ToUcs(e.what()));
  } catch (const std::exception& e) {
    throw rti1516e::RTIinternalError(OpenRTI::utf8ToUcs(e.what()));
  } catch (...) {
    throw rti1516e::RTIinternalError(L"Unknown internal error!");
  }
}


*/