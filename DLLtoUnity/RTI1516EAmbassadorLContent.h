#ifndef RTI1516EAmbassadorLContent_h
#define RTI1516EAmbassadorLContent_h

#include <algorithm>
#include <cstring>
#include <iterator>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <RTI/FederateAmbassador.h>
#include <RTI/RTIambassadorFactory.h>
#include <RTI/RTIambassador.h>
#include <RTI/LogicalTime.h>
#include <RTI/LogicalTimeInterval.h>
#include <RTI/LogicalTimeFactory.h>
#include <RTI/RangeBounds.h>


#include <iostream>

//#include <TestLib.h>
#include <NetworkServer.h>



//!!!
// https://github.com/mkowalik92/UnityNativePluginPractice
// Call this function from a Untiy script
extern "C"
{
    typedef void(*DebugCallback)(const char* message, int color, int size);
    static DebugCallback callbackFunction = nullptr;
    __declspec(dllexport) void RegisterDebugCallback(DebugCallback callback);
}

extern "C"
{
    typedef void(*�onnectionLostCallback)(const char* message, int size);
    static �onnectionLostCallback �onnectionLostCallbackFunction = nullptr;
    __declspec(dllexport) void RegisterConnectionLostCallback(�onnectionLostCallback callback);
}

extern "C"
{
    typedef void(*reportFederationExecutionsCallback)(const char* message1, const char* message2, int size1, int size2);
    static reportFederationExecutionsCallback reportFederationExecutionsCallbackFunction = nullptr;
    __declspec(dllexport) void RegisterReportFederationExecutionsCallback(reportFederationExecutionsCallback callback);
}


extern "C"
{
    typedef void(*synchronizationPointRegistrationSucceededCallback)(const char* message1,  int size1);
    static synchronizationPointRegistrationSucceededCallback synchronizationPointRegistrationSucceededFunction = nullptr;
    __declspec(dllexport) void RegisterSynchronizationPointRegistrationSucceededCallback(synchronizationPointRegistrationSucceededCallback callback);
}


extern "C"
{
    typedef void(*synchronizationPointRegistrationFailedCallback)(const char* message1, const char* message2, int size1, int size2);
    static synchronizationPointRegistrationFailedCallback synchronizationPointRegistrationFailedFunction = nullptr;
    __declspec(dllexport) void RegisterSynchronizationPointRegistrationFailedCallback(synchronizationPointRegistrationFailedCallback callback);
}

extern "C"
{
    typedef void(*announceSynchronizationPointCallback)(const char* message1,  int size1);
    static announceSynchronizationPointCallback announceSynchronizationPointCallbackFunction = nullptr;
    __declspec(dllexport) void RegisterAnnounceSynchronizationPointCallback(announceSynchronizationPointCallback callback);
}

extern "C"
{
    typedef void(*federationSynchronizedCallback)(const char* message1, int size1);
    static federationSynchronizedCallback federationSynchronizedFunction = nullptr;
    __declspec(dllexport) void RegisterFederationSynchronizedCallback(federationSynchronizedCallback callback);
}




void RegisterDebugCallback(DebugCallback callback)
{
    callbackFunction = callback;
}
void RegisterConnectionLostCallback(�onnectionLostCallback callback)
{
    �onnectionLostCallbackFunction = callback;
}
void RegisterReportFederationExecutionsCallback(reportFederationExecutionsCallback callback)
{
    reportFederationExecutionsCallbackFunction = callback;
}
void RegisterSynchronizationPointRegistrationSucceededCallback(synchronizationPointRegistrationSucceededCallback callback)
{
    synchronizationPointRegistrationSucceededFunction = callback;
}
void RegisterSynchronizationPointRegistrationFailedCallback(synchronizationPointRegistrationFailedCallback callback)
{
    synchronizationPointRegistrationFailedFunction = callback;
}
void RegisterAnnounceSynchronizationPointCallback(announceSynchronizationPointCallback callback)
{
    announceSynchronizationPointCallbackFunction = callback;
}
void RegisterFederationSynchronizedCallback(federationSynchronizedCallback callback)
{
    federationSynchronizedFunction = callback;
}
//!!!


namespace OpenRTI
{
    class OPENRTI_LOCAL RTI1516EAmbassadorLContent : public rti1516e::FederateAmbassador {
    public:
        RTI1516EAmbassadorLContent() :
            _useDataUrlObjectModels(false),
            // _fail(false),
            _timeRegulationEnabled(false),
            _timeConstrainedEnabled(false),
            _timeAdvancePending(false)
        {
         
        }
        virtual ~RTI1516EAmbassadorLContent()
            RTI_NOEXCEPT
        { }

        //!!!
        void SendLog(const std::wstring& str, const int& color)
        {
            std::string s(str.begin(), str.end());
            const char* tmsg = s.c_str();
            if (callbackFunction != nullptr)
            {
                callbackFunction(tmsg, (int)color, (int)strlen(tmsg));
            }
        }
        //!!!

        //������ ������ ��������� � ���������, ����� ��. reportFederationExecutions
        void mylistFederationExecutions()
        {
            _ambassador->listFederationExecutions();
        }



        void setUseDataUrlObjectModels(bool useDataUrlObjectModels)
        {
            _useDataUrlObjectModels = useDataUrlObjectModels;
        }
        bool getUseDataUrlObjectModels() const
        {
            return _useDataUrlObjectModels;
        }

        // bool getFail() const
        // { return _fail; }

        bool getTimeRegulationEnabled() const
        {
            return _timeRegulationEnabled;
        }
        bool getTimeConstrainedEnabled() const
        {
            return _timeConstrainedEnabled;
        }
        bool getTimeAdvancePending() const
        {
            return _timeAdvancePending;
        }

        


        const rti1516e::FederateHandle& getFederateHandle() const
        {
            return _federateHandle;
        }

        void connect(const std::wstring& url)
        {
            rti1516e::RTIambassadorFactory factory;
            _ambassador = factory.createRTIambassador();
            _ambassador->connect(*this, rti1516e::HLA_EVOKED, url);
            setLogicalTimeFactory();
        }

        void setLogicalTimeFactory(const std::wstring& logicalTimeImplementationName = std::wstring(L"HLAinteger64Time"))
        {
            _logicalTimeImplementationName = logicalTimeImplementationName;
            _logicalTimeFactory = rti1516e::LogicalTimeFactoryFactory::makeLogicalTimeFactory(logicalTimeImplementationName);
        }

        void createFederationExecution(const std::wstring& federationExecutionName, std::wstring fddFile)
        {
            _replaceFileWithDataIfNeeded(fddFile);
            _ambassador->createFederationExecution(federationExecutionName, fddFile, _logicalTimeImplementationName);
        }
        void createFederationExecution(const std::wstring& federationExecutionName, std::vector<std::wstring> fomModules)
        {
            _replaceFilesWithDataIfNeeded(fomModules);
            _ambassador->createFederationExecution(federationExecutionName, fomModules, _logicalTimeImplementationName);
        }
        void createFederationExecutionWithMIM(const std::wstring& federationExecutionName, std::vector<std::wstring> fomModules,
            std::wstring mimModule)
        {
            _replaceFilesWithDataIfNeeded(fomModules);
            _replaceFileWithDataIfNeeded(mimModule);
            _ambassador->createFederationExecutionWithMIM(federationExecutionName, fomModules, mimModule, _logicalTimeImplementationName);
        }

