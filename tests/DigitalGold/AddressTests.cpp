// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Bitcoin/DgldAddress.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Bitcoin;

TEST(DigitalGoldAddress, Valid) {
    ASSERT_TRUE(DgldAddress::isValid("GJCMxPGMH3LVoGtZ3yEhPYbMVnYwSybBzi"));
    ASSERT_TRUE(DgldAddress::isValid("GaLFdPjbDVGcv7v6xuhbvpXUq8mtMkR24y"));
    ASSERT_TRUE(DgldAddress::isValid("GfxjHLysUq5XmPqtYPVyzdVm39QSDpFdET"));
    ASSERT_TRUE(DgldAddress::isValid("Gdzoxj6QFLDndw5htN2hLJ8JwKRWYHdX2K"));
    ASSERT_TRUE(DgldAddress::isValid("GJKt1CQ4wj2i7QTL2VzhXGQ7u7uUXJ7s5A"));
    ASSERT_TRUE(DgldAddress::isValid("Gc5JdxYv2k1FfJMr689J66VxQ4yGp97GzZ"));
    ASSERT_TRUE(DgldAddress::isValid("GdfXJwBdQUwD1nuySrQeZjFw8KEtPEoBeB"));
    ASSERT_TRUE(DgldAddress::isValid("GcSsKzmisNB5KCH6wJr6NZyoSMyydVvi22"));
    ASSERT_TRUE(DgldAddress::isValid("GKm9Uj8R6MPqNw2xkhXNYKsvUy46n8E655"));
    ASSERT_TRUE(DgldAddress::isValid("GJu16E3CmFdSknrzbKpXacW2MPoLwiesTv"));
    ASSERT_TRUE(DgldAddress::isValid("GTWUe2J9amAkhGm6ePrg2knPXYwNZWL2AH"));
    ASSERT_TRUE(DgldAddress::isValid("GJBJoGst6SPYweTdE2rLXpc7ky3JaN8s3M"));
    ASSERT_TRUE(DgldAddress::isValid("GYPHQbxED8GFtpHkZShNzAxyZDrUVKjPFC"));

    // TODO: test segwit addresses
}

TEST(DigitalGoldAddress, Invalid) {
    ASSERT_FALSE(DgldAddress::isValid("GZ8fKGGqBZtenPR8hL-----BpMcagkXg5g"));
    ASSERT_FALSE(DgldAddress::isValid("1MVk8kVbs7nepTGwbQN1H46UDjSDAUgcRU")); // valid bitcoin address, but invalid dgld address

    // TODO: Add more tests
}

TEST(DigitalGoldAddress, FromPrivateKey) {
    // TODO: Check public key type, finalize implementation
    auto privateKey = PrivateKey(parse_hex("63832fb769a6f12b0a403b8906d6b718a3994b1c1ace78be359a7ff39b3a7362"));
    auto address = DgldAddress(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    ASSERT_EQ(address.string(), "GNxs8kNGP4E18is7TRvjh9WknrStwMmS2N");
}

TEST(DigitalGoldAddress, FromPublicKey) {
    // TODO: Check public key type, finalize implementation
    auto publicKey = PublicKey(parse_hex("028bd49b59eaa0aa4485ef35bc0433cf5071eeb41b229d17c5563d935ca474a9b4"), TWPublicKeyTypeSECP256k1);
    auto address = DgldAddress(publicKey);
    ASSERT_EQ(address.string(), "GSvAp2iiodmfkuS4xcpAyJo7s5xhPGw6YY");
}

TEST(DigitalGoldAddress, FromString) {
    auto address = DgldAddress("GRZ3JdwD4LuiJd2x7kKfFVJr6MdHREj7ie");
    ASSERT_EQ(address.string(), "GRZ3JdwD4LuiJd2x7kKfFVJr6MdHREj7ie");
}
