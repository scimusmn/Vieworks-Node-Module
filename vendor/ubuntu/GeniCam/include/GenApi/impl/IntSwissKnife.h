//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header$
//
//  License: This file is published under the license of the EMVA GenICam  Standard Group.
//  A text file describing the legal terms is included in  your installation as 'GenICam_license.pdf'.
//  If for some reason you are missing  this file please contact the EMVA or visit the website
//  (http://www.genicam.org) for a full copy.
//
//  THIS SOFTWARE IS PROVIDED BY THE EMVA GENICAM STANDARD GROUP "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE EMVA GENICAM STANDARD  GROUP
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT  LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  DATA, OR PROFITS;
//  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY  THEORY OF LIABILITY,
//  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
/*!
\file
\brief    Definition of CIntSwissKnife
\ingroup GenApi_Implementation
*/

#ifndef GENAPI_INTSWISSKNIFE_H
#define GENAPI_INTSWISSKNIFE_H

#include "MathParser/Int64MathParser.h"
#include "PolyReference.h"
#include "Node.h"
#include "BaseT.h"
#include "ValueT.h"
#include "IntegerT.h"
#include "NodeT.h"
#include <map>

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{
    //*************************************************************
    // CIntSwissKnife class
    //*************************************************************

    /**
    * \ingroup internal_impl
    *
    * \brief  IInteger implementation for a SwissKnife used for formula evaluation
    */
    class GENAPI_DECL CIntSwissKnifeImpl : public IInteger, public CNodeImpl
    {
    public:
        //! Constructor
        CIntSwissKnifeImpl();

        //! Destructor
        virtual ~CIntSwissKnifeImpl();

    public:

        //! Implementation of IBase::GetPrincipalInterfaceType()
        virtual EInterfaceType InternalGetPrincipalInterfaceType() const
        {
            return intfIInteger;
        }

        //-------------------------------------------------------------
        // INodePrivate implementation
        //-------------------------------------------------------------
        virtual void FinalConstruct();

        //-------------------------------------------------------------
        // IInteger implementation
        //-------------------------------------------------------------

        //! Get feature value usingt m_InputName as hard coded variable name
        /*! This is a helper for the implementation of the converter */
        virtual int64_t GetValueWithInput(int64_t input, bool Verify = false, bool IgnoreCache = false);


    protected:
        // Get access mode
        virtual EAccessMode InternalGetAccessMode() const;

        //! Set feature value
        #pragma BullseyeCoverage off
        virtual void InternalSetValue(int64_t /*Value*/, bool /* Verify = true */)
        {
            throw ACCESS_EXCEPTION_NODE("IntSwissKnife is read only.");
        }
        #pragma BullseyeCoverage on

        //! Get feature value
        virtual int64_t InternalGetValue(bool Verify = false, bool IgnoreCache = false);


        //! Get minimum value allowed
        virtual int64_t InternalGetMin() const
        {
            return m_Min;
        }

        //! Get maximum value allowed
        virtual int64_t InternalGetMax() const
        {
            return m_Max;
        }

        //! Get increment
        virtual int64_t InternalGetInc() const
        {
            return 1LL;
        }

        //! Get recommended representation
        virtual  ERepresentation InternalGetRepresentation() const
        {
            if( m_Representation != _UndefinedRepresentation )
                return m_Representation;
            else
                return PureNumber;
        }

        //! Get the unit
        virtual GenICam::gcstring InternalGetUnit()
        {
            return m_Unit;
        }

        //! Get Caching Mode
        virtual ECachingMode InternalGetCachingMode() const;

    public:
        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
        //! Convert map
        BEGIN_CONVERT_MAP
            CHAIN_CONVERT_MAP(CNodeImpl)
            SWITCH_CONVERT_MAP

            /**********************************************/
            // special treatment of variables
            /**********************************************/
            case pVariable_ID:
            {
                if (Direction == In)
                {
                    // store the symbolic name of the variable
                    m_Symbolics.insert(std::pair<GenICam::gcstring, GenICam::gcstring>(ValueStrIn, AttributeStrIn));

                    // prepare the variable pointer
                    INode * const pNode = m_pNodeMap->GetNode(ValueStrIn);
                    if(!pNode)
                        throw PROPERTY_EXCEPTION_NODE("property '%s' : node '%s' does not exist.", PropertyName, ValueStrIn );
                    AddChild(pNode );

                    // store the pointer to the variables
                    CIntegerPolyRef Value;
                    Value = dynamic_cast<IBase*>(pNode);
                    m_Variables.insert(std::pair<GenICam::gcstring, CIntegerPolyRef>(AttributeStrIn, Value));
                    return true;
                }
                else if (Direction == Out)
                {
                    // create a list of vlaues and a list of indeces
                    std::map<GenICam::gcstring, GenICam::gcstring>::iterator ptr;
                    for(ptr=m_Symbolics.begin(); ptr!=m_Symbolics.end(); ptr++ )
                    {
                        ValueStrOut += (*ptr).first + "\t";
                        AttributeStrOut += (*ptr).second + "\t";
                    }

                    // get rid of the respective last tabs of the lists
                    if(ValueStrOut.size() > 1)
                    {
                        AttributeStrOut = AttributeStrOut.substr(0, AttributeStrOut.size()-1);
                        ValueStrOut = ValueStrOut.substr(0, ValueStrOut.size()-1);
                    }

                    return true;
                }
            }
            break;
            /**********************************************/
            CONVERT_STRING_ENTRY(Formula_ID, m_Formula)
            CONVERT_STRING_ENTRY(Input_ID, m_InputName)
            CONVERT_ENUM_ENTRY(Representation_ID, m_Representation, ERepresentationClass)
            CONVERT_STRING_ENTRY(Unit_ID, m_Unit)
        END_CONVERT_MAP

    protected:
        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------
        //! minimum value to be stored in the Register
        int64_t  m_Min;

        //! maximum value to be stored in the Register
        int64_t  m_Max;

        //! the formula evaluated by the swiss knife
        GenICam::gcstring m_Formula;

        //! Mapping of the variable's node names to the SYMBOLIC names in the folmulas
        std::map<GenICam::gcstring, GenICam::gcstring> m_Symbolics;

        //! Mapping of SYMBOLIC names to the references of the variables
        std::map<GenICam::gcstring, CIntegerPolyRef> m_Variables;

        //! the parser doing the actual work
        CInt64MathParser m_MathParser;

        //! the representation of this integer node
        ERepresentation m_Representation;

        //! the physical unit name
        GenICam::gcstring m_Unit;

        //! A hardcoded variable name
        /*! This is a helper for the implementation of the converter */
        GenICam::gcstring m_InputName;

    };

    class CIntSwissKnife : public BaseT< ValueT< NodeT< IntegerT< CIntSwissKnifeImpl > > > >
    {
    };

};

#pragma warning ( pop )

#endif // GENAPI_INTSWISSKNIFE_H