        void destroyFederationExecution(const std::wstring& federationExecutionName)
        {
            _ambassador->destroyFederationExecution(federationExecutionName);
        }

        const rti1516e::FederateHandle& joinFederationExecution(const std::wstring& federateType,
            const std::wstring& federationExecutionName,
            std::vector<std::wstring> additionalFomModules = std::vector<std::wstring>())
        {
            _replaceFilesWithDataIfNeeded(additionalFomModules);
            _federateHandle = _ambassador->joinFederationExecution(federateType, federationExecutionName, additionalFomModules);
            _grantedLogicalTime = _logicalTimeFactory->makeInitial();
            return _federateHandle;
        }

        const rti1516e::FederateHandle& joinFederationExecution(const std::wstring& federateName, const std::wstring& federateType,
            const std::wstring& federationExecutionName,
            std::vector<std::wstring> additionalFomModules = std::vector<std::wstring>())
        {
            _replaceFilesWithDataIfNeeded(additionalFomModules);
            _federateHandle = _ambassador->joinFederationExecution(federateName, federateType, federationExecutionName, additionalFomModules);
            _grantedLogicalTime = _logicalTimeFactory->makeInitial();
            return _federateHandle;
        }

        void resignFederationExecution(rti1516e::ResignAction resignAction)
        {
            _ambassador->resignFederationExecution(resignAction);
            _federateHandle = rti1516e::FederateHandle();
        }

        void registerFederationSynchronizationPoint(const std::wstring& label, const rti1516e::VariableLengthData& tag)
        {
            _ambassador->registerFederationSynchronizationPoint(label, tag);
        }

        void registerFederationSynchronizationPoint(const std::wstring& label, const rti1516e::VariableLengthData& tag,
            const rti1516e::FederateHandleSet& federateHandleSet)
        {
            _ambassador->registerFederationSynchronizationPoint(label, tag, federateHandleSet);
        }

        void synchronizationPointAchieved(const std::wstring& label)
        {
            _ambassador->synchronizationPointAchieved(label);
        }

        void requestFederationSave(const std::wstring& label)
        {
            _ambassador->requestFederationSave(label);
        }

        void requestFederationSave(const std::wstring& label, const rti1516e::LogicalTime& logicalTime)
        {
            _ambassador->requestFederationSave(label, logicalTime);
        }

        void federateSaveComplete()
        {
            _ambassador->federateSaveComplete();
        }

        void federateSaveNotComplete()
        {
            _ambassador->federateSaveNotComplete();
        }

        void queryFederationSaveStatus()
        {
            _ambassador->queryFederationSaveStatus();
        }

        void requestFederationRestore(const std::wstring& label)
        {
            _ambassador->requestFederationRestore(label);
        }

        void federateRestoreComplete()
        {
            _ambassador->federateRestoreComplete();
        }

        void federateRestoreNotComplete()
        {
            _ambassador->federateRestoreNotComplete();
        }

        void queryFederationRestoreStatus()
        {
            _ambassador->queryFederationRestoreStatus();
        }

        void publishObjectClassAttributes(const rti1516e::ObjectClassHandle& objectClassHandle,
            const rti1516e::AttributeHandleSet& attributeList)
        {
            _ambassador->publishObjectClassAttributes(objectClassHandle, attributeList);
        }

        void unpublishObjectClass(const rti1516e::ObjectClassHandle& objectClassHandle)
        {
            _ambassador->unpublishObjectClass(objectClassHandle);
        }

        void unpublishObjectClassAttributes(const rti1516e::ObjectClassHandle& objectClassHandle,
            const rti1516e::AttributeHandleSet& attributeList)
        {
            _ambassador->unpublishObjectClassAttributes(objectClassHandle, attributeList);
        }

        void publishInteractionClass(const rti1516e::InteractionClassHandle& interactionClassHandle)
        {
            _ambassador->publishInteractionClass(interactionClassHandle);
        }

        void unpublishInteractionClass(const rti1516e::InteractionClassHandle& interactionClassHandle)
        {
            _ambassador->unpublishInteractionClass(interactionClassHandle);
        }

        void subscribeObjectClassAttributes(const rti1516e::ObjectClassHandle& objectClassHandle,
            const rti1516e::AttributeHandleSet& attributeHandleSet,
            bool active = true)
        {
            _ambassador->subscribeObjectClassAttributes(objectClassHandle, attributeHandleSet, active);
            // _subscribedObjectClassAttributeHandleSetMap[objectClassHandle].insert(attributeHandleSet.begin(), attributeHandleSet.end());
        }

        void unsubscribeObjectClass(const rti1516e::ObjectClassHandle& objectClassHandle)
        {
            _ambassador->unsubscribeObjectClass(objectClassHandle);
            // _subscribedObjectClassAttributeHandleSetMap.erase(objectClassHandle);
        }

        void unsubscribeObjectClassAttributes(const rti1516e::ObjectClassHandle& objectClassHandle,
            const rti1516e::AttributeHandleSet& attributeHandleSet)
        {
            _ambassador->unsubscribeObjectClassAttributes(objectClassHandle, attributeHandleSet);
            // ObjectClassAttributeHandleSetMap::iterator i = _subscribedObjectClassAttributeHandleSetMap.find(objectClassHandle);
            // for (rti1516e::AttributeHandleSet::const_iterator j = attributeHandleSet.begin();
            //      j != attributeHandleSet.end(); ++j)
            //     i->second.erase(*j);
        }

        void subscribeInteractionClass(const rti1516e::InteractionClassHandle& interactionClassHandle, bool active = true)
        {
            _ambassador->subscribeInteractionClass(interactionClassHandle, active);
        }

        void unsubscribeInteractionClass(const rti1516e::InteractionClassHandle& interactionClassHandle)
        {
            _ambassador->unsubscribeInteractionClass(interactionClassHandle);
        }

        void reserveObjectInstanceName(const std::wstring& objectInstanceName)
        {
            _ambassador->reserveObjectInstanceName(objectInstanceName);
        }

        rti1516e::ObjectInstanceHandle registerObjectInstance(const rti1516e::ObjectClassHandle& objectClassHandle)
        {
            return _ambassador->registerObjectInstance(objectClassHandle);
        }

        rti1516e::ObjectInstanceHandle registerObjectInstance(const rti1516e::ObjectClassHandle& objectClassHandle,
            const std::wstring& objectInstanceName)
        {
            return _ambassador->registerObjectInstance(objectClassHandle, objectInstanceName);
        }

        void updateAttributeValues(const rti1516e::ObjectInstanceHandle& objectInstanceHandle,
            const rti1516e::AttributeHandleValueMap& attributeHandleValueMap,
            const rti1516e::VariableLengthData& tag)
        {
            _ambassador->updateAttributeValues(objectInstanceHandle, attributeHandleValueMap, tag);
        }

        rti1516e::MessageRetractionHandle updateAttributeValues(const rti1516e::ObjectInstanceHandle& objectInstanceHandle,
            const rti1516e::AttributeHandleValueMap& attributeHandleValueMap,
            const rti1516e::VariableLengthData& tag,
            const rti1516e::LogicalTime& logicalTime)
        {
            return _ambassador->updateAttributeValues(objectInstanceHandle, attributeHandleValueMap, tag, logicalTime);
        }

        void sendInteraction(const rti1516e::InteractionClassHandle& interactionClassHandle,
            const rti1516e::ParameterHandleValueMap& parameterHandleValueMap,
            const rti1516e::VariableLengthData& tag)
        {
            _ambassador->sendInteraction(interactionClassHandle, parameterHandleValueMap, tag);
        }

