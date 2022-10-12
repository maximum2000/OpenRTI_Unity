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

//создать федерат
int MyCreateFederationExecution(std::wstring name)
{
    LastErrorString = L"";
    FederationName = name;

    try
    {
        //M:/GIT2/ieee1516/UnityTest/Assets/Plugins
        //C:/WORK/OpenRTI_Unity/UnityTest/Assets/Plugins
        
        ambassador->createFederationExecution(FederationName, L"C:/WORK/OpenRTI_Unity/UnityTest/Assets/Plugins/fdd_test.xml");
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
    return 0 ;
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

//подключение нового федерата
int JoinFederationExecution(char* myString, int length)
{
    std::wstringstream cls;
    cls << myString;
    std::wstring name = cls.str();

    ambassador->SendLog(name,0);

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



/*


 //rti1516::InteractionClassHandle
    //rti1516::ParameterHandle class0Parameter0Handle;

    try
    {
      _requestInteractionClassHandle = ambassador.getInteractionClassHandle(L"Request");
      //ambassador.getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0");
    }
    catch (const rti1516::Exception& e)
    {
      std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    }
    catch (...)
    {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try
    {
      _requestTypeHandle = ambassador.getParameterHandle(_requestInteractionClassHandle, L"requestType");
    }
    catch (const rti1516::Exception& e)
    {
      std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    }
    catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }



    try {
      ambassador.subscribeInteractionClass(_requestInteractionClassHandle);
    } catch (const rti1516::Exception& e) {
      std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }

    try {
      ambassador.publishInteractionClass(_requestInteractionClassHandle);
    } catch (const rti1516::Exception& e) {
      std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
      return false;
    } catch (...) {
      std::wcout << L"Unknown Exception!" << std::endl;
      return false;
    }



 ambassador.evokeCallback(10.0)

rti1516::ParameterHandleValueMap parameterValues;
interactionClassHandle = interactionClassHandle1;
parameterValues[class1Parameter0Handle] = toVariableLengthData("parameter0");
parameterValues[class1Parameter1Handle] = toVariableLengthData("parameter1");

try {
  ambassador.sendInteraction(interactionClassHandle, parameterValues, getFederateHandle().encode());

  // Also send when we reponded, so that the requestor knows when we have sent something
  parameterValues.clear();
  parameterValues[_requestTypeHandle] = toVariableLengthData(unsigned(_requestType));
  ambassador.sendInteraction(_requestInteractionClassHandle, parameterValues, _requestFederate);
}
catch (const rti1516::Exception& e)
{
  std::wcout << L"rti1516::Exception: \"" << e.what() << L"\"" << std::endl;
  return false;
}
catch (...)
{
  std::wcout << L"Unknown Exception!" << std::endl;
  return false;
}






  void receiveInteraction(rti1516::InteractionClassHandle interactionClassHandle,
                          const rti1516::ParameterHandleValueMap& parameterValues,
                          const rti1516::VariableLengthData& tag,
                          rti1516::OrderType sentOrder,
                          rti1516::TransportationType theType)
      RTI_THROW ((rti1516::InteractionClassNotRecognized,
             rti1516::InteractionParameterNotRecognized,
             rti1516::InteractionClassNotSubscribed,
             rti1516::FederateInternalError))
  {
    if (interactionClassHandle != _requestInteractionClassHandle) {
      std::wcout << L"Received interaction class that was not subscribed!" << std::endl;
      _fail = true;
      _receivedInteraction = true;
    }

    if (getFederateHandle().encode() != tag)
      return;

    if (interactionClassHandle == _interactionClassHandles[0])
      _receivedRequestType = Interaction0;

  }





















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