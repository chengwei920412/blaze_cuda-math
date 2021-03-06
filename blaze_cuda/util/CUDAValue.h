//=================================================================================================
/*!
//  \file blaze_cuda/util/algorithms/CUDAReduce.h
//  \brief Header file for CUDAReduce's implementation
//
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

#ifndef _BLAZE_CUDA_UTIL_CUDAVALUE_H_
#define _BLAZE_CUDA_UTIL_CUDAVALUE_H_

#include <utility>

#include <cuda_runtime.h>

#include <blaze/system/HostDevice.h>

namespace blaze {

template< typename T >
class CUDAManagedValue
{
   T* _ptr;

public:
   inline BLAZE_DEVICE_CALLABLE CUDAManagedValue()
   {
      cudaMallocManaged( ( void** )&_ptr, sizeof( T ) );
      *_ptr = T();
   }

   inline BLAZE_DEVICE_CALLABLE CUDAManagedValue( CUDAManagedValue && o )      : _ptr( o._ptr )
      { o._ptr = nullptr; }
   inline BLAZE_DEVICE_CALLABLE CUDAManagedValue( CUDAManagedValue const& o )  : CUDAManagedValue()
      { *_ptr = *o._ptr; }

   inline BLAZE_DEVICE_CALLABLE CUDAManagedValue( T const& v )   : CUDAManagedValue()
      { *_ptr = v; }
   inline BLAZE_DEVICE_CALLABLE CUDAManagedValue( T &&     v )   : CUDAManagedValue()
      { *_ptr = std::move( v ); }
   inline BLAZE_DEVICE_CALLABLE CUDAManagedValue( T *      ptr ) : _ptr( ptr ) {}


   inline BLAZE_DEVICE_CALLABLE T& operator*() { return *_ptr; }
   inline BLAZE_DEVICE_CALLABLE T* ptr()       { return  _ptr; }

   inline BLAZE_DEVICE_CALLABLE ~CUDAManagedValue() { if( _ptr != nullptr ) cudaFree( _ptr ); }
};

}  // namespace blaze

#endif
