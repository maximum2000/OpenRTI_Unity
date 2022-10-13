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

//typedef std::map<rti1516e::ObjectClassHandle, rti1516e::AttributeHandleSet> ObjectClassAttributeHandleSet;


#define DLLExport __declspec(dllexport)  //nothrow

extern "C"
{
    
    DLLExport int Connect(char* myString, int length);
    DLLExport int CreateFederationExecution(char* myString, int length);
    DLLExport int JoinFederationExecution(char* myString, int length);

    DLLExport int RegisterFederationSynchronizationPoint(char* myString, int length);
    DLLExport int SynchronizationPointAchieved(char* myString, int length);
    DLLExport int ListFederationExecutions(char* myString, int length);
    DLLExport int evokeCallback(double dT);

    //Тест интеракций
    DLLExport int TestInteraction(char* myString, int length); 
    //Тест объектов
    DLLExport int TestObjects(char* myString, int length);
    
}


OpenRTI::RTI1516EAmbassadorLContent *ambassador;

std::wstring LastErrorString;
std::wstring FederationName;
std::wstring FederateName;

#endif