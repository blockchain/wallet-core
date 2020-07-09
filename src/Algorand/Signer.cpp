// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Algorand;

const Data TRANSACTION_TAG = {84, 88};
const std::string TRANSACTION_PAY = "pay";

Data Signer::sign(const PrivateKey &privateKey, Transaction &transaction) noexcept {
    Data data;
    append(data, TRANSACTION_TAG);
    append(data, transaction.serialize());
    auto signature = privateKey.sign(data, TWCurveED25519);
    return Data(signature.begin(), signature.end());
}
