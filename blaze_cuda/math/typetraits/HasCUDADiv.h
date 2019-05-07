//=================================================================================================
/*!
//  \file blaze_cuda/math/typetraits/HasCUDADiv.h
//  \brief Header file for the HasCUDADiv type trait
//
//  Copyright (C) 2012-2019 Klaus Iglberger - All Rights Reserved
//  Copyright (C) 2019 Jules Penuchot - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================

#ifndef _BLAZE_CUDA_MATH_TYPETRAITS_HASCUDADIV_H_
#define _BLAZE_CUDA_MATH_TYPETRAITS_HASCUDADIV_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/system/Vectorization.h>
#include <blaze/util/Complex.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/FalseType.h>
#include <blaze/util/IntegralConstant.h>
#include <blaze/util/typetraits/Decay.h>
#include <blaze/util/typetraits/IsIntegral.h>
#include <blaze/util/typetraits/IsNumeric.h>
#include <blaze/util/typetraits/IsSigned.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Auxiliary helper struct for the HasCUDADiv type trait.
// \ingroup math_type_traits
*/
template< typename T1        // Type of the left-hand side operand
        , typename T2        // Type of the right-hand side operand
        , typename = void >  // Restricting condition
struct HasCUDADivHelper
   : public FalseType
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, typename T2 >
struct HasCUDADivHelper< T1, T2, EnableIf_t< IsNumeric_v<T1> && IsIntegral_v<T1> &&
                                             IsSigned_v<T1> && IsNumeric_v<T2> &&
                                             IsIntegral_v<T2> && IsSigned_v<T2> &&
                                             sizeof(T1) == sizeof(T2) > >
   : public BoolConstant< bool( BLAZE_SVML_MODE ) &&
                          ( ( bool( BLAZE_MIC_MODE      ) && sizeof(T1) >= 4UL ) ||
                            ( bool( BLAZE_AVX512BW_MODE ) && sizeof(T1) <= 2UL ) ||
                            ( bool( BLAZE_AVX512F_MODE  ) && sizeof(T1) >= 4UL ) ) >
{};

template< typename T >
struct HasCUDADivHelper< complex<T>, T, EnableIf_t< IsNumeric_v<T> && IsIntegral_v<T> && IsSigned_v<T> > >
   : public BoolConstant< bool( BLAZE_SVML_MODE ) &&
                          ( ( bool( BLAZE_MIC_MODE      ) && sizeof(T) >= 4UL ) ||
                            ( bool( BLAZE_AVX512BW_MODE ) && sizeof(T) <= 2UL ) ||
                            ( bool( BLAZE_AVX512F_MODE  ) && sizeof(T) >= 4UL ) ) >
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template<>
struct HasCUDADivHelper< float, float >
   : public BoolConstant< bool( BLAZE_SSE_MODE     ) ||
                          bool( BLAZE_AVX_MODE     ) ||
                          bool( BLAZE_MIC_MODE     ) ||
                          bool( BLAZE_AVX512F_MODE ) >
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template<>
struct HasCUDADivHelper< double, double >
   : public BoolConstant< bool( BLAZE_SSE2_MODE    ) ||
                          bool( BLAZE_AVX_MODE     ) ||
                          bool( BLAZE_MIC_MODE     ) ||
                          bool( BLAZE_AVX512F_MODE ) >
{};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Availability of a CUDA division for the given data types.
// \ingroup math_type_traits
//
// Depending on the available instruction set (SSE, SSE2, SSE3, SSE4, AVX, AVX2, MIC, ...) and
// the used compiler, this type trait provides the information whether a CUDA division operation
// exists for the two given data types \a T1 and \a T2 (ignoring the cv-qualifiers). In case the
// CUDA division is available, the \a value member constant is set to \a true, the nested type
// definition \a Type is \a TrueType, and the class derives from \a TrueType. Otherwise \a value
// is set to \a false, \a Type is \a FalseType, and the class derives from \a FalseType. The
// following example assumes that AVX is available:

   \code
   blaze::HasCUDADiv< float, float >::value    // Evaluates to 1
   blaze::HasCUDADiv< double, double >::Type   // Results in TrueType
   blaze::HasCUDADiv< complex<float>, float >  // Is derived from TrueType
   blaze::HasCUDADiv< int, int >::value        // Evaluates to 0
   blaze::HasCUDADiv< float, int >::Type       // Results in FalseType
   blaze::HasCUDADiv< double, float >          // Is derived from FalseType
   \endcode
*/
template< typename T1        // Type of the left-hand side operand
        , typename T2        // Type of the right-hand side operand
        , typename = void >  // Restricting condition
struct HasCUDADiv
   : public BoolConstant< HasCUDADivHelper< Decay_t<T1>, Decay_t<T2> >::value >
{};
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Auxiliary variable template for the HasCUDADiv type trait.
// \ingroup type_traits
//
// The HasCUDADiv_v variable template provides a convenient shortcut to access the nested
// \a value of the HasCUDADiv class template. For instance, given the types \a T1 and \a T2
// the following two statements are identical:

   \code
   constexpr bool value1 = blaze::HasCUDADiv<T1,T2>::value;
   constexpr bool value2 = blaze::HasCUDADiv_v<T1,T2>;
   \endcode
*/
template< typename T1    // Type of the left-hand side operand
        , typename T2 >  // Type of the right-hand side operand
constexpr bool HasCUDADiv_v = HasCUDADiv<T1,T2>::value;
//*************************************************************************************************

} // namespace blaze

#endif
