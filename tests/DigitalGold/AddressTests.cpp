// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Bitcoin/Address.h"
#include "Coin.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Bitcoin;

TEST(DigitalGoldAddress, Valid) {
    ASSERT_TRUE(Address::isValid("GJCMxPGMH3LVoGtZ3yEhPYbMVnYwSybBzi"));
    ASSERT_TRUE(Address::isValid("GaLFdPjbDVGcv7v6xuhbvpXUq8mtMkR24y"));
    ASSERT_TRUE(Address::isValid("GfxjHLysUq5XmPqtYPVyzdVm39QSDpFdET"));
    ASSERT_TRUE(Address::isValid("Gdzoxj6QFLDndw5htN2hLJ8JwKRWYHdX2K"));
    ASSERT_TRUE(Address::isValid("GJKt1CQ4wj2i7QTL2VzhXGQ7u7uUXJ7s5A"));
    ASSERT_TRUE(Address::isValid("Gc5JdxYv2k1FfJMr689J66VxQ4yGp97GzZ"));
    ASSERT_TRUE(Address::isValid("GdfXJwBdQUwD1nuySrQeZjFw8KEtPEoBeB"));
    ASSERT_TRUE(Address::isValid("GcSsKzmisNB5KCH6wJr6NZyoSMyydVvi22"));
    ASSERT_TRUE(Address::isValid("GKm9Uj8R6MPqNw2xkhXNYKsvUy46n8E655"));
    ASSERT_TRUE(Address::isValid("GJu16E3CmFdSknrzbKpXacW2MPoLwiesTv"));
    ASSERT_TRUE(Address::isValid("GTWUe2J9amAkhGm6ePrg2knPXYwNZWL2AH"));
    ASSERT_TRUE(Address::isValid("GJBJoGst6SPYweTdE2rLXpc7ky3JaN8s3M"));
    ASSERT_TRUE(Address::isValid("GYPHQbxED8GFtpHkZShNzAxyZDrUVKjPFC"));

    // TODO: test segwit addresses
}

TEST(DigitalGoldAddress, Invalid) {

    const byte p2pkh = p2pkhPrefix(TWCoinTypeDigitalGold);
    const byte p2sh = p2shPrefix(TWCoinTypeDigitalGold);

    ASSERT_FALSE(Address::isValid("GZ8fKGGqBZtenPR8hL-----BpMcagkXg5g"));
    ASSERT_FALSE(Address::isValid("1MVk8kVbs7nepTGwbQN1H46UDjSDAUgcRU", {{p2pkh}, {p2sh}})); // valid bitcoin address, but invalid dgld address

    // TODO: Add more tests
}

TEST(DigitalGoldAddress, FromPrivateKey) {

    const byte p2pkh = p2pkhPrefix(TWCoinTypeDigitalGold);
    const byte p2sh = p2shPrefix(TWCoinTypeDigitalGold);

    auto privateKey = PrivateKey(parse_hex("63832fb769a6f12b0a403b8906d6b718a3994b1c1ace78be359a7ff39b3a7362"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1), p2pkh);
    ASSERT_EQ(address.string(), "GNxs8kNGP4E18is7TRvjh9WknrStwMmS2N");
}

TEST(DigitalGoldAddress, FromPublicKey) {

    const byte p2pkh = p2pkhPrefix(TWCoinTypeDigitalGold);
    const byte p2sh = p2shPrefix(TWCoinTypeDigitalGold);

    auto publicKey = PublicKey(parse_hex("028bd49b59eaa0aa4485ef35bc0433cf5071eeb41b229d17c5563d935ca474a9b4"), TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey, p2pkh);
    ASSERT_EQ(address.string(), "GSvAp2iiodmfkuS4xcpAyJo7s5xhPGw6YY");
}

TEST(DigitalGoldAddress, FromString) {
    auto address = Address("GRZ3JdwD4LuiJd2x7kKfFVJr6MdHREj7ie");
    ASSERT_EQ(address.string(), "GRZ3JdwD4LuiJd2x7kKfFVJr6MdHREj7ie");
}
