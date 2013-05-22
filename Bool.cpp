//------------------------------------------------------------------------
// File:      Bool.cpp
// Functions: ostream& operator<<(ostream& sout, const Bool& object)
//------------------------------------------------------------------------
#include "Bool.h"

namespace JO_UTILS
{

    //--------------------------------------------------------------------
    // Function:    operator<<(ostream& sout, const Bool& object)
    // Description: output operator for Bool
    // Programmers: Jacob O'Bryant
    // Version:     1.0
    // Environment: Intel Pentium PC, Arch Linux, gcc 4.7
    // Output:      object to sout
    // Parameters:  ostream& sout:      the output stream
    //              const Bool& object: th object
    // Returns:     sout
    // History:     4/17/2013 JO finished v1.0
    //--------------------------------------------------------------------
    ostream& operator<<(ostream& sout, const Bool& object)
    {
        object.print(sout);
        return sout;
    }
}
