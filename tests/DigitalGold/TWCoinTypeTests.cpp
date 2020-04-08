// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here MAY BE LOST.
// Generated one-time (codegen/bin/cointests)
//

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWCoinTypeConfiguration.h>
#include <gtest/gtest.h>


TEST(TWDigitalGoldCoinType, TWCoinType) {
    auto symbol = WRAPS(TWCoinTypeConfigurationGetSymbol(TWCoinTypeDigitalGold));
    auto txId = TWStringCreateWithUTF8Bytes("68e203d1837aad907f4c09b22835e78304a7ae5c4268d0c2487958e3a1858b6e");
    auto txUrl = WRAPS(TWCoinTypeConfigurationGetTransactionURL(TWCoinTypeDigitalGold, txId));
    auto accId = TWStringCreateWithUTF8Bytes("g3X4Jwf9WLuBqET7tr2bCkDX3A2FGBrUm8");
    auto accUrl = WRAPS(TWCoinTypeConfigurationGetAccountURL(TWCoinTypeDigitalGold, accId));
    auto id = WRAPS(TWCoinTypeConfigurationGetID(TWCoinTypeDigitalGold));
    auto name = WRAPS(TWCoinTypeConfigurationGetName(TWCoinTypeDigitalGold));

    ASSERT_EQ(TWCoinTypeConfigurationGetDecimals(TWCoinTypeDigitalGold), 8);
    ASSERT_EQ(TWBlockchainBitcoin, TWCoinTypeBlockchain(TWCoinTypeDigitalGold));
    ASSERT_EQ(0x61, TWCoinTypeP2shPrefix(TWCoinTypeDigitalGold));
    ASSERT_EQ(0x0, TWCoinTypeStaticPrefix(TWCoinTypeDigitalGold));
    assertStringsEqual(symbol, "DGLD");
    assertStringsEqual(txUrl, "https://explorer.dgld.ch/tx/68e203d1837aad907f4c09b22835e78304a7ae5c4268d0c2487958e3a1858b6e");
    assertStringsEqual(accUrl, "https://explorer.dgld.ch/address/g3X4Jwf9WLuBqET7tr2bCkDX3A2FGBrUm8");
    assertStringsEqual(id, "dgld");
    assertStringsEqual(name, "DigitalGold");
}
