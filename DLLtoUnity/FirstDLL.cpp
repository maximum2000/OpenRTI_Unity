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

#include "FirstDLL.h"

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
int MyConnect()
{
    LastErrorString = L"";
    FederationName = L"";
    FederateName = L"";

    try
    {
        ambassador->connect(L"rti://127.0.0.1"); //thread://
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
    //OpenRTI::RTI1516ESimpleAmbassador ambassador;
    ambassador = new OpenRTI::RTI1516EAmbassadorLContent();
    ambassador->setUseDataUrlObjectModels(false);

    /*
    When I call "connect" function with computer name as its input (i.e ambassador->connect("rti://rtinodeSystem")) the federate can join to rti(rtinode in TCP mode),
    but if I replace computer name with its IP (i.e ambassador->connect("rti://192.168.1.4")) it cannot. Why this happen?
    Is the syntax when we use the IP address something else(I am sure that the "rtinodeSystem"s IP is "192.168.1.4")?
    Maybe  turn off IPV6_V6ONLY ?
    */

    int ret = MyConnect();
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
int MyCreateFederationExecution(std::wstring name)
{
    LastErrorString = L"";
    FederationName = name;

    try
    {
        //M:/GIT2/ieee1516/UnityTest/Assets/Plugins
        //C:/WORK/OpenRTI_Unity/UnityTest/Assets/Plugins
        
        ambassador->createFederationExecution(FederationName, L"M:/GIT2/ieee1516/UnityTest/Assets/Plugins/fdd_test.xml");
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
int CreateFederationExecution(char* myString, int length)
{
    std::wstringstream cls;
    cls << myString;
    std::wstring name = cls.str();

    ambassador->SendLog(name, 0);

    // create, must work
    int ret = MyCreateFederationExecution(name);
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

    ambassador->SendLog(name, 0);

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
    parameterValues[class0Parameter0Handle] = toVariableLengthData("parameter0");
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

//----------------[Тест объектов]----------------------------------------
//Тест объектов
int MyTestObjects()
{
    LastErrorString = L"";

    //хранение класса объекта и его аттрибутов
    rti1516e::ObjectClassHandle objectClassHandle;
    rti1516e::AttributeHandleSet attributes;
    //хранение экземпляра класса объекта
    rti1516e::ObjectInstanceHandle objectInstanceHandle;

    //получение класса объекта
    try 
    {
        objectClassHandle = ambassador->getObjectClassHandle(L"HLAobjectRoot.ObjectClass0");
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
        attributes.insert(ambassador->getAttributeHandle(objectClassHandle, L"Attribute0"));
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
        ambassador->reserveObjectInstanceName(L"objectInstanceName1");
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
        std::wstring objectInstanceName = L"objectInstanceName1";
        objectInstanceHandle = ambassador->registerObjectInstance(objectClassHandle, objectInstanceName);
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
            attributeValues[*k] = toVariableLengthData(ambassador->getAttributeName(objectClassHandle, *k));
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



    /*
    не реализованы:
    * ambassador.deleteObjectInstance(objectInstanceHandle, toVariableLengthData("tag"));
    * ambassador.unsubscribeObjectClass(subscribedObjectClass);    
    */
   
    return 0;

}

//Тест объектов
int TestObjects(char* myString, int length)
{
    //std::wstringstream cls;
    //cls << myString;
    //std::wstring name = cls.str();

    //ambassador->SendLog(name, 0);

    // join must work
    int ret = MyTestObjects();
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