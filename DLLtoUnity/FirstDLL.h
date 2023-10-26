#ifndef FIRSTDLL_NATIVE_LIB_H
#define FIRSTDLL_NATIVE_LIB_H

#include <cstdlib>
#include <iostream>
#include <memory>

#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>
//#include <RTI1516ETestLib.h>

#include "RTI1516EAmbassadorLContent.h"

#define NOMINMAX
#include <windows.h>

#define DLLExport __declspec(dllexport)  //nothrow

//экпортируемые функции
extern "C"
{
    //v2
    //подключиться к серверу федерации rtinode.exe (ambassador->connect(L"rti://127.0.0.1"))
    DLLExport int Connect(char* myString, int length);
    //создать федерат (ambassador->createFederationExecution(FederationName, filename))
    DLLExport int CreateFederationExecution(char* myString1, int length1, char* myString2);
    //подключаемсz к указанной федерации как федерат с указанным именем (ambassador->joinFederationExecution(FederateName,FederationName ))
    DLLExport int JoinFederationExecution(char* myString, int length);
    //отправить сообщение о получении списка федератов (_ambassador->listFederationExecutions())
    DLLExport int ListFederationExecutions(char* myString, int length);
    //Зарегистрировать точку синхронизации (ambassador->registerFederationSynchronizationPoint(L"PPP"))
    DLLExport int RegisterFederationSynchronizationPoint(char* myString, int length);
    //отправить сообщение о установлении точки синхронизации (ambassador->synchronizationPointAchieved(L"PPP"))
    DLLExport int SynchronizationPointAchieved(char* myString, int length);
    //обновить федерат (ambassador->evokeCallback(dT))
    DLLExport int evokeCallback(double dT);

    //Получить хендл класса по имени (ambassador->getObjectClassHandle(L"HLAobjectRoot.ObjectClass0"))
    DLLExport int GetObjectClassHandle(char* myString, int length);
    //Получить хендл атрибута по имени (ambassador->getAttributeHandle(objectClassHandle, L"Attribute0"))
    DLLExport int GetAttributeHandle(char* myString1, int length1, char* myString2);
    //Получить хендл экземпляра объекта по имени  getObjectInstanceHandle
    DLLExport int GetObjectInstanceHandle(wchar_t* myString, int length);
    //------------------------------------------------------------------------------------------------------------------
    //опубликовать объект и атрибуты (через запятую) (ambassador->publishObjectClassAttributes (...)
    DLLExport int PublishObjectClassAttributes(char* myString1, int length1, char* myString2);
    //подписка на получение событий этого класса объекта ( ambassador->subscribeObjectClassAttributes(objectClassHandle, attributes))
    DLLExport int SubscribeObjectClassAttributes(char* myString1, int length1, char* myString2);
    //зарегистрировать имя  (ambassador->reserveObjectInstanceName(L"objectInstanceName1"))
    DLLExport int ReserveObjectInstanceName(char* myString1, int length1);
    //создание и регистрация нового экземпляра объекта класса (objectInstanceHandle = ambassador->registerObjectInstance(objectClassHandle, L"objectInstanceName1")) +-возврат хендла?
    DLLExport int RegisterObjectInstance(char* myString1, int length1, char* myString2);
    //изменение значений атрибутов экземпляра объекта (ambassador->updateAttributeValues(objectInstanceHandle, attributeValues, tag))
    DLLExport int UpdateAttributeValues(char* myString1, int length1, char* myString2, char* myString3, char* myString4);




    //------------------------------------------------------------------------------------------------------------------
    //получение класса интеракции (InteractionClass0Handle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0"))
    DLLExport int GetInteractionClassHandle(char* myString1, int length1);
    //получение класса параметров интеракции (class0Parameter0Handle = ambassador->getParameterHandle(InteractionClass0Handle, L"Parameter0"))
    DLLExport int GetParameterHandle(char* myString1, int length1, char* myString2, int length2);
    //подписка на эту интеракцию (ambassador->subscribeInteractionClass(InteractionClass0Handle))
    DLLExport int SubscribeInteractionClass(char* myString1, int length1);
    //публикация интеракции (ambassador->publishInteractionClass(InteractionClass0Handle))
    DLLExport int PublishInteractionClass(char* myString1, int length1);
    //отправить интеракцию ( ambassador->sendInteraction(InteractionClass0Handle, parameterValues, ambassador->getFederateHandle().encode())
    DLLExport int SendInteraction(char* myString1, int length1, char* myString2, int length2);
    //------------------------------------------------------------------------------------------------------------------
    //выйти из федерации resignFederationExecution(rti1516e::ResignAction resignAction)
    DLLExport int ResignFederationExecution(char* myString1, int length1);
    //------------------------------------------------------------------------------------------------------------------
    //сообщаем о желании безусловной отдачи владения другому федерату (толкаем-отдаем права) - unconditionalAttributeOwnershipDivestiture
    DLLExport int UnconditionalAttributeOwnershipDivestiture(char* myString1, int length1, char* myString2, int length2);
    //сообщаем о желании согласованной отдачи владения другому федерату (толкаем-отдаем права) - negotiatedAttributeOwnershipDivestiture
    DLLExport int NegotiatedAttributeOwnershipDivestiture(char* myString1, int length1, char* myString2, int length2);
    //подтверждаем отдачу владения (толкаем-отдаем права) - confirmDivestiture
    DLLExport int ConfirmDivestiture(char* myString1, int length1, char* myString2, int length2);
    //Навязчиво просим отдать права нам (тянем-требуем права) - attributeOwnershipAcquisition
    DLLExport int AttributeOwnershipAcquisition(char* myString1, int length1, char* myString2, int length2);
    //Пытаемся спросить отдать права нам, если сейчас они никому не пренадлежат, если комуто принадлежат надо навязчиво требовать (тянем права) - attributeOwnershipAcquisitionIfAvailable
    DLLExport int AttributeOwnershipAcquisitionIfAvailable(char* myString1, int length1, char* myString2, int length2);
    //отказать в передаче владения, даже если нас навязчиво попросили - attributeOwnershipReleaseDenied
    DLLExport int AttributeOwnershipReleaseDenied(char* myString1, int length1, char* myString2, int length2);
    //разрешить передачу владения - attributeOwnershipDivestitureIfWanted
    DLLExport int AttributeOwnershipDivestitureIfWanted(char* myString1, int length1, char* myString2, int length2);
    //прекратить согласование передачи прав - cancelNegotiatedAttributeOwnershipDivestiture
    DLLExport int CancelNegotiatedAttributeOwnershipDivestiture(char* myString1, int length1, char* myString2, int length2);
    //cancelAttributeOwnershipAcquisition
    DLLExport int CancelAttributeOwnershipAcquisition(char* myString1, int length1, char* myString2, int length2);
    //видимо узнать кому сейчас паринадлежат права - queryAttributeOwnership
    DLLExport int queryAttributeOwnership(char* myString1, int length1, char* myString2, int length2);
    //видимо узнать, нам сейчас сейчас паринадлежат права или нет - isAttributeOwnedByFederate
    DLLExport int isAttributeOwnedByFederate(char* myString1, int length1, char* myString2, int length2);


    //v1
    //Тест интеракций
    DLLExport int TestInteraction(char* myString, int length); 
    //Тест объектов
    DLLExport int TestObjects(char* myString, int length, char* _className, char* _attributeName, char* _objectInstanceName);
    //Изменение значения
    DLLExport int SetValueAttributeObject(char* myString, int length, char* _objectInstanceName, char* _attributeName, char* _attributeValue);




    
}


OpenRTI::RTI1516EAmbassadorLContent *ambassador;

std::wstring IP;
std::wstring LastErrorString;
std::wstring FederationName;
std::wstring FederateName;

#endif