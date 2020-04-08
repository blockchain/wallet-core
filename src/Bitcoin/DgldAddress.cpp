// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

using namespace TW::Bitcoin;

bool DlgdAddress::isValid(const std::string& string) {
    // TODO: Finalize implementation
    return false;
}

DlgdAddress::DlgdAddress(const std::string& string) {
    // TODO: Finalize implementation

    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
}

DlgdAddress::DlgdAddress(const PublicKey& publicKey) {
    // TODO: Finalize implementation
}

std::string DlgdAddress::string() const {
    // TODO: Finalize implementation
    return "TODO";
}
