//
// file:            packer.hpp
// path:			include/jsonpacker/packer.hpp
// created on:		2022 May 11
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#pragma once

#include <jsonpacker/internal_header.h>
#include <istream>
#include <ostream>

namespace jsonpacker{ namespace packer{


JSONPACKER_EXPORT bool Pack(const char* a_inpFileName, const char* a_outFileName);

template <typename CharIn, typename CharOut>
JSONPACKER_EXPORT bool Pack(::std::basic_istream<CharIn>& a_inp, ::std::basic_ostream<CharOut>& a_out);

template <typename CharIn>
JSONPACKER_EXPORT bool Pack(::std::basic_istream<CharIn>& a_inp, const char* a_outFileName);

template <typename CharOut>
JSONPACKER_EXPORT bool Pack(const char* a_inpFileName, ::std::basic_ostream<CharOut>& a_out);


}}  // namespace jsonpacker{ namespace packer{
