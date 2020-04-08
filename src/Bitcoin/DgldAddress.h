// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../PublicKey.h"

#include <string>

namespace TW::Bitcoin {

class DlgdAddress {
  public:
    // TODO: Complete class definition

    /// Determines whether a string makes a valid address.
    static bool isValid(const std::string& string);

    /// Initializes a DigitalGold address with a string representation.
    explicit DlgdAddress(const std::string& string);

    /// Initializes a DigitalGold address with a public key.
    explicit DlgdAddress(const PublicKey& publicKey);

    /// Returns a string representation of the address.
    std::string string() const;
};

inline bool operator==(const DlgdAddress& lhs, const DlgdAddress& rhs) {
    // TODO: Complete equality operator
    return true;
}

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWDigitalGoldAddress {
    TW::Bitcoin::DlgdAddress impl;
};
