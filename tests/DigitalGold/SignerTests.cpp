// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Signer.h"
#include "Bitcoin/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Coin.h"
#include "Bitcoin/Script.h"
#include <TrustWalletCore/TWBitcoinSigHashType.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitcoin;


static const byte p2pkh = p2pkhPrefix(TWCoinTypeDigitalGold);
static const byte p2sh = p2shPrefix(TWCoinTypeDigitalGold);


TEST(DigitalGoldSigner, SignP2PKH) {
    // Test spending from P2PKH address. A single private key is needed.
    
    auto inputTxHash = parse_hex("db6b1b20aa0fd7b23880be2ecbd4a98130974cf4748fb66092ac4d3ceb1a5477"); // transaction id to spend from (example, can be anything)

    auto privateKeyFrom = PrivateKey(parse_hex("63832fb769a6f12b0a403b8906d6b718a3994b1c1ace78be359a7ff39b3a7362")); // private key of the owner
    auto toAddress = Address("GYPHQbxED8GFtpHkZShNzAxyZDrUVKjPFC"); // destination address

    auto publicKeyFrom = privateKeyFrom.getPublicKey(TWPublicKeyTypeSECP256k1); // public key (from private key)
    auto fromAddress = Address(publicKeyFrom, p2pkh); // P2PKH source address (reconstructed from private key)
    EXPECT_EQ(fromAddress.string(), "GNxs8kNGP4E18is7TRvjh9WknrStwMmS2N");
    auto redeemScript = Script::buildForAddress(fromAddress.string(), TWCoinTypeDigitalGold);
    auto redeemScriptHash = redeemScript.hash(); //Hash::ripemd(Hash::sha256(redeemScript.bytes));

    // optional sanity checks
    auto publicKeyHash = Hash::ripemd(Hash::sha256(publicKeyFrom.bytes));
    EXPECT_EQ(hex(publicKeyHash.begin(), publicKeyHash.end()), "3828ccf7696ce8e8c16a397b103f0a4f3b876bf4");
    auto scriptSig = Script::buildPayToPublicKeyHash(publicKeyHash);
    auto scriptSigHash = Hash::ripemd(Hash::sha256(scriptSig.bytes));
    EXPECT_EQ(hex(scriptSigHash.begin(), scriptSigHash.end()), "82472cab708d49bb1e80fae459d6ae0fa34a7bde");
    Data pubKeyHashExtracted;
    redeemScript.matchPayToPublicKeyHash(pubKeyHashExtracted);
    EXPECT_EQ(hex(pubKeyHashExtracted.begin(), pubKeyHashExtracted.end()), hex(publicKeyHash.begin(), publicKeyHash.end()));
    EXPECT_EQ(hex(redeemScriptHash.begin(), redeemScriptHash.end()), hex(scriptSigHash.begin(), scriptSigHash.end()));

    auto changeAddress = Address("GTWUe2J9amAkhGm6ePrg2knPXYwNZWL2AH"); // change address, should belong to the owner

    int inputAmount = 1000; // must cover output amount and the fee, rest will go to the change address
    int outputAmount = 600;

    Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_coin_type(TWCoinTypeDigitalGold);
    input.set_amount(outputAmount); // amount to send
    input.set_byte_fee(1);
    input.set_to_address(toAddress.string());
    input.set_change_address(changeAddress.string());

    input.add_private_key(privateKeyFrom.bytes.data(), privateKeyFrom.bytes.size());
    (*input.mutable_scripts())[hex(redeemScriptHash.begin(), redeemScriptHash.end())] = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());

    Proto::UnspentTransaction* utxo = input.add_utxo();
    utxo->set_amount(inputAmount); // amount to take
    utxo->set_script(redeemScript.bytes.data(), redeemScript.bytes.size());
    utxo->mutable_out_point()->set_hash(inputTxHash.data(), inputTxHash.size()); // transaction id
    utxo->mutable_out_point()->set_index(1); // index of output of the transaction
    utxo->mutable_out_point()->set_sequence(12345678);

    auto plan = Signer::plan(input);
    int feeAmount = plan.fee();
    int changeAmount = plan.change();

    ASSERT_TRUE(feeAmount >= 0);
    ASSERT_TRUE(changeAmount >= 0);

    input.set_allocated_plan(new Proto::TransactionPlan(std::move(plan)));

    ASSERT_TRUE(inputAmount == outputAmount + feeAmount + changeAmount);

    auto signature = Signer::sign(input);

    // TODO: match individual fields
    auto correct_signed_transaction = "transaction {\n"
                                      "  version: 1\n"
                                      "  inputs {\n"
                                      "    previousOutput {\n"
                                      "      hash: \"\\333k\\033 \\252\\017\\327\\2628\\200\\276.\\313\\324\\251\\2010\\227L\\364t\\217\\266`\\222\\254M<\\353\\032Tw\"\n"
                                      "      index: 1\n"
                                      "    }\n"
                                      "    sequence: 12345678\n"
                                      "    script: \"G0D\\002 1@\\300\\177\\204}\\002r\\340\\367?a5\\262y\\202\\303\\'\\03377n\\235#P\\242\\376\\321\\215\\034>\\307\\002 {\\033E\\206\\253\\024\\202z\\375+!j\\264\\020\\345\\t8\\222/\\013\\271\\215\\271\\323\\336\\307(\\225\\237H\\356#\\001!\\002E\\036\\201UM\\260Lf_\\345-F2\\335\\232*\\326W]\\272-\\021)\\331(#\\031\\357\\363\\244LI\"\n"
                                      "  }\n"
                                      "  outputs {\n"
                                      "    value: 600\n"
                                      "    script: \"v\\251\\024\\237\\200-\\234o\\360:\\\"%\\026Q%N\\255\\\\\\363]\\262\\354\\325\\210\\254\"\n"
                                      "  }\n"
                                      "  outputs {\n"
                                      "    value: 174\n"
                                      "    script: \"v\\251\\024j\\003\\326\\\"P\\3748:x\\355\\253>\\246\\244\\3371A%\\207\\212\\210\\254\"\n"
                                      "  }\n"
                                      "}\n"
                                      "encoded: \"\\001\\000\\000\\000\\001\\333k\\033 \\252\\017\\327\\2628\\200\\276.\\313\\324\\251\\2010\\227L\\364t\\217\\266`\\222\\254M<\\353\\032Tw\\001\\000\\000\\000jG0D\\002 1@\\300\\177\\204}\\002r\\340\\367?a5\\262y\\202\\303\\'\\03377n\\235#P\\242\\376\\321\\215\\034>\\307\\002 {\\033E\\206\\253\\024\\202z\\375+!j\\264\\020\\345\\t8\\222/\\013\\271\\215\\271\\323\\336\\307(\\225\\237H\\356#\\001!\\002E\\036\\201UM\\260Lf_\\345-F2\\335\\232*\\326W]\\272-\\021)\\331(#\\031\\357\\363\\244LINa\\274\\000\\002X\\002\\000\\000\\000\\000\\000\\000\\031v\\251\\024\\237\\200-\\234o\\360:\\\"%\\026Q%N\\255\\\\\\363]\\262\\354\\325\\210\\254\\256\\000\\000\\000\\000\\000\\000\\000\\031v\\251\\024j\\003\\326\\\"P\\3748:x\\355\\253>\\246\\244\\3371A%\\207\\212\\210\\254\\000\\000\\000\\000\"\n"
                                      "transaction_id: \"dac0ad7a0ee5b91b3eb20910a87001ea31271e800cc2d6c1e08c87b517462d60\"\n";

    ASSERT_EQ(signature.Utf8DebugString(), correct_signed_transaction);
}



