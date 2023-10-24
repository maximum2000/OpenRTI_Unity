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
    //�������� ����� �������� �� ����� (ambassador->getAttributeHandle(objectClassHandle, L"Attribute0"))
    //�������� ����� ���������� ������� �� �����  getObjectInstanceHandle
    //------------------------------------------------------------------------------------------------------------------
    //������������ ������ � �������� (����� �������)
    //ambassador->publishObjectClassAttributes (har* myString1, int length1, char* myString2, int length2)
    //�������� �� ��������� ������� ����� ������ ������� ( ambassador->subscribeObjectClassAttributes(objectClassHandle, attributes))
    //���������������� ���  (ambassador->reserveObjectInstanceName(L"objectInstanceName1"))
    //�������� � ����������� ������ ���������� ������� ������ (objectInstanceHandle = ambassador->registerObjectInstance(objectClassHandle, L"objectInstanceName1")) +-������� ������?
    //��������� �������� ��������� ���������� ������� (ambassador->updateAttributeValues(objectInstanceHandle, attributeValues, tag))
    //------------------------------------------------------------------------------------------------------------------
    //��������� ������ ���������� (InteractionClass0Handle = ambassador->getInteractionClassHandle(L"HLAinteractionRoot.InteractionClass0"))
    //��������� ������ ���������� ���������� (class0Parameter0Handle = ambassador->getParameterHandle(InteractionClass0Handle, L"Parameter0"))
    //�������� �� ��� ���������� (ambassador->subscribeInteractionClass(InteractionClass0Handle))
    //���������� ���������� (ambassador->publishInteractionClass(InteractionClass0Handle))
    //��������� ���������� ( ambassador->sendInteraction(InteractionClass0Handle, parameterValues, ambassador->getFederateHandle().encode())
    //------------------------------------------------------------------------------------------------------------------
    //+- ����� �� ��������� resignFederationExecution(rti1516e::ResignAction resignAction)
    //------------------------------------------------------------------------------------------------------------------
    //�������� � ������� ����������� ������ �������� ������� �������� (�������-������ �����) - unconditionalAttributeOwnershipDivestiture
    //�������� � ������� ������������� ������ �������� ������� �������� (�������-������ �����) - negotiatedAttributeOwnershipDivestiture
    //������������ ������ �������� (�������-������ �����) - confirmDivestiture
    //��������� ������ ������ ����� ��� (�����-������� �����) - attributeOwnershipAcquisition
    //�������� �������� ������ ����� ���, ���� ������ ��� ������ �� �����������, ���� ������ ����������� ���� ��������� ��������� (����� �����) - attributeOwnershipAcquisitionIfAvailable
    //�������� � �������� ��������, ���� ���� ��� ��������� ��������� - attributeOwnershipReleaseDenied
    //��������� �������� �������� - attributeOwnershipDivestitureIfWanted
    //? ���������� ������������ �������� ���� - cancelNegotiatedAttributeOwnershipDivestiture
    //? cancelAttributeOwnershipAcquisition
    //������ ������ ���� ������ ������������ ����� - queryAttributeOwnership
    //������ ������, ��� ������ ������ ������������ ����� ��� ��� - isAttributeOwnedByFederate



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