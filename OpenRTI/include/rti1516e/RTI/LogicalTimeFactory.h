/***********************************************************************
   The IEEE hereby grants a general, royalty-free license to copy, distribute,
   display and make derivative works from this material, for all purposes,
   provided that any use of the material contains the following
   attribution: "Reprinted with permission from IEEE 1516.1(TM)-2010".
   Should you require additional information, contact the Manager, Standards
   Intellectual Property, IEEE Standards Association (stds-ipr@ieee.org).
***********************************************************************/
/***********************************************************************
   IEEE 1516.1 High Level Architecture Interface Specification C++ API
   File: RTI/LogicalTimeFactory.h
***********************************************************************/

#ifndef RTI_LogicalTimeFactory_h
#define RTI_LogicalTimeFactory_h

namespace rti1516e
{
   class LogicalTime;
   class LogicalTimeInterval;
   class VariableLengthData;
}

#include <RTI/SpecificConfig.h>
#include <RTI/Exception.h>
#include <string>
#include <memory>

// LogicalTimeFactory is used by the RTI to construct instances of classes
// derived from LogicalTime and LogicalTimeInterval.  A federation is responsible
// for providing a fedtime library that includes one or more subclasses
// of LogicalTime and LogicalTimeInterval, one or more subclasses of LogicalTimeFactory
// (which is used to create instances of those LogicalTime and LogicalTimeInterval
// subclasses), and a single implementation of
// LogicalTimeFactoryFactory::makeLogicalTimeFactory.  This static function should
// choose a LogicalTimeFactory based on the string identifier passed as an argument,
// and return an instance of that kind of factory.  The RTI will call this function to
// obtain a LogicalTimeFactory for a federation, and then will use that factory to create
// any instances of LogicalTime or LogicalTimeInterval that it needs.

// All RTIs shall implement a reference time library with time types named HLAinteger64Time
// and HLAfloat64Time.  The interfaces for these types shall be found in the time subdirectory.

namespace rti1516e
{
   class RTI_EXPORT LogicalTimeFactory
   {
   public:
      virtual ~LogicalTimeFactory ()
         RTI_NOEXCEPT = 0;

      // Return a LogicalTime with a value of "initial"
      virtual RTI_UNIQUE_PTR< LogicalTime > makeInitial()
         RTI_THROW ((
            InternalError)) = 0;

      // Return a LogicalTime with a value of "final"
      virtual RTI_UNIQUE_PTR< LogicalTime > makeFinal()
         RTI_THROW ((
            InternalError)) = 0;

      // Return a LogicalTimeInterval with a value of "zero"
      virtual RTI_UNIQUE_PTR< LogicalTimeInterval > makeZero()
         RTI_THROW ((
            InternalError)) = 0;

      // Return a LogicalTimeInterval with a value of "epsilon"
      virtual RTI_UNIQUE_PTR< LogicalTimeInterval > makeEpsilon()
         RTI_THROW ((
            InternalError)) = 0;

      // LogicalTime decode from an encoded LogicalTime
      virtual RTI_UNIQUE_PTR< LogicalTime > decodeLogicalTime (
         VariableLengthData const & encodedLogicalTime)
         RTI_THROW ((
            InternalError,
            CouldNotDecode)) = 0;

      // Alternate LogicalTime decode that reads directly from a buffer
      virtual RTI_UNIQUE_PTR< LogicalTime > decodeLogicalTime (
         void* buffer,
         size_t bufferSize)
         RTI_THROW ((
            InternalError,
            CouldNotDecode)) = 0;

      // LogicalTimeInterval decode from an encoded LogicalTimeInterval
      virtual RTI_UNIQUE_PTR< LogicalTimeInterval > decodeLogicalTimeInterval (
         VariableLengthData const & encodedValue)
         RTI_THROW ((
            InternalError,
            CouldNotDecode)) = 0;

      // Alternate LogicalTimeInterval decode that reads directly from a buffer
      virtual RTI_UNIQUE_PTR< LogicalTimeInterval > decodeLogicalTimeInterval (
         void* buffer,
         size_t bufferSize)
         RTI_THROW ((
            InternalError,
            CouldNotDecode)) = 0;

      // Return the name of the logical time implementation
      virtual std::wstring getName () const = 0;
   };

   class RTI_EXPORT HLAlogicalTimeFactoryFactory
   {
   public:

      // Provides a factory for the standard logical time types HLAfloat64Time
      // and HLAinteger64Time. The RTI reference time library's
      // LogicalTimeFactoryFactory should just forward requests to here.
      static RTI_UNIQUE_PTR< LogicalTimeFactory > makeLogicalTimeFactory (
         std::wstring const & implementationName);
   };
}

namespace rti1516e
{
   class RTI_EXPORT_FEDTIME LogicalTimeFactoryFactory
   {
   public:

      // The name is used to choose among several LogicalTimeFactories that might
      // be present in the fedtime library.  Each federation chooses its
      // implementation by passing the appropriate name to createFederationExecution.
      // If the supplied name is the empty string, a default LogicalTimeFactory is
      // returned.  If the supplied implementation name does not match any name
      // supported by the library, then a NULL pointer is returned.
      static RTI_UNIQUE_PTR< LogicalTimeFactory > makeLogicalTimeFactory (
         std::wstring const & implementationName);
   };
}

#endif // RTI_LogicalTimeFactory_h
