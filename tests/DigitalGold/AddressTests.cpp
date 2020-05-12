// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Bitcoin/Address.h"
#include "Bitcoin/Script.h"
#include "Coin.h"
#include "PublicKey.h"
#include "PrivateKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;
using namespace TW::Bitcoin;

static const byte p2pkh = p2pkhPrefix(TWCoinTypeDigitalGold);
static const byte p2sh = p2shPrefix(TWCoinTypeDigitalGold);

TEST(DigitalGoldAddress, Valid) {

    ASSERT_TRUE(Address::isValid("GJCMxPGMH3LVoGtZ3yEhPYbMVnYwSybBzi", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GaLFdPjbDVGcv7v6xuhbvpXUq8mtMkR24y", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GfxjHLysUq5XmPqtYPVyzdVm39QSDpFdET", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("Gdzoxj6QFLDndw5htN2hLJ8JwKRWYHdX2K", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GJKt1CQ4wj2i7QTL2VzhXGQ7u7uUXJ7s5A", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("Gc5JdxYv2k1FfJMr689J66VxQ4yGp97GzZ", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GdfXJwBdQUwD1nuySrQeZjFw8KEtPEoBeB", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GcSsKzmisNB5KCH6wJr6NZyoSMyydVvi22", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GKm9Uj8R6MPqNw2xkhXNYKsvUy46n8E655", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GJu16E3CmFdSknrzbKpXacW2MPoLwiesTv", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GTWUe2J9amAkhGm6ePrg2knPXYwNZWL2AH", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GJBJoGst6SPYweTdE2rLXpc7ky3JaN8s3M", {{p2pkh}, {p2sh}}));
    ASSERT_TRUE(Address::isValid("GYPHQbxED8GFtpHkZShNzAxyZDrUVKjPFC", {{p2pkh}, {p2sh}}));

    // TODO: test segwit addresses
}

TEST(DigitalGoldAddress, Invalid) {

    ASSERT_FALSE(Address::isValid("GZ8fKGGqBZtenPR8hL-----BpMcagkXg5g", {{p2pkh}, {p2sh}}));
    ASSERT_FALSE(Address::isValid("1MVk8kVbs7nepTGwbQN1H46UDjSDAUgcRU", {{p2pkh}, {p2sh}})); // valid bitcoin address, but invalid dgld address

    // TODO: Add more tests
}

TEST(DigitalGoldAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("63832fb769a6f12b0a403b8906d6b718a3994b1c1ace78be359a7ff39b3a7362"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1), p2pkh);
    ASSERT_EQ(address.string(), "GNxs8kNGP4E18is7TRvjh9WknrStwMmS2N");
}

TEST(DigitalGoldAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("028bd49b59eaa0aa4485ef35bc0433cf5071eeb41b229d17c5563d935ca474a9b4"), TWPublicKeyTypeSECP256k1);
    auto address = Address(publicKey, p2pkh);
    ASSERT_EQ(address.string(), "GSvAp2iiodmfkuS4xcpAyJo7s5xhPGw6YY");
}

TEST(DigitalGoldAddress, FromString) {
    auto address = Address("GRZ3JdwD4LuiJd2x7kKfFVJr6MdHREj7ie");
    ASSERT_EQ(address.string(), "GRZ3JdwD4LuiJd2x7kKfFVJr6MdHREj7ie");
}


TEST(DigitalGoldAddress, ScriptTest) {

    auto privateKey1 = PrivateKey(parse_hex("63832fb769a6f12b0a403b8906d6b718a3994b1c1ace78be359a7ff39b3a7362"));

    auto address1a = Address(privateKey1.getPublicKey(TWPublicKeyTypeSECP256k1), p2pkh);
    auto script1a = Script::buildForAddress(address1a.string(), TWCoinTypeDigitalGold);
    ASSERT_FALSE(script1a.isPayToWitnessScriptHash());
    ASSERT_FALSE(script1a.isPayToScriptHash());
    ASSERT_EQ(script1a.bytes.size(), 25);
    ASSERT_EQ(hex(script1a.bytes.begin(), script1a.bytes.end()), "76a9143828ccf7696ce8e8c16a397b103f0a4f3b876bf488ac");
    auto hash1a = Data();
    script1a.matchPayToPubkeyHash(hash1a);
    ASSERT_EQ(hash1a.size(), 20);
    ASSERT_EQ(hex(hash1a.begin(), hash1a.end()), "3828ccf7696ce8e8c16a397b103f0a4f3b876bf4");

    auto address1b = Address(privateKey1.getPublicKey(TWPublicKeyTypeSECP256k1), p2sh);
    auto script1b = Script::buildForAddress(address1b.string(), TWCoinTypeDigitalGold);
    ASSERT_FALSE(script1b.isPayToWitnessScriptHash());
    ASSERT_TRUE(script1b.isPayToScriptHash());
    ASSERT_EQ(script1b.bytes.size(), 23);
    ASSERT_EQ(hex(script1b.bytes.begin(), script1b.bytes.end()), "a9143828ccf7696ce8e8c16a397b103f0a4f3b876bf487");
    auto hash1b = Data();
    script1b.matchPayToScriptHash(hash1b);
    ASSERT_EQ(hash1b.size(), 20);
    ASSERT_EQ(hex(hash1b.begin(), hash1b.end()), "3828ccf7696ce8e8c16a397b103f0a4f3b876bf4");

    auto privateKey2 = PrivateKey(parse_hex("63832fb769a6f12b0a403b8906d6b718a3994b1c1ace78be359a7ff39b3a7363"));

    auto address2a = Address(privateKey2.getPublicKey(TWPublicKeyTypeSECP256k1), p2pkh);
    auto script2a = Script::buildForAddress(address2a.string(), TWCoinTypeDigitalGold);
    ASSERT_FALSE(script2a.isPayToWitnessScriptHash());
    ASSERT_FALSE(script2a.isPayToScriptHash());
    ASSERT_EQ(script2a.bytes.size(), 25);
    ASSERT_EQ(hex(script2a.bytes.begin(), script2a.bytes.end()), "76a9142f8a97924776e36fafaae12e41cf2b0f7d24597c88ac");
    auto hash2a = Data();
    script2a.matchPayToPubkeyHash(hash2a);
    ASSERT_EQ(hash2a.size(), 20);
    ASSERT_EQ(hex(hash2a.begin(), hash2a.end()), "2f8a97924776e36fafaae12e41cf2b0f7d24597c");

    auto address2b = Address(privateKey2.getPublicKey(TWPublicKeyTypeSECP256k1), p2sh);
    auto script2b = Script::buildForAddress(address2b.string(), TWCoinTypeDigitalGold);
    ASSERT_FALSE(script2b.isPayToWitnessScriptHash());
    ASSERT_TRUE(script2b.isPayToScriptHash());
    ASSERT_EQ(script2b.bytes.size(), 23);
    ASSERT_EQ(hex(script2b.bytes.begin(), script2b.bytes.end()), "a9142f8a97924776e36fafaae12e41cf2b0f7d24597c87");
    auto hash2b = Data();
    script2b.matchPayToScriptHash(hash2b);
    ASSERT_EQ(hash2b.size(), 20);
    ASSERT_EQ(hex(hash2b.begin(), hash2b.end()), "2f8a97924776e36fafaae12e41cf2b0f7d24597c");

}


