// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core.app.blockchains.digitalgold

import com.google.protobuf.ByteString
import com.trustwallet.core.app.utils.Numeric
import com.trustwallet.core.app.utils.toHexByteArray
import com.trustwallet.core.app.utils.toHexBytes
import com.trustwallet.core.app.utils.toHexBytesInByteString
import junit.framework.Assert.assertEquals
import org.junit.Test
import wallet.core.jni.BitcoinSigner
import wallet.core.jni.proto.Bitcoin

class TestDigitalGoldSigner {

    init {
        System.loadLibrary("TrustWalletCore")
    }

    @Test
    fun DigitalGoldTransactionSigning() {
        // TODO: Finalize implementation

        //val transfer = DigitalGold.TransferMessage.newBuilder()
        //    .setTo("...")
        //    .setAmount(...)
        //    ...
        //    .build()
        //val signingInput = DigitalGold.SigningInput.newBuilder()
        //    ...
        //    .build()

        //val output: DigitalGold.SigningOutput = DigitalGoldSigner.sign(signingInput)

        //assertEquals(
        //    "__EXPECTED_RESULT_DATA__",
        //    Numeric.toHexString(output.encoded.toByteArray())
        //)
    }
}