TEST(DigitalGoldSigner, SignP2SH) {
    // Test spending from P2SH address. 2-of-3 multisig script is used as example. 3 public keys are needed to generate the address, 2 private keys to spend from it.
    
    auto inputTxHash = parse_hex("db6b1b20aa0fd7b23880be2ecbd4a98130974cf4748fb66092ac4d3ceb1a5477"); // transaction id to spend from (example, can be anything)

    PrivateKey privateKeyFrom[3] = {
        PrivateKey(parse_hex("63832fb769a6f12b0a403b8906d6b718a3994b1c1ace78be359a7ff39b3a7360")), // private key 1
        PrivateKey(parse_hex("63832fb769a6f12b0a403b8906d6b718a3994b1c1ace78be359a7ff39b3a7361")), // private key 2
        PrivateKey(parse_hex("63832fb769a6f12b0a403b8906d6b718a3994b1c1ace78be359a7ff39b3a7362"))  // private key 3
    };
    auto toAddress = Address("GYPHQbxED8GFtpHkZShNzAxyZDrUVKjPFC"); // destination address

    std::vector<PublicKey> publicKeyFrom;
    for(int i = 0; i < 3; i++) {
        publicKeyFrom.push_back(privateKeyFrom[i].getPublicKey(TWPublicKeyTypeSECP256k1)); // public key (from private key)
    }

    Data redeemScriptSrc; // 2-of-3 multisig redeem script
    redeemScriptSrc.push_back(OP_2);
    for(int i = 0; i < 3; i++) {
        redeemScriptSrc.push_back(publicKeyFrom[i].bytes.size());
        std::copy(publicKeyFrom[i].bytes.begin(), publicKeyFrom[i].bytes.end(), std::back_inserter(redeemScriptSrc));
    }
    redeemScriptSrc.push_back(OP_3);
    redeemScriptSrc.push_back(OP_CHECKMULTISIG);

    auto redeemScript = Script(std::move(redeemScriptSrc));
    auto redeemScriptHash = redeemScript.hash(); //Hash::ripemd(Hash::sha256(redeemScript.bytes));

    Data fromAddressSrc;
    fromAddressSrc.push_back(p2sh);
    std::copy(redeemScriptHash.begin(), redeemScriptHash.end(), std::back_inserter(fromAddressSrc));
    auto fromAddress = Address(fromAddressSrc);
    EXPECT_EQ(fromAddress.string(), "gP2d72von4mf3NUyT2EvpMi8TrFd7cYYUt");

    auto changeAddress = Address("GTWUe2J9amAkhGm6ePrg2knPXYwNZWL2AH"); // change address, should belong to the owner

    int inputAmount = 1000; // must cover output amount and the fee, rest will go to the change address
    int outputAmount = 600;

    Proto::SigningInput input;
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_coin_type(TWCoinTypeDigitalGold);
    input.set_amount(outputAmount); // amount to send
    input.set_byte_fee(1);
    input.set_to_address(toAddress.string());
    input.set_change_address(changeAddress.string());

    for(int i = 0; i < 2; i++) { // only 2 private keys of 3
        input.add_private_key(privateKeyFrom[i].bytes.data(), privateKeyFrom[i].bytes.size());
    }
    (*input.mutable_scripts())[hex(redeemScriptHash.begin(), redeemScriptHash.end())] = std::string(redeemScript.bytes.begin(), redeemScript.bytes.end());

    Proto::UnspentTransaction* utxo = input.add_utxo();
    utxo->set_amount(inputAmount); // amount to take
    utxo->set_script(redeemScript.bytes.data(), redeemScript.bytes.size());
    utxo->mutable_out_point()->set_hash(inputTxHash.data(), inputTxHash.size()); // source transaction id
    utxo->mutable_out_point()->set_index(1); // index of output of the source transaction to spend from
    utxo->mutable_out_point()->set_sequence(12345678);

    auto plan = Signer::plan(input);

    int feeAmount = plan.fee();
    int changeAmount = plan.change();

    ASSERT_TRUE(feeAmount >= 0);
    ASSERT_TRUE(changeAmount >= 0);

    // plan can still be modified

    input.set_allocated_plan(new Proto::TransactionPlan(std::move(plan)));

    ASSERT_TRUE(inputAmount == outputAmount + feeAmount + changeAmount);

    auto signature = Signer::sign(input);

    // TODO: match individual fields
    auto correct_signed_transaction = "transaction {\n"
                                      "  version: 1\n"
                                      "  inputs {\n"
                                      "    previousOutput {\n"
                                      "      hash: \"\\333k\\033 \\252\\017\\327\\2628\\200\\276.\\313\\324\\251\\2010\\227L\\364t\\217\\266`\\222\\254M<\\353\\032Tw\"\n"
                                      "      index: 1\n"
                                      "    }\n"
                                      "    sequence: 12345678\n"
                                      "    script: \"\\000H0E\\002!\\000\\304\\263\\240\\375\\326\\250X:C\\3000\\3055\\206\\t\\003\\246\\335\\030\\324\\006\\242;\\372\\201{\\270<\\325\\335\\2175\\002 W\\275(\\207p\\231\\\"\\315\\007\\201w\\263p\\375e\\375\\010\\r8\\316Iy0\\352cF\\366\\241\\363\\325\\226\\251\\001G0D\\002 2\\343\\372A\\226K@\\3420t\\006\\2703\\016\\227]d\\333\\007\\037\\372W\\275\\017H\\227\\306\\202M\\2151\\273\\002 \\017\\217\\276\\362\\020iG\\\\\\347\\364\\375\\306\\263\\317D\\225}\\360\\007\\001 [J\\300\\022\\304^?\\322\\377\\036\\335\\001\"\n"
                                      "  }\n"
                                      "  outputs {\n"
                                      "    value: 600\n"
                                      "    script: \"v\\251\\024\\237\\200-\\234o\\360:\\\"%\\026Q%N\\255\\\\\\363]\\262\\354\\325\\210\\254\"\n"
                                      "  }\n"
                                      "  outputs {\n"
                                      "    value: 174\n"
                                      "    script: \"v\\251\\024j\\003\\326\\\"P\\3748:x\\355\\253>\\246\\244\\3371A%\\207\\212\\210\\254\"\n"
                                      "  }\n"
                                      "}\n"
                                      "encoded: \"\\001\\000\\000\\000\\001\\333k\\033 \\252\\017\\327\\2628\\200\\276.\\313\\324\\251\\2010\\227L\\364t\\217\\266`\\222\\254M<\\353\\032Tw\\001\\000\\000\\000\\222\\000H0E\\002!\\000\\304\\263\\240\\375\\326\\250X:C\\3000\\3055\\206\\t\\003\\246\\335\\030\\324\\006\\242;\\372\\201{\\270<\\325\\335\\2175\\002 W\\275(\\207p\\231\\\"\\315\\007\\201w\\263p\\375e\\375\\010\\r8\\316Iy0\\352cF\\366\\241\\363\\325\\226\\251\\001G0D\\002 2\\343\\372A\\226K@\\3420t\\006\\2703\\016\\227]d\\333\\007\\037\\372W\\275\\017H\\227\\306\\202M\\2151\\273\\002 \\017\\217\\276\\362\\020iG\\\\\\347\\364\\375\\306\\263\\317D\\225}\\360\\007\\001 [J\\300\\022\\304^?\\322\\377\\036\\335\\001Na\\274\\000\\002X\\002\\000\\000\\000\\000\\000\\000\\031v\\251\\024\\237\\200-\\234o\\360:\\\"%\\026Q%N\\255\\\\\\363]\\262\\354\\325\\210\\254\\256\\000\\000\\000\\000\\000\\000\\000\\031v\\251\\024j\\003\\326\\\"P\\3748:x\\355\\253>\\246\\244\\3371A%\\207\\212\\210\\254\\000\\000\\000\\000\"\n"
                                      "transaction_id: \"5b09ae87620f65d7975e98abb0dedf21cb9e97c0d1ff943283999e9a4b7351fb\"\n";

    ASSERT_EQ(signature.Utf8DebugString(), correct_signed_transaction);
}


