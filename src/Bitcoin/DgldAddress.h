// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include "Address.h"
#include "SegwitAddress.h"

#include <string>

namespace TW::Bitcoin {

class DgldAddress : public Address {
  public:

    static const byte ADDRTYPE_P2PKH = 38;
    static const byte ADDRTYPE_P2SH = 97;
    
    explicit DgldAddress(const std::string& string) : Address(string) {}
    /// Initializes a  address with a collection of bytes.
    explicit DgldAddress(const Data& data) : Address(data) {}

    /// Initializes a  address with a public key and a prefix.
    DgldAddress(const PublicKey& publicKey, byte prefix) : Address(publicKey, prefix) {}

    DgldAddress(const PublicKey& publicKey) : Address(publicKey, ADDRTYPE_P2PKH) {}

    static bool isValid(const std::string& string) {
        return Address::isValid(string, {TW::data(ADDRTYPE_P2PKH), TW::data(ADDRTYPE_P2SH)});
    }
};


inline bool operator==(const DgldAddress& lhs, const DgldAddress& rhs) {
    return lhs.string() == rhs.string();
}


} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWDigitalGoldAddress {
    TW::Bitcoin::DgldAddress impl;
};

