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

extern "C"
{
    
    DLLExport int Connect(char* myString, int length);
    DLLExport int CreateFederationExecution(char* myString, int length);
    DLLExport int JoinFederationExecution(char* myString, int length);

    DLLExport int RegisterFederationSynchronizationPoint(char* myString, int length);
    DLLExport int SynchronizationPointAchieved(char* myString, int length);
    DLLExport int ListFederationExecutions(char* myString, int length);
    DLLExport int evokeCallback(double dT);
    

}


OpenRTI::RTI1516EAmbassadorLContent *ambassador;

std::wstring LastErrorString;
std::wstring FederationName;
std::wstring FederateName;

#endif