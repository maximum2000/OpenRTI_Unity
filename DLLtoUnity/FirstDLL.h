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
    DLLExport int Connect(char* myString, int length);
    DLLExport int CreateFederationExecution(char* myString1, int length1, char* myString2);
    DLLExport int JoinFederationExecution(char* myString, int length);

    DLLExport int RegisterFederationSynchronizationPoint(char* myString, int length);
    DLLExport int SynchronizationPointAchieved(char* myString, int length);
    DLLExport int ListFederationExecutions(char* myString, int length);
    DLLExport int evokeCallback(double dT);

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