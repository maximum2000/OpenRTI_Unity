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
   File: RTI/LogicalTimeInterval.h
***********************************************************************/

#ifndef RTI_LogicalTimeInterval_h
#define RTI_LogicalTimeInterval_h

// The classes associated with logical time allow a federation to provide
// their own representation for logical time and logical time interval. The
// federation is responsible to inherit from the abstract classes declared
// below. The encoded time classes are used to hold the arbitrary bit
// representation of the logical time and logical time intervals.

namespace rti1516e
{
   class LogicalTime;
}

#include <RTI/SpecificConfig.h>
#include <RTI/Exception.h>
#include <string>
#include <RTI/VariableLengthData.h>

namespace rti1516e
{
   class RTI_EXPORT LogicalTimeInterval
   {
   public:
      // Destructor
      virtual ~LogicalTimeInterval ()
         RTI_NOEXCEPT = 0;

      // Basic accessors/mutators

      virtual void setZero () = 0;

      virtual bool isZero () const = 0;

      virtual void setEpsilon () = 0;

      virtual bool isEpsilon () const = 0;

      // Operators

      virtual LogicalTimeInterval & operator= (
         LogicalTimeInterval const & value)
         RTI_THROW ((
            InvalidLogicalTimeInterval)) = 0;

      virtual LogicalTimeInterval & operator+= (
         LogicalTimeInterval const & addend)
         RTI_THROW ((
            IllegalTimeArithmetic,
            InvalidLogicalTimeInterval)) = 0;

      virtual LogicalTimeInterval & operator-= (
         LogicalTimeInterval const & subtrahend)
         RTI_THROW ((
            IllegalTimeArithmetic,
            InvalidLogicalTimeInterval)) = 0;

      virtual bool operator> (
         LogicalTimeInterval const & value) const
         RTI_THROW ((
            InvalidLogicalTimeInterval)) = 0;

      virtual bool operator< (
         LogicalTimeInterval const & value) const
         RTI_THROW ((
            InvalidLogicalTimeInterval)) = 0;

      virtual bool operator== (
         LogicalTimeInterval const & value) const
         RTI_THROW ((
            InvalidLogicalTimeInterval)) = 0;

      virtual bool operator>= (
         LogicalTimeInterval const & value) const
         RTI_THROW ((
            InvalidLogicalTimeInterval)) = 0;

      virtual bool operator<= (
         LogicalTimeInterval const & value) const
         RTI_THROW ((
            InvalidLogicalTimeInterval)) = 0;

      // Set self to the difference between two LogicalTimes
      virtual void setToDifference (
         LogicalTime const & minuend,
         LogicalTime const& subtrahend)
         RTI_THROW ((
            IllegalTimeArithmetic,
            InvalidLogicalTime)) = 0;

      // Generates an encoded value that can be used to send
      // LogicalTimeIntervals to other federates in updates or interactions
      virtual VariableLengthData encode () const = 0;

      // Alternate encode for directly filling a buffer
      virtual size_t encode (
         void* buffer,
         size_t bufferSize) const
         RTI_THROW ((
            CouldNotEncode)) = 0;

      // The length of the encoded data
      virtual size_t encodedLength () const = 0;

      // Decode encodedValue into self
      virtual void decode (
         VariableLengthData const & encodedValue)
         RTI_THROW ((
            InternalError,
            CouldNotDecode)) = 0;

      // Alternate decode that reads directly from a buffer
      virtual void decode (
         void* buffer,
         size_t bufferSize)
         RTI_THROW ((
            InternalError,
            CouldNotDecode)) = 0;

      // Diagnostic string representation of time interval
      virtual std::wstring toString () const = 0;

      // Return the name of the implementation, as needed by
      // createFederationExecution.
      virtual std::wstring implementationName () const = 0;
   };

   // Output operator for LogicalTimeInterval
   std::wostream RTI_EXPORT & operator << (
      std::wostream &,
      LogicalTimeInterval const &);
}
#endif // RTI_LogicalTimeInterval_h
