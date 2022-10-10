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





//подключиться к серверу федерации
int MyConnect()
{
    LastErrorString = L"";

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

//создать федерат
int MyCreateFederationExecution()
{
    LastErrorString = L"";

    try
    {
        ambassador->createFederationExecution(L"TEST1", L"M:/GIT2/ieee1516/UnityTestHLA/Assets/Plugins/fdd_test.xml");
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

//присоедениться к федерации
int MyJoinFederationExecution()
{
    LastErrorString = L"";

    try
    {
        //подключаемс к федерации TEST1 как федерат с именем "federate"
        ambassador->joinFederationExecution(L"federate", L"TEST1");
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



int evokeCallback(double dT)
{
    ambassador->evokeCallback(dT);
    return 0;
}


int StartRTI(char* myString, int length)
{
    //std::cout << "Max Gammer Test 1" << std::endl;

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
   
    // create, must work
    ret = MyCreateFederationExecution();
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }

    // join must work
    ret = MyJoinFederationExecution();
    if (ret == 1)
    {
        std::string s(LastErrorString.begin(), LastErrorString.end());
        strcpy_s(myString, length, s.c_str());
        return 1;
    }


   
   
    
    //std::cout << "Max Gammer Test 2" << std::endl;
    std::string s("ok");
    strcpy_s(myString, length, s.c_str());

    return 0 ;
}



/*


// and now resign must work
      try {
        ambassador->resignFederationExecution(rti1516e::CANCEL_THEN_DELETE_THEN_DIVEST);
      } catch (const rti1516e::Exception& e) {
        std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
        return false;
      } catch (...) {
        std::wcout << L"Unknown Exception!" << std::endl;
        return false;
      }



      // destroy, must work once
    try {
      ambassador->destroyFederationExecution(getFederationExecution());

      if (!getFederationBarrier()->success())
        return false;
    } catch (const rti1516e::FederatesCurrentlyJoined&) {
      // Can happen in this test
      // Other threads just might have still joined.

      if (!getFederationBarrier()->fail())
        return false;
    } catch (const rti1516e::FederationExecutionDoesNotExist&) {
      // Can happen in this test
      // Other threads might have been faster

      if (!getFederationBarrier()->fail())
        return false;
    } catch (const rti1516e::Exception& e) {
      std::wcout << L"rti1516e::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

*/