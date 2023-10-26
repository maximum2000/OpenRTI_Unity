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

//������������� �������
extern "C"
{
    //v2
    //������������ � ������� ��������� rtinode.exe (ambassador->connect(L"rti://127.0.0.1"))
    DLLExport int Connect(char* myString, int length);
    //������� ������� (ambassador->createFederationExecution(FederationName, filename))
    DLLExport int CreateFederationExecution(char* myString1, int length1, char* myString2);
    //�����������z � ��������� ��������� ��� ������� � ��������� ������ (ambassador->joinFederationExecution(FederateName,FederationName ))
    DLLExport int JoinFederationExecution(char* myString, int length);
    //��������� ��������� � ��������� ������ ��������� (_ambassador->listFederationExecutions())
    DLLExport int ListFederationExecutions(char* myString, int length);
    //���������������� ����� ������������� (ambassador->registerFederationSynchronizationPoint(L"PPP"))
    DLLExport int RegisterFederationSynchronizationPoint(char* myString, int length);
    //��������� ��������� � ������������ ����� ������������� (ambassador->synchronizationPointAchieved(L"PPP"))
    DLLExport int SynchronizationPointAchieved(char* myString, int length);
    //�������� ������� (ambassador->evokeCallback(dT))
    DLLExport int evokeCallback(double dT);

    //�������� ����� ������ �� ����� (ambassador->getObjectClassHandle(L"HLAobjectRoot.ObjectClass0"))
    DLLExport int GetObjectClassHandle(char* myString, int length);
    //�������� ����� �������� �� ����� (ambassador->getAttributeHandle(objectClassHandle, L"Attribute0"))
    DLLExport int GetAttributeHandle(char* myString1, int length1, char* myString2);
    //�������� ����� ���������� ������� �� �����  getObjectInstanceHandle
    DLLExport int GetObjectInstanceHandle(wchar_t* myString, int length);
    //------------------------------------------------------------------------------------------------------------------
    //������������ ������ � �������� (����� �������) (ambassador->publishObjectClassAttributes (...)
    DLLExport int PublishObjectClassAttributes(char* myString1, int length1, char* myString2);
    //�������� �� ��������� ������� ����� ������ ������� ( ambassador->subscribeObjectClassAttributes(objectClassHandle, attributes))
    DLLExport int SubscribeObjectClassAttributes(char* myString1, int length1, char* myString2);
    //���������������� ���  (ambassador->reserveObjectInstanceName(L"objectInstanceName1"))
    DLLExport int ReserveObjectInstanceName(char* myString1, int length1);
    //�������� � ����������� ������ ���������� ������� ������ (objectInstanceHandle = ambassador->registerObjectInstance(objectClassHandle, L"objectInstanceName1")) +-������� ������?
    DLLExport int RegisterObjectInstance(char* myString1, int length1, char* myString2);
    //��������� �������� ��������� ���������� ������� (ambassador->updateAttributeValues(objectInstanceHandle, attributeValues, tag))
    DLLExport int UpdateAttributeValues(char* myString1, int length1, char* myString2, char* myString3, char* myString4);




    //------------------------------------------------------------------------------------------------------------------
    //��������� ������ ���������� (InteractionClass0Handle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0"))
    DLLExport int GetInteractionClassHandle(char* myString1, int length1);
    //��������� ������ ���������� ���������� (class0Parameter0Handle = ambassador->getParameterHandle(InteractionClass0Handle, L"Parameter0"))
    DLLExport int GetParameterHandle(char* myString1, int length1, char* myString2, int length2);
    //�������� �� ��� ���������� (ambassador->subscribeInteractionClass(InteractionClass0Handle))
    DLLExport int SubscribeInteractionClass(char* myString1, int length1);
    //���������� ���������� (ambassador->publishInteractionClass(InteractionClass0Handle))
    DLLExport int PublishInteractionClass(char* myString1, int length1);
    //��������� ���������� ( ambassador->sendInteraction(InteractionClass0Handle, parameterValues, ambassador->getFederateHandle().encode())
    DLLExport int SendInteraction(char* myString1, int length1, char* myString2, int length2);
    //------------------------------------------------------------------------------------------------------------------
    //����� �� ��������� resignFederationExecution(rti1516e::ResignAction resignAction)
    DLLExport int ResignFederationExecution(char* myString1, int length1);
    //------------------------------------------------------------------------------------------------------------------
    //�������� � ������� ����������� ������ �������� ������� �������� (�������-������ �����) - unconditionalAttributeOwnershipDivestiture
    DLLExport int UnconditionalAttributeOwnershipDivestiture(char* myString1, int length1, char* myString2, int length2);
    //�������� � ������� ������������� ������ �������� ������� �������� (�������-������ �����) - negotiatedAttributeOwnershipDivestiture
    DLLExport int NegotiatedAttributeOwnershipDivestiture(char* myString1, int length1, char* myString2, int length2);
    //������������ ������ �������� (�������-������ �����) - confirmDivestiture
    DLLExport int ConfirmDivestiture(char* myString1, int length1, char* myString2, int length2);
    //��������� ������ ������ ����� ��� (�����-������� �����) - attributeOwnershipAcquisition
    DLLExport int AttributeOwnershipAcquisition(char* myString1, int length1, char* myString2, int length2);
    //�������� �������� ������ ����� ���, ���� ������ ��� ������ �� �����������, ���� ������ ����������� ���� ��������� ��������� (����� �����) - attributeOwnershipAcquisitionIfAvailable
    DLLExport int AttributeOwnershipAcquisitionIfAvailable(char* myString1, int length1, char* myString2, int length2);
    //�������� � �������� ��������, ���� ���� ��� ��������� ��������� - attributeOwnershipReleaseDenied
    DLLExport int AttributeOwnershipReleaseDenied(char* myString1, int length1, char* myString2, int length2);
    //��������� �������� �������� - attributeOwnershipDivestitureIfWanted
    DLLExport int AttributeOwnershipDivestitureIfWanted(char* myString1, int length1, char* myString2, int length2);
    //���������� ������������ �������� ���� - cancelNegotiatedAttributeOwnershipDivestiture
    DLLExport int CancelNegotiatedAttributeOwnershipDivestiture(char* myString1, int length1, char* myString2, int length2);
    //cancelAttributeOwnershipAcquisition
    DLLExport int CancelAttributeOwnershipAcquisition(char* myString1, int length1, char* myString2, int length2);
    //������ ������ ���� ������ ������������ ����� - queryAttributeOwnership
    DLLExport int queryAttributeOwnership(char* myString1, int length1, char* myString2, int length2);
    //������ ������, ��� ������ ������ ������������ ����� ��� ��� - isAttributeOwnedByFederate
    DLLExport int isAttributeOwnedByFederate(char* myString1, int length1, char* myString2, int length2);


    //v1
    //���� ����������
    DLLExport int TestInteraction(char* myString, int length); 
    //���� ��������
    DLLExport int TestObjects(char* myString, int length, char* _className, char* _attributeName, char* _objectInstanceName);
    //��������� ��������
    DLLExport int SetValueAttributeObject(char* myString, int length, char* _objectInstanceName, char* _attributeName, char* _attributeValue);




    
}


OpenRTI::RTI1516EAmbassadorLContent *ambassador;

std::wstring IP;
std::wstring LastErrorString;
std::wstring FederationName;
std::wstring FederateName;

#endif