        rti1516e::MessageRetractionHandle sendInteraction(const rti1516e::InteractionClassHandle& interactionClassHandle,
            const rti1516e::ParameterHandleValueMap& parameterHandleValueMap,
            const rti1516e::VariableLengthData& tag,
            const rti1516e::LogicalTime& logicalTime)
        {
            return _ambassador->sendInteraction(interactionClassHandle, parameterHandleValueMap, tag, logicalTime);
        }

        void deleteObjectInstance(const rti1516e::ObjectInstanceHandle& objectInstanceHandle,
            const rti1516e::VariableLengthData& tag)
        {
            _ambassador->deleteObjectInstance(objectInstanceHandle, tag);
        }

        rti1516e::MessageRetractionHandle deleteObjectInstance(const rti1516e::ObjectInstanceHandle& objectInstanceHandle,
            const rti1516e::VariableLengthData& tag,
            const rti1516e::LogicalTime& logicalTime)
        {
            return _ambassador->deleteObjectInstance(objectInstanceHandle, tag, logicalTime);
        }

        void localDeleteObjectInstance(const rti1516e::ObjectInstanceHandle& objectInstanceHandle)
        {
            _ambassador->localDeleteObjectInstance(objectInstanceHandle);
        }

        void requestAttributeTransportationTypeChange(const rti1516e::ObjectInstanceHandle& objectInstanceHandle,
            const rti1516e::AttributeHandleSet& attributeHandleSet,
            const rti1516e::TransportationType& transportationType)
        {
            _ambassador->requestAttributeTransportationTypeChange(objectInstanceHandle, attributeHandleSet, transportationType);
        }

        void requestInteractionTransportationTypeChange(const rti1516e::InteractionClassHandle& interactionClassHandle,
            const rti1516e::TransportationType& transportationType)
        {
            _ambassador->requestInteractionTransportationTypeChange(interactionClassHandle, transportationType);
        }

        void requestAttributeValueUpdate(const rti1516e::ObjectInstanceHandle& objectInstanceHandle,
            const rti1516e::AttributeHandleSet& attributeHandleSet,
            const rti1516e::VariableLengthData& tag)
        {
            _ambassador->requestAttributeValueUpdate(objectInstanceHandle, attributeHandleSet, tag);
        }

        void requestAttributeValueUpdate(const rti1516e::ObjectClassHandle& objectClassHandle,
            const rti1516e::AttributeHandleSet& attributeHandleSet,
            const rti1516e::VariableLengthData& tag)
        {
            _ambassador->requestAttributeValueUpdate(objectClassHandle, attributeHandleSet, tag);
        }

//!!!!!!!!!!!!!!!!
    void unconditionalAttributeOwnershipDivestiture(const rti1516e::ObjectInstanceHandle& objectInstanceHandle,
            const rti1516e::AttributeHandleSet& attributeHandleSet)
        {
            _ambassador->unconditionalAttributeOwnershipDivestiture(objectInstanceHandle, attributeHandleSet);
        }

        // // 7.3
        //  void negotiatedAttributeOwnershipDivestiture
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSet const & attributes,
        //  VariableLengthData const & tag)

        // // 7.6
        //  void confirmDivestiture
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSet const & confirmedAttributes,
        //  VariableLengthData const & tag)

        // // 7.8
        //  void attributeOwnershipAcquisition
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSet const & desiredAttributes,
        //  VariableLengthData const & tag)

        // // 7.9
        //  void attributeOwnershipAcquisitionIfAvailable
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSet const & desiredAttributes)

        // // 7.12
        //  void attributeOwnershipDivestitureIfWanted
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSet const & attributes,
        //  AttributeHandleSet & theDivestedAttributes) // filled by RTI

