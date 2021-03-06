//=================================================================================================
/*!
//  \file blaze_cuda/util/Unroll.h
//  \brief Header file for arbitrary template unrolling function template
//
//  Copyright (C) 2019 Jules P�nuchot - All Rights Reserved
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

#ifndef _BLAZE_CUDA_UTIL_ALGORITHMS_UNROLL_H_
#define _BLAZE_CUDA_UTIL_ALGORITHMS_UNROLL_H_

#include <cstddef>

#include <utility>

#include <blaze/system/Inline.h>
#include <blaze/system/HostDevice.h>

namespace blaze {

namespace unroll_detail {

   template < typename F, std::size_t... Is >
   BLAZE_ALWAYS_INLINE void BLAZE_DEVICE_CALLABLE unroll_impl( F const& f, std::index_sequence< Is... > const& )
   {
      ( f( std::integral_constant< std::size_t, Is >{} ) , ... );
   }

} // namespace unroll_detail

template< std::size_t N, typename F >
BLAZE_ALWAYS_INLINE void BLAZE_DEVICE_CALLABLE unroll( F const& f )
{
   unroll_detail::unroll_impl( f, std::make_index_sequence< N >{} );
}

}  // namespace blaze

#endif