        // // 7.13
        //  void cancelNegotiatedAttributeOwnershipDivestiture
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSet const & attributes)

        // // 7.14
        //  void cancelAttributeOwnershipAcquisition
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSet const & attributes)

        // // 7.16
        //  void queryAttributeOwnership
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandle attribute)

        // // 7.18
        //  bool isAttributeOwnedByFederate
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandle attribute)

        void enableTimeRegulation(const rti1516e::LogicalTimeInterval& logicalTimeInterval)
        {
            _ambassador->enableTimeRegulation(logicalTimeInterval);
        }

        void disableTimeRegulation()
        {
            _timeRegulationEnabled = false;
            _ambassador->disableTimeRegulation();
        }

        void enableTimeConstrained()
        {
            _ambassador->enableTimeConstrained();
        }

        void disableTimeConstrained()
        {
            _timeConstrainedEnabled = false;
            _ambassador->disableTimeConstrained();
        }

        void timeAdvanceRequest(const rti1516e::LogicalTime& logicalTime)
        {
            _timeAdvancePending = true;
            _ambassador->timeAdvanceRequest(logicalTime);
        }

        void timeAdvanceRequestAvailable(const rti1516e::LogicalTime& logicalTime)
        {
            _timeAdvancePending = true;
            _ambassador->timeAdvanceRequestAvailable(logicalTime);
        }

        void nextMessageRequest(const rti1516e::LogicalTime& logicalTime)
        {
            _timeAdvancePending = true;
            _ambassador->nextMessageRequest(logicalTime);
        }

        void nextMessageRequestAvailable(const rti1516e::LogicalTime& logicalTime)
        {
            _timeAdvancePending = true;
            _ambassador->nextMessageRequestAvailable(logicalTime);
        }

        void flushQueueRequest(const rti1516e::LogicalTime& logicalTime)
        {
            // _timeAdvancePending = true;
            _ambassador->flushQueueRequest(logicalTime);
        }

        void enableAsynchronousDelivery()
        {
            _ambassador->enableAsynchronousDelivery();
        }

        void disableAsynchronousDelivery()
        {
            _ambassador->disableAsynchronousDelivery();
        }

        //  bool queryGALT (LogicalTime & logicalTime)

        //  void queryLogicalTime (LogicalTime & logicalTime)

        //  bool queryLITS (LogicalTime & logicalTime)

        //  void modifyLookahead

        //  void queryLookahead (LogicalTimeInterval & interval)

        //  void retract
        // (MessageRetractionHandle theHandle)

        //  void changeAttributeOrderType
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSet const & attributes,
        //  OrderType theType)

        //  void changeInteractionOrderType
        // (InteractionClassHandle theClass,
        //  OrderType theType)

        //  RegionHandle createRegion
        // (DimensionHandleSet const & theDimensions)

        //  void commitRegionModifications
        // (RegionHandleSet const & regionHandleSet)

        //  void deleteRegion
        // (RegionHandle region)

        //  ObjectInstanceHandle registerObjectInstanceWithRegions
        // (ObjectClassHandle theClass,
        //  AttributeHandleSetRegionHandleSetPairVector const &
        //  attributeHandleSetRegionHandleSetPairVector)

        //  ObjectInstanceHandle registerObjectInstanceWithRegions
        // (ObjectClassHandle theClass,
        //  AttributeHandleSetRegionHandleSetPairVector const &
        //  attributeHandleSetRegionHandleSetPairVector,
        //  std::wstring const & objectInstanceName)

        //  void associateRegionsForUpdates
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSetRegionHandleSetPairVector const &
        //  attributeHandleSetRegionHandleSetPairVector)

        //  void unassociateRegionsForUpdates
        // (ObjectInstanceHandle objectInstanceHandle,
        //  AttributeHandleSetRegionHandleSetPairVector const &
        //  attributeHandleSetRegionHandleSetPairVector)

        //  void subscribeObjectClassAttributesWithRegions
        // (ObjectClassHandle theClass,
        //  AttributeHandleSetRegionHandleSetPairVector const &
        //  attributeHandleSetRegionHandleSetPairVector,
        //  bool active = true)

        //  void unsubscribeObjectClassAttributesWithRegions
        // (ObjectClassHandle theClass,
        //  AttributeHandleSetRegionHandleSetPairVector const &
        //  attributeHandleSetRegionHandleSetPairVector)

        //  void subscribeInteractionClassWithRegions
        // (InteractionClassHandle theClass,
        //  RegionHandleSet const & regionHandleSet,
        //  bool active = true)

        //  void unsubscribeInteractionClassWithRegions
        // (InteractionClassHandle theClass,
        //  RegionHandleSet const & regionHandleSet)

        //  void sendInteractionWithRegions
        // (InteractionClassHandle interaction,
        //  ParameterHandleValueMap const & parameterValues,
        //  RegionHandleSet const & regionHandleSet,
        //  VariableLengthData const & tag)

        //  MessageRetractionHandle sendInteractionWithRegions
        // (InteractionClassHandle interaction,
        //  ParameterHandleValueMap const & parameterValues,
        //  RegionHandleSet const & regionHandleSet,
        //  VariableLengthData const & tag,
        //  LogicalTime const & logicalTime)

        //  void requestAttributeValueUpdateWithRegions
        // (ObjectClassHandle theClass,
        //  AttributeHandleSetRegionHandleSetPairVector const & theSet,
        //  VariableLengthData const & tag)

        rti1516e::ObjectClassHandle getObjectClassHandle(std::wstring const& theName)
        {
            return _ambassador->getObjectClassHandle(theName);
        }

        std::wstring getObjectClassName(rti1516e::ObjectClassHandle theHandle)
        {
            return _ambassador->getObjectClassName(theHandle);
        }

        rti1516e::AttributeHandle getAttributeHandle(rti1516e::ObjectClassHandle whichClass, std::wstring const& attributeName)
        {
            return _ambassador->getAttributeHandle(whichClass, attributeName);
        }

        std::wstring getAttributeName(rti1516e::ObjectClassHandle whichClass, rti1516e::AttributeHandle theHandle)
        {
            return _ambassador->getAttributeName(whichClass, theHandle);
        }

        double getUpdateRateValue(std::wstring const& updateRateDesignator)
        {
            return _ambassador->getUpdateRateValue(updateRateDesignator);
        }

        double getUpdateRateValueForAttribute(rti1516e::ObjectInstanceHandle objectInstanceHandle, rti1516e::AttributeHandle attributeHandle)
        {
            return _ambassador->getUpdateRateValueForAttribute(objectInstanceHandle, attributeHandle);
        }

        rti1516e::InteractionClassHandle getInteractionClassHandle(std::wstring const& theName)
        {
            return _ambassador->getInteractionClassHandle(theName);
        }

        std::wstring getInteractionClassName(rti1516e::InteractionClassHandle theHandle)
        {
            return _ambassador->getInteractionClassName(theHandle);
        }

        rti1516e::ParameterHandle getParameterHandle(rti1516e::InteractionClassHandle whichClass, std::wstring const& theName)
        {
            return _ambassador->getParameterHandle(whichClass, theName);
        }

        std::wstring getParameterName(rti1516e::InteractionClassHandle whichClass, rti1516e::ParameterHandle theHandle)
        {
            return _ambassador->getParameterName(whichClass, theHandle);
        }

        rti1516e::ObjectInstanceHandle getObjectInstanceHandle(std::wstring const& theName)
        {
            return _ambassador->getObjectInstanceHandle(theName);
        }

        std::wstring getObjectInstanceName(rti1516e::ObjectInstanceHandle theHandle)
        {
            return _ambassador->getObjectInstanceName(theHandle);
        }

        rti1516e::DimensionHandle getDimensionHandle(std::wstring const& theName)
        {
            return _ambassador->getDimensionHandle(theName);
        }

        std::wstring getDimensionName(rti1516e::DimensionHandle theHandle)
        {
            return _ambassador->getDimensionName(theHandle);
        }

        unsigned long getDimensionUpperBound(rti1516e::DimensionHandle theHandle)
        {
            return _ambassador->getDimensionUpperBound(theHandle);
        }

        rti1516e::DimensionHandleSet getAvailableDimensionsForClassAttribute(rti1516e::ObjectClassHandle theClass,
            rti1516e::AttributeHandle theHandle)
        {
            return _ambassador->getAvailableDimensionsForClassAttribute(theClass, theHandle);
        }

        rti1516e::ObjectClassHandle getKnownObjectClassHandle(rti1516e::ObjectInstanceHandle object)
        {
            return _ambassador->getKnownObjectClassHandle(object);
        }

        rti1516e::DimensionHandleSet getAvailableDimensionsForInteractionClass(rti1516e::InteractionClassHandle theClass)
        {
            return _ambassador->getAvailableDimensionsForInteractionClass(theClass);
        }

        rti1516e::TransportationType getTransportationType(std::wstring const& transportationName)
        {
            return _ambassador->getTransportationType(transportationName);
        }

        std::wstring getTransportationName(rti1516e::TransportationType transportationType)
        {
            return _ambassador->getTransportationName(transportationType);
        }

        rti1516e::OrderType getOrderType(std::wstring const& orderName)
        {
            return _ambassador->getOrderType(orderName);
        }

        std::wstring getOrderName(rti1516e::OrderType orderType)
        {
            return _ambassador->getOrderName(orderType);
        }

        void enableObjectClassRelevanceAdvisorySwitch()
        {
            _ambassador->enableObjectClassRelevanceAdvisorySwitch();
        }

        void disableObjectClassRelevanceAdvisorySwitch()
        {
            _ambassador->disableObjectClassRelevanceAdvisorySwitch();
        }

        void enableAttributeRelevanceAdvisorySwitch()
        {
            _ambassador->enableAttributeRelevanceAdvisorySwitch();
        }

        void disableAttributeRelevanceAdvisorySwitch()
        {
            _ambassador->disableAttributeRelevanceAdvisorySwitch();
        }

        void enableAttributeScopeAdvisorySwitch()
        {
            _ambassador->enableAttributeScopeAdvisorySwitch();
        }

        void disableAttributeScopeAdvisorySwitch()
        {
            _ambassador->disableAttributeScopeAdvisorySwitch();
        }

        void enableInteractionRelevanceAdvisorySwitch()
        {
            _ambassador->enableInteractionRelevanceAdvisorySwitch();
        }

        void disableInteractionRelevanceAdvisorySwitch()
        {
            _ambassador->disableInteractionRelevanceAdvisorySwitch();
        }

        rti1516e::DimensionHandleSet getDimensionHandleSet(rti1516e::RegionHandle regionHandle)
        {
            return _ambassador->getDimensionHandleSet(regionHandle);
        }

        rti1516e::RangeBounds getRangeBounds(rti1516e::RegionHandle regionHandle, rti1516e::DimensionHandle theDimensionHandle)
        {
            return _ambassador->getRangeBounds(regionHandle, theDimensionHandle);
        }

        void setRangeBounds(rti1516e::RegionHandle regionHandle, rti1516e::DimensionHandle theDimensionHandle,
            rti1516e::RangeBounds const& rangeBounds)
        {
            return _ambassador->setRangeBounds(regionHandle, theDimensionHandle, rangeBounds);
        }

        unsigned long normalizeFederateHandle(rti1516e::FederateHandle federateHandle)
        {
            return _ambassador->normalizeFederateHandle(federateHandle);
        }

        unsigned long normalizeServiceGroup(rti1516e::ServiceGroup theServiceGroup)
        {
            return _ambassador->normalizeServiceGroup(theServiceGroup);
        }

        bool evokeCallback(double approximateMinimumTimeInSeconds)
        {
            return _ambassador->evokeCallback(approximateMinimumTimeInSeconds);
        }

        bool evokeMultipleCallbacks(double approximateMinimumTimeInSeconds,
            double approximateMaximumTimeInSeconds)
        {
            return _ambassador->evokeMultipleCallbacks(approximateMinimumTimeInSeconds, approximateMaximumTimeInSeconds);
        }

        void enableCallbacks()
        {
            _ambassador->enableCallbacks();
        }

        void disableCallbacks()
        {
            _ambassador->disableCallbacks();
        }

        rti1516e::FederateHandle decodeFederateHandle(rti1516e::VariableLengthData const& encodedValue) const
        {
            return _ambassador->decodeFederateHandle(encodedValue);
        }

        rti1516e::ObjectClassHandle decodeObjectClassHandle(rti1516e::VariableLengthData const& encodedValue) const
        {
            return _ambassador->decodeObjectClassHandle(encodedValue);
        }

        rti1516e::InteractionClassHandle decodeInteractionClassHandle(rti1516e::VariableLengthData const& encodedValue) const
        {
            return _ambassador->decodeInteractionClassHandle(encodedValue);
        }

        rti1516e::ObjectInstanceHandle decodeObjectInstanceHandle(rti1516e::VariableLengthData const& encodedValue) const
        {
            return _ambassador->decodeObjectInstanceHandle(encodedValue);
        }

        rti1516e::AttributeHandle decodeAttributeHandle(rti1516e::VariableLengthData const& encodedValue) const
        {
            return _ambassador->decodeAttributeHandle(encodedValue);
        }

        rti1516e::ParameterHandle decodeParameterHandle(rti1516e::VariableLengthData const& encodedValue) const
        {
            return _ambassador->decodeParameterHandle(encodedValue);
        }

        rti1516e::DimensionHandle decodeDimensionHandle(rti1516e::VariableLengthData const& encodedValue) const
        {
            return _ambassador->decodeDimensionHandle(encodedValue);
        }

        rti1516e::MessageRetractionHandle decodeMessageRetractionHandle(rti1516e::VariableLengthData const& encodedValue) const
        {
            return _ambassador->decodeMessageRetractionHandle(encodedValue);
        }

        rti1516e::RegionHandle decodeRegionHandle(rti1516e::VariableLengthData const& encodedValue) const
        {
            return _ambassador->decodeRegionHandle(encodedValue);
        }

    protected:

        //��������� � ������� �����
        virtual void connectionLost(const std::wstring& faultDescription)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:connectionLost", 0);

            //����� callback �������� ������� �� ������� �#
            std::string s(faultDescription.begin(), faultDescription.end());
            const char* tmsg = s.c_str();
            if (�onnectionLostCallbackFunction != nullptr)
            {
                �onnectionLostCallbackFunction(tmsg, (int)strlen(tmsg));
            }

        }

        //callback ����� ������ ListFederationExecutions ... ������� ������ ���������
        virtual void reportFederationExecutions(const rti1516e::FederationExecutionInformationVector& theFederationExecutionInformationList)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:reportFederationExecutions", 0);

            for (auto e : theFederationExecutionInformationList)
            {
                std::string s1(e.federationExecutionName.begin(), e.federationExecutionName.end());
                const char* tmsg1 = s1.c_str();
                std::string s2(e.logicalTimeImplementationName.begin(), e.logicalTimeImplementationName.end());
                const char* tmsg2 = s2.c_str();
                if (reportFederationExecutionsCallbackFunction != nullptr)
                {
                    reportFederationExecutionsCallbackFunction(tmsg1, tmsg2, (int)strlen(tmsg1), (int)strlen(tmsg2));
                }
            }
            
            
        }

        //��������� � ������� ������� ����������� ����� ������������� 
        virtual void synchronizationPointRegistrationSucceeded(const std::wstring& label)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:synchronizationPointRegistrationSucceeded", 0);

            //����� callback �������� ������� �� ������� �#
            std::string s(label.begin(), label.end());
            const char* tmsg = s.c_str();
            if (synchronizationPointRegistrationSucceededFunction != nullptr)
            {
                synchronizationPointRegistrationSucceededFunction(tmsg, (int)strlen(tmsg));
            }
            
        }

        //��������� � ��������� ������� ����������� ����� ������������� 
        virtual void synchronizationPointRegistrationFailed(const std::wstring& label, rti1516e::SynchronizationPointFailureReason reason)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:synchronizationPointRegistrationFailed", 0);

            std::wstring synchronizationPointFlag=L"";
            if (reason == rti1516e::SynchronizationPointFailureReason::SYNCHRONIZATION_POINT_LABEL_NOT_UNIQUE)
            {
                synchronizationPointFlag = L"SYNCHRONIZATION_POINT_LABEL_NOT_UNIQUE";
            }
            if (reason == rti1516e::SynchronizationPointFailureReason::SYNCHRONIZATION_SET_MEMBER_NOT_JOINED)
            {
                synchronizationPointFlag = L"SYNCHRONIZATION_SET_MEMBER_NOT_JOINED";
            }

            //����� callback �������� ������� �� ������� �#
            std::string s1(label.begin(), label.end());
            const char* tmsg1 = s1.c_str();
            std::string s2(synchronizationPointFlag.begin(), synchronizationPointFlag.end());
            const char* tmsg2 = s2.c_str();
            if (synchronizationPointRegistrationFailedFunction != nullptr)
            {
                synchronizationPointRegistrationFailedFunction(tmsg1, tmsg2,(int)strlen(tmsg1), (int)strlen(tmsg2));
            }
        }

        //��������� ��� ���� �� ��������� ��������� ����� �������������
        virtual void announceSynchronizationPoint(const std::wstring& label, const rti1516e::VariableLengthData& tag)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:announceSynchronizationPoint", 0);

            //����� callback �������� ������� �� ������� �#
            std::string s(label.begin(), label.end());
            const char* tmsg = s.c_str();
            if (announceSynchronizationPointCallbackFunction != nullptr)
            {
                announceSynchronizationPointCallbackFunction(tmsg, (int)strlen(tmsg));
            }

        }

        //��������� ��� ��������� ����������������
        virtual void federationSynchronized(const std::wstring& label, const rti1516e::FederateHandleSet& failedToSyncSet)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:federationSynchronized", 0);
            //
            std::wstring federationSynchronizedFlag = L"federationSynchronized";
            SendLog(L"federationSynchronized",0);

            //����� callback �������� ������� �� ������� �#
            std::string s(label.begin(), label.end());
            const char* tmsg = s.c_str();
            if (federationSynchronizedFunction != nullptr)
            {
                federationSynchronizedFunction(tmsg, (int)strlen(tmsg));
            }

            
        }

        virtual void initiateFederateSave(const std::wstring& label)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:initiateFederateSave", 0);
        }

        virtual void initiateFederateSave(const std::wstring& label, const rti1516e::LogicalTime& logicalTime)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:initiateFederateSave", 0);
        }

        virtual void federationSaved()
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:federationSaved", 0);
        }

        virtual void federationNotSaved(rti1516e::SaveFailureReason theSaveFailureReason)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:federationNotSaved", 0);
        }

        virtual void federationSaveStatusResponse(const rti1516e::FederateHandleSaveStatusPairVector& federateStatusVector)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:federationSaveStatusResponse", 0);
        }

        virtual void requestFederationRestoreSucceeded(const std::wstring& label)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:requestFederationRestoreSucceeded", 0);
        }

        virtual void requestFederationRestoreFailed(const std::wstring& label)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:requestFederationRestoreFailed", 0);
        }

        virtual void federationRestoreBegun()
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:federationRestoreBegun", 0);
        }

        virtual void initiateFederateRestore(const std::wstring& label, const std::wstring& federateName, rti1516e::FederateHandle handle)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:initiateFederateRestore", 0);
        }

        virtual void federationRestored()
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:federationRestored", 0);
        }

        virtual void federationNotRestored(rti1516e::RestoreFailureReason restoreFailureReason)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:federationNotRestored", 0);
        }

        virtual void federationRestoreStatusResponse(const rti1516e::FederateRestoreStatusVector& federateStatusVector)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:federationRestoreStatusResponse", 0);
        }

        virtual void startRegistrationForObjectClass(rti1516e::ObjectClassHandle)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:startRegistrationForObjectClass", 0);
        }

        virtual void stopRegistrationForObjectClass(rti1516e::ObjectClassHandle)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:stopRegistrationForObjectClass", 0);
        }

        virtual void turnInteractionsOn(rti1516e::InteractionClassHandle)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:turnInteractionsOn", 0);
        }

        virtual void turnInteractionsOff(rti1516e::InteractionClassHandle)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:turnInteractionsOff", 0);
        }

        virtual void objectInstanceNameReservationSucceeded(const std::wstring& name)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"objectInstanceNameReservationSucceeded, name =" + name, 0);
        }

        virtual void objectInstanceNameReservationFailed(const std::wstring& name)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"objectInstanceNameReservationFailed, name =" + name, 0);
        }

        virtual void multipleObjectInstanceNameReservationSucceeded(const std::set<std::wstring>& theObjectInstanceNames)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:multipleObjectInstanceNameReservationSucceeded", 0);
        }

        virtual void multipleObjectInstanceNameReservationFailed(const std::set<std::wstring>& theObjectInstanceNames)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:multipleObjectInstanceNameReservationFailed", 0);
        }

        virtual void discoverObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::ObjectClassHandle objectClassHandle,
            const std::wstring& objectInstanceName)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            
            SendLog(L"DEBUG:discoverObjectInstance", 0);
            SendLog(L"DEBUG:objectClassHandle =" + objectClassHandle.toString(), 0);
            SendLog(L"DEBUG:objectInstanceHandle =" + objectInstanceHandle.toString(), 0);
            SendLog(L"DEBUG:objectInstanceName =" + objectInstanceName, 0);

            //Log(Assert, Error) << "discover "  << objectClassHandle.toString() << " " << objectInstanceHandle.toString() << std::endl;
            
            
            // 
            // ObjectClassAttributeHandleSetMap::iterator i = _subscribedObjectClassAttributeHandleSetMap.find(objectClassHandle);
            // if (i == _subscribedObjectClassAttributeHandleSetMap.end()) {
            //     fail();
            //     throw rti1516e::FederateInternalError(objectClassHandle.toString());
            // }

            // if (_objectInstanceMap.find(objectInstanceHandle) != _objectInstanceMap.end()) {
            //     fail();
            //     throw rti1516e::FederateInternalError(objectInstanceHandle.toString());
            // }

            // _objectInstanceMap[objectInstanceHandle]._objectClassHandle = objectClassHandle;
        }

        virtual void discoverObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::ObjectClassHandle objectClassHandle, const std::wstring& objectInstanceName,
            rti1516e::FederateHandle producingFederate)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:discoverObjectInstance 2", 0);
            discoverObjectInstance(objectInstanceHandle, objectClassHandle, objectInstanceName);
        }

        virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            const rti1516e::AttributeHandleValueMap& attributeHandleValueMap,
            const rti1516e::VariableLengthData& tag, rti1516e::OrderType, rti1516e::TransportationType,
            rti1516e::SupplementalReflectInfo info)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:reflectAttributeValues, objectInstanceHandle =" + objectInstanceHandle.toString(), 0);
            // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);

            //if (getFederateHandle().encode() != tag)

            // for (auto it = attributeHandleValueMap.begin(); it != attributeHandleValueMap.end(); ++it)
            for (auto z : attributeHandleValueMap)
            {
                SendLog(L"DEBUG:attribute handle =" + z.first.toString(), 0);
                //if (it->first == DataHandle) //���������� ��������
 
                size_t len = z.second.size();
                
                //const char* p = (char*)z.second.data();
                // char* temptemp = new char[90]; //len
                //memcpy(temptemp, z.second.data(), len); // memcpy(&temptemp, z.second.data(), len);
                //wcout << "    for the handle " << (it->first).toString() << " value = " << d << endl;
                //std::wstringstream cls;
                //cls << temptemp;
                //cls << p;
                //std::wstring value = cls.str();
                //delete temptemp;

                //������ � ������
                std::wostringstream myStringStream;
                myStringStream << len;
                std::wstring concatenatedStr = myStringStream.str();
                SendLog(L"DEBUG:len in bytes = " + concatenatedStr, 0);
                
                //�������������� void* � wstring ������� �1
                std::wstring string_Data1;
                string_Data1.resize(len);
                std::memcpy(&string_Data1[0], z.second.data(), len);
                SendLog(L"DEBUG:value1 =" + string_Data1, 0);

                //�������������� void* � wstring ������� �2
                std::wstring string_Data2;
                string_Data2.assign(reinterpret_cast<const wchar_t*>(z.second.data()), len/2);
                SendLog(L"DEBUG:value2 =" + string_Data2, 0);

                //tag
                //size_t taglen = tag.size();
                //// std::wstring string_tag1;
                //string_tag1.resize(taglen);
                //std::memcpy(&string_tag1[0], tag.data(), len);
                //SendLog(L"DEBUG:tag =" + string_tag1, 0);

                if (info.hasProducingFederate == true)
                {
                    SendLog(L"DEBUG:info.producingFederate =" + info.producingFederate.toString(),0);
                }
                
            }



        }

        virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            const rti1516e::AttributeHandleValueMap& attributeHandleValueMap,
            const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
            const rti1516e::RegionHandleSet&, rti1516e::SupplementalReflectInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:reflectAttributeValues 2", 0);
            // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
        }

        virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            const rti1516e::AttributeHandleValueMap& attributeHandleValueMap,
            const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
            const rti1516e::LogicalTime&, rti1516e::OrderType, rti1516e::SupplementalReflectInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:reflectAttributeValues 3", 0);
            // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
        }

        virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            const rti1516e::AttributeHandleValueMap& attributeHandleValueMap,
            const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
            const rti1516e::LogicalTime&, rti1516e::OrderType, const rti1516e::RegionHandleSet&,
            rti1516e::SupplementalReflectInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:reflectAttributeValues 4", 0);
            // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
        }

        virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            const rti1516e::AttributeHandleValueMap& attributeHandleValueMap,
            const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
            const rti1516e::LogicalTime&, rti1516e::OrderType, rti1516e::MessageRetractionHandle,
            rti1516e::SupplementalReflectInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:reflectAttributeValues 5", 0);
            // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
        }

        virtual void reflectAttributeValues(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            const rti1516e::AttributeHandleValueMap& attributeHandleValueMap,
            const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
            const rti1516e::LogicalTime&, rti1516e::OrderType, rti1516e::MessageRetractionHandle,
            const rti1516e::RegionHandleSet&, rti1516e::SupplementalReflectInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:reflectAttributeValues 6", 0);
            // _verifyReflectAttributeValues(objectInstanceHandle, attributeHandleValueMap);
        }

        virtual void receiveInteraction(rti1516e::InteractionClassHandle interactionClassHandle, const rti1516e::ParameterHandleValueMap& data1,
            const rti1516e::VariableLengthData& data, rti1516e::OrderType, rti1516e::TransportationType,
            rti1516e::SupplementalReceiveInfo theReceiveInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            //if (interactionClassHandle != _requestInteractionClassHandle) 
            //{
            //    std::wcout << L"Received interaction class that was not subscribed!" << std::endl;
            //}

            //if (getFederateHandle().encode() != tag)

            SendLog(L"DEBUG:receiveInteraction1, interactionClassHandle=" + interactionClassHandle.toString(), 0);

            for (auto z : data1)
            {
                SendLog(L"DEBUG:parameter handle=", 0);
                SendLog(z.first.toString(), 0);

                size_t len = z.second.size();
                //const char* p = (char*)z.second.data();

                //������ � ������
                std::wostringstream myStringStream;
                myStringStream << len;
                std::wstring concatenatedStr = myStringStream.str();
                SendLog(L"DEBUG:len in bytes =" + concatenatedStr, 0);
                
                //�������������� void* � wstring ������� �1
                std::wstring string_Data1;
                string_Data1.resize(len);
                std::memcpy(&string_Data1[0], z.second.data(), len);
                SendLog(L"DEBUG:value1 =" + string_Data1, 0);

                //�������������� void* � wstring ������� �2
                std::wstring string_Data2;
                string_Data2.assign(reinterpret_cast<const wchar_t*>(z.second.data()), len/2);
                SendLog(L"DEBUG:value2 =" + string_Data2, 0);
            }

            //PyByteArray_FromStringAndSize(static_cast<const char*>(variableLengthData.data()), variableLengthData.size())

            //����� callback �������� ������� �� ������� �#
            //std::string s(label.begin(), label.end());
            //const char* tmsg = s.c_str();
            //if (federationSynchronizedFunction != nullptr)
            //{
            //    federationSynchronizedFunction(tmsg, (int)strlen(tmsg));
            //}


        }

        virtual void receiveInteraction(rti1516e::InteractionClassHandle, const rti1516e::ParameterHandleValueMap&,
            const rti1516e::VariableLengthData&, rti1516e::OrderType, rti1516e::TransportationType,
            const rti1516e::RegionHandleSet&, rti1516e::SupplementalReceiveInfo theReceiveInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:receiveInteraction2", 0);
        }

        virtual void receiveInteraction(rti1516e::InteractionClassHandle interaction,
            rti1516e::ParameterHandleValueMap const& parameterValues,
            rti1516e::VariableLengthData const& tag,
            rti1516e::OrderType sentOrder,
            rti1516e::TransportationType theType,
            rti1516e::LogicalTime const& logicalTime,
            rti1516e::OrderType receivedOrder,
            rti1516e::SupplementalReceiveInfo theReceiveInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:receiveInteraction3", 0);
        }

        virtual void receiveInteraction(rti1516e::InteractionClassHandle interaction,
            rti1516e::ParameterHandleValueMap const& parameterValues,
            rti1516e::VariableLengthData const& tag,
            rti1516e::OrderType sentOrder,
            rti1516e::TransportationType theType,
            rti1516e::LogicalTime const& logicalTime,
            rti1516e::OrderType receivedOrder,
            rti1516e::RegionHandleSet const& theSentRegionHandleSet,
            rti1516e::SupplementalReceiveInfo theReceiveInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:receiveInteraction4", 0);
        }

        virtual void receiveInteraction(rti1516e::InteractionClassHandle interaction,
            rti1516e::ParameterHandleValueMap const& parameterValues,
            rti1516e::VariableLengthData const& tag,
            rti1516e::OrderType sentOrder,
            rti1516e::TransportationType theType,
            rti1516e::LogicalTime const& logicalTime,
            rti1516e::OrderType receivedOrder,
            rti1516e::MessageRetractionHandle theHandle,
            rti1516e::SupplementalReceiveInfo theReceiveInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:receiveInteraction5", 0);
        }

        virtual void receiveInteraction(rti1516e::InteractionClassHandle interaction,
            rti1516e::ParameterHandleValueMap const& parameterValues,
            rti1516e::VariableLengthData const& tag,
            rti1516e::OrderType sentOrder,
            rti1516e::TransportationType theType,
            rti1516e::LogicalTime const& logicalTime,
            rti1516e::OrderType receivedOrder,
            rti1516e::MessageRetractionHandle theHandle,
            rti1516e::RegionHandleSet const& theSentRegionHandleSet,
            rti1516e::SupplementalReceiveInfo theReceiveInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:receiveInteraction6", 0);
        }

        virtual void removeObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::VariableLengthData const& tag,
            rti1516e::OrderType sentOrder,
            rti1516e::SupplementalRemoveInfo theRemoveInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:removeObjectInstance, objectInstanceHandle=" + objectInstanceHandle.toString(), 0);
            // _verifyRemoveObjectInstance(objectInstanceHandle);
        }

        virtual void removeObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::VariableLengthData const& tag,
            rti1516e::OrderType sentOrder,
            rti1516e::LogicalTime const& logicalTime,
            rti1516e::OrderType receivedOrder,
            rti1516e::SupplementalRemoveInfo theRemoveInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:removeObjectInstance 2", 0);
            // _verifyRemoveObjectInstance(objectInstanceHandle);
        }

        virtual void removeObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::VariableLengthData const& tag,
            rti1516e::OrderType sentOrder,
            rti1516e::LogicalTime const& logicalTime,
            rti1516e::OrderType receivedOrder,
            rti1516e::MessageRetractionHandle theHandle,
            rti1516e::SupplementalRemoveInfo theRemoveInfo)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:removeObjectInstance 3", 0);
            // _verifyRemoveObjectInstance(objectInstanceHandle);
        }

        virtual void attributesInScope(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& attributes)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:attributesInScope", 0);
        }

        virtual void attributesOutOfScope(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& attributes)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:attributesOutOfScope", 0);
        }

        virtual void provideAttributeValueUpdate(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& attributes,
            rti1516e::VariableLengthData const& tag)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:provideAttributeValueUpdate", 0);
        }

        virtual void turnUpdatesOnForObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& attributes)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:turnUpdatesOnForObjectInstance", 0);
        }

        virtual void turnUpdatesOnForObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& attributes,
            const std::wstring& updateRateDesignator)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:turnUpdatesOnForObjectInstance", 0);
        }

        virtual void turnUpdatesOffForObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& attributes)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:turnUpdatesOffForObjectInstance", 0);
        }

        virtual void confirmAttributeTransportationTypeChange(rti1516e::ObjectInstanceHandle theObject,
            rti1516e::AttributeHandleSet theAttributes,
            rti1516e::TransportationType theTransportation)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:confirmAttributeTransportationTypeChange", 0);
        }

        virtual void reportAttributeTransportationType(rti1516e::ObjectInstanceHandle theObject,
            rti1516e::AttributeHandle theAttribute,
            rti1516e::TransportationType theTransportation)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:reportAttributeTransportationType", 0);
        }

        virtual void confirmInteractionTransportationTypeChange(rti1516e::InteractionClassHandle theInteraction,
            rti1516e::TransportationType theTransportation)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:confirmInteractionTransportationTypeChange", 0);
        }

        virtual void reportInteractionTransportationType(rti1516e::FederateHandle federateHandle,
            rti1516e::InteractionClassHandle theInteraction,
            rti1516e::TransportationType theTransportation)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:reportInteractionTransportationType", 0);
        }

        virtual void requestAttributeOwnershipAssumption(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& offeredAttributes,
            rti1516e::VariableLengthData const& tag)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:requestAttributeOwnershipAssumption", 0);
        }

        virtual void requestDivestitureConfirmation(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& releasedAttributes)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:requestDivestitureConfirmation", 0);
        }

        virtual void attributeOwnershipAcquisitionNotification(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& securedAttributes,
            rti1516e::VariableLengthData const& tag)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:attributeOwnershipAcquisitionNotification", 0);
        }

        virtual void attributeOwnershipUnavailable(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& attributes)
            RTI_THROW((rti1516e::FederateInternalError))
        {
        }

        virtual void requestAttributeOwnershipRelease(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& candidateAttributes,
            rti1516e::VariableLengthData const& tag)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:requestAttributeOwnershipRelease", 0);
        }

        virtual void confirmAttributeOwnershipAcquisitionCancellation(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandleSet const& attributes)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:confirmAttributeOwnershipAcquisitionCancellation", 0);
        }

        virtual void informAttributeOwnership(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandle attribute,
            rti1516e::FederateHandle owner)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:informAttributeOwnership", 0);
        }

        virtual void attributeIsNotOwned(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandle attribute)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:attributeIsNotOwned", 0);
        }

        virtual void attributeIsOwnedByRTI(rti1516e::ObjectInstanceHandle objectInstanceHandle,
            rti1516e::AttributeHandle attribute)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:attributeIsOwnedByRTI", 0);
        }

        virtual void timeRegulationEnabled(const rti1516e::LogicalTime& logicalTime)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            _timeRegulationEnabled = true;
            *_grantedLogicalTime = logicalTime;
            SendLog(L"DEBUG:timeRegulationEnabled", 0);
        }

        virtual void timeConstrainedEnabled(const rti1516e::LogicalTime& logicalTime)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            _timeConstrainedEnabled = true;
            *_grantedLogicalTime = logicalTime;
            SendLog(L"DEBUG:timeConstrainedEnabled", 0);
        }

        virtual void timeAdvanceGrant(const rti1516e::LogicalTime& logicalTime)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            _timeAdvancePending = false;
            *_grantedLogicalTime = logicalTime;
            SendLog(L"DEBUG:timeAdvanceGrant", 0);
        }

        virtual void requestRetraction(rti1516e::MessageRetractionHandle theHandle)
            RTI_THROW((rti1516e::FederateInternalError))
        {
            SendLog(L"DEBUG:requestRetraction", 0);
        }

        // void fail()
        // { _fail = true; }

    private:
        void _replaceFileWithDataIfNeeded(std::wstring& fddFile)
        {
            if (!_useDataUrlObjectModels)
                return;
            // already a data url
            if (fddFile.compare(0, 5, L"data:") == 0)
                return;
            std::wifstream stream;
            if (fddFile.compare(0, 8, L"file:///") == 0)
                stream.open(OpenRTI::ucsToLocale(fddFile.substr(8)).c_str());
            else
                stream.open(OpenRTI::ucsToLocale(fddFile).c_str());
            if (!stream.is_open())
                return;
            fddFile = L"data:,";
            std::copy(std::istreambuf_iterator<wchar_t>(stream), std::istreambuf_iterator<wchar_t>(), std::back_inserter(fddFile));
        }

        void _replaceFilesWithDataIfNeeded(std::vector<std::wstring>& fomModules)
        {
            for (std::vector<std::wstring>::iterator i = fomModules.begin(); i != fomModules.end(); ++i)
                _replaceFileWithDataIfNeeded(*i);
        }

        // bool _verifyGrantedLogicalTime(const rti1516e::LogicalTime& logicalTime) const
        // { return logicalTime == *_grantedLogicalTime; }

        // void _verifyReflectAttributeValues(const rti1516e::ObjectInstanceHandle& objectInstanceHandle,
        //                                    const rti1516e::AttributeHandleValueMap& attributeHandleValueMap)
        // {
        //     ObjectInstanceMap::iterator i = _objectInstanceMap.find(objectInstanceHandle);
        //     if (i == _objectInstanceMap.end()) {
        //         fail();
        //         throw rti1516e::ObjectInstanceNotKnown(objectInstanceHandle.toString());
        //     }

        //     ObjectClassAttributeHandleSetMap::iterator j = _subscribedObjectClassAttributeHandleSetMap.find(i->second._objectClassHandle);
        //     for (rti1516e::AttributeHandleValueMap::const_iterator k = attributeHandleValueMap.begin();
        //          k != attributeHandleValueMap.end(); ++k) {
        //         if (j->second.find(k->first) != j->second.end())
        //             continue;
        //         fail();
        //         throw rti1516e::AttributeNotSubscribed(k->first.toString());
        //     }
        // }

        // void _verifyRemoveObjectInstance(rti1516e::ObjectInstanceHandle objectInstanceHandle)
        // {
        //     if (_objectInstanceMap.find(objectInstanceHandle) == _objectInstanceMap.end()) {
        //         fail();
        //         throw rti1516e::ObjectInstanceNotKnown(objectInstanceHandle.toString());
        //     }

        //     _objectInstanceMap.erase(objectInstanceHandle);
        // }

        // bool _fail;

        RTI_UNIQUE_PTR<rti1516e::RTIambassador> _ambassador;

        bool _useDataUrlObjectModels;

        std::wstring _logicalTimeImplementationName;
        RTI_UNIQUE_PTR<rti1516e::LogicalTimeFactory> _logicalTimeFactory;

        rti1516e::FederateHandle _federateHandle;

        RTI_UNIQUE_PTR<rti1516e::LogicalTime> _grantedLogicalTime;
        bool _timeRegulationEnabled;
        bool _timeConstrainedEnabled;
        bool _timeAdvancePending;

        // Hmm, FIXME: make an additional derived checking ambassador for the tests, keep a simple one without expensive tests
        // FIXME make this and for example the simple log below callbacks that we can attach or not as apropriate

        // // FIXME implement subscription tracking also for regions at some time
        // typedef std::map<rti1516e::ObjectClassHandle, rti1516e::AttributeHandleSet> ObjectClassAttributeHandleSetMap;
        // ObjectClassAttributeHandleSetMap _subscribedObjectClassAttributeHandleSetMap;

        // struct ObjectInstance {
        //   rti1516e::ObjectClassHandle _objectClassHandle;
        // };

        // typedef std::map<rti1516e::ObjectInstanceHandle, ObjectInstance> ObjectInstanceMap;
        // ObjectInstanceMap _objectInstanceMap;
    };
}
#endif