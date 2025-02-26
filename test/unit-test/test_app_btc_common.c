// Copyright 2019 Shift Cryptosecurity AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>

#include <apps/btc/btc_common.h>
#include <keystore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <util.h>

typedef struct {
    uint32_t threshold;
    size_t xpubs_count;
    const char* xpubs[15];
    uint32_t keypath_change;
    uint32_t keypath_address;
    const char* expected_script_hex;
} test_case_redeemscript_multisig_p2wsh;

static void _test_btc_common_pkscript_from_multisig(void** state)
{
    // clang-format off
    const test_case_redeemscript_multisig_p2wsh tests[] = {
        {
            .threshold = 1,
            .xpubs_count = 2,
            .xpubs =
                {
                    "xpub6FEZ9Bv73h1vnE4TJG4QFj2RPXJhhsPbnXgFyH3ErLvpcZrDcynY65bhWga8PazWHLSLi23PoBhGcLcYW6JRiJ12zXZ9Aop4LbAqsS3gtcy",
                    "xpub6EGAio99SxruuNxoBtG4fbYx3xM8fs7wjYJLRNcUg7UQin3LTANQiUYyb3RLjZ2EAyLsQBrtbNENUGh3oWzjHtgfQ3mtjPNFgNMronzTTVR",
                },
            .keypath_change = 0,
            .keypath_address = 1,
            .expected_script_hex =
                "51210217fb1e3415108fee2b004c932dc5a89eabf3587e3e7b21165c123de1f37a3a612102ae082612"
                "4c98c4e255c1a6cc404ff6d2448a0d9f853e6d72d6b02d9ad2d3565052ae",
        },
        { // different xpub order should have the same result.
            .threshold = 1,
            .xpubs_count = 2,
            .xpubs =
                {
                    "xpub6EGAio99SxruuNxoBtG4fbYx3xM8fs7wjYJLRNcUg7UQin3LTANQiUYyb3RLjZ2EAyLsQBrtbNENUGh3oWzjHtgfQ3mtjPNFgNMronzTTVR",
                    "xpub6FEZ9Bv73h1vnE4TJG4QFj2RPXJhhsPbnXgFyH3ErLvpcZrDcynY65bhWga8PazWHLSLi23PoBhGcLcYW6JRiJ12zXZ9Aop4LbAqsS3gtcy",
                },
            .keypath_change = 0,
            .keypath_address = 1,
            .expected_script_hex =
                "51210217fb1e3415108fee2b004c932dc5a89eabf3587e3e7b21165c123de1f37a3a612102ae082612"
                "4c98c4e255c1a6cc404ff6d2448a0d9f853e6d72d6b02d9ad2d3565052ae",
        },
        {
            .threshold = 1,
            .xpubs_count = 2,
            .xpubs =
                {
                    "xpub6FEZ9Bv73h1vnE4TJG4QFj2RPXJhhsPbnXgFyH3ErLvpcZrDcynY65bhWga8PazWHLSLi23PoBhGcLcYW6JRiJ12zXZ9Aop4LbAqsS3gtcy",
                    "xpub6EGAio99SxruuNxoBtG4fbYx3xM8fs7wjYJLRNcUg7UQin3LTANQiUYyb3RLjZ2EAyLsQBrtbNENUGh3oWzjHtgfQ3mtjPNFgNMronzTTVR",
                },
            .keypath_change = 1,
            .keypath_address = 10,
            .expected_script_hex =
                "512102b6da3d9e33c3bcee679ef3bb2fca8e60c4a8ade06519146c77b007778756b2c92103f42b45d0d"
                "91039df309ff5d10d0a044fb4eb6595d015281be2d56c288524d68f52ae"
        },
        {
            .threshold = 2,
            .xpubs_count = 2,
            .xpubs =
                {
                    "xpub6FEZ9Bv73h1vnE4TJG4QFj2RPXJhhsPbnXgFyH3ErLvpcZrDcynY65bhWga8PazWHLSLi23PoBhGcLcYW6JRiJ12zXZ9Aop4LbAqsS3gtcy",
                    "xpub6EGAio99SxruuNxoBtG4fbYx3xM8fs7wjYJLRNcUg7UQin3LTANQiUYyb3RLjZ2EAyLsQBrtbNENUGh3oWzjHtgfQ3mtjPNFgNMronzTTVR",
                },
            .keypath_change = 0,
            .keypath_address = 1,
            .expected_script_hex =
                "52210217fb1e3415108fee2b004c932dc5a89eabf3587e3e7b21165c123de1f37a3a612102ae082612"
                "4c98c4e255c1a6cc404ff6d2448a0d9f853e6d72d6b02d9ad2d3565052ae",
        },
        {
            .threshold = 15,
            .xpubs_count = 15,
            .xpubs =
                {
                    "xpub6FEZ9Bv73h1vnE4TJG4QFj2RPXJhhsPbnXgFyH3ErLvpcZrDcynY65bhWga8PazWHLSLi23PoBhGcLcYW6JRiJ12zXZ9Aop4LbAqsS3gtcy",
                    "xpub6EGAio99SxruuNxoBtG4fbYx3xM8fs7wjYJLRNcUg7UQin3LTANQiUYyb3RLjZ2EAyLsQBrtbNENUGh3oWzjHtgfQ3mtjPNFgNMronzTTVR",
                    "xpub6E9Qk6G1PAZPqheZ85sySQc9fxS8mp2muF9dNaXpnCGvW2NB13rCm4TKLo9vJaCyxcXBJPF2yBSkKuivLGA5fxuXhbRSL2Sp8HfgxEMFYD3",
                    "xpub6DxHJ5evyWcSBrG9zCauY1zrh3J6HkiBGLzgG4wvuRaDQYxF6suuPNh1hD2VktphRhEwWXECaWLXo1PkVkGn7hW6vq6AN3ZgqFUrQ7boHqs",
                    "xpub6EdCXJqHFRVqVCZpain6TMzkpmcU6pLU5jSzjUUouumdkzKUAmvBiTsVeJSwxdBzH5mLU1FEFka7jsrs1JeRbqJnwHE31bVF26gkJQ5SCs3",
                    "xpub6EG6LDy2hGg7NBUKyPzqe8k57Jm6H9WmH85MKKWGVTCbr5tVDt8oaKSAArXga4LbYy6Aawfzr324kXq4ia4vSkRBzPCktDv5XJbPRg3sXsz",
                    "xpub6EVoeT8mm6jfq5mtG3Kuv2ozffH1oRaLYsq88N1x7225QBzfBeZxbdx6sGYpFpkcJohzLHXhM7GjqqyrvxzkfvZjydSCGPbaDxWirKH3TRp",
                    "xpub6FNtqhDqFmHJxZsocfd2LftXzZAcDXK2ijhzcscsrsu46Ccz3uv7rrZYbFEvA98svjzkD49x8K2Mi2BuJuhyZHfTtBfdgeUc66JdCez8KG8",
                    "xpub6F1behqQRigaf5gbFbbdjrNV4M64UTQTrzEU535dURgBMJakSFpiZkXveqEscL6Y6gyveFwxp8PGKn3q9MLtwk1UmyRRkFCQb2X6hfvGYWt",
                    "xpub6FG3mVwCkRmtmFuCKZa6MXc4kCPEd5bKrjrNAPgwcmekysnsHBaadhuzo2jV2AjYyg4QjGmu3LgyEUAw4bUXPUsQJG61ZtKM7MVkBxbxcVj",
                    "xpub6DoEJEeUNBkLF7zmKKu8YewqK1PcXWfuek2J2Y8USdGh2McQStsGbVn2oqv521KdJiESeRW4mBBtpBamKHNaD6yZhAbyPwy51VyqHS4EFq6",
                    "xpub6EzUjWSuWk7kBKZTKsdXkFMUropKFLq1iWabRtQpXckxf6s9NMR8UrmY6aYQUuvHyXpYo78RJhyZ1sK9Re4ZbdzpG4Awm6yW221N2AQM6ZU",
                    "xpub6DeznbrZVRaZ4P5Xr79JBs8dNyBMamFmAgAX52o73Pap5RLkMmUi9oQH1sopigbSr6gwUoDMd3EhpoB5tBZXzu4HWJiGETKQGneYtRpjaJB",
                    "xpub6EYf1KXzjaTgcNZFq7pVXGtGDkqHFPvEGBDygkDodz94ZpDazWppGe57hDhTA94z6zeGEubqyLqUMP67ubdd8hf6BbKYA9qtdDf3yM5wdJX",
                    "xpub6ELR9CAGqxwbKcCh591AfKs74neEY9UjtNbvLjrpsxH2FakqE238J1DmsFHePtXXyYhkZshW3qTWWwhENTQgWb6KHaf7SQnVovsKxtvZQaG",
                },
            .keypath_change = 0,
            .keypath_address = 1,
            .expected_script_hex =
            "5f210210e4a9e6d84a7d4b88d5f0450ade30de2046f824374f9b4954a6f03bd37b7269210217fb1e341"
            "5108fee2b004c932dc5a89eabf3587e3e7b21165c123de1f37a3a61210219ad58aa89a3e1669b0757b7"
            "b87d72350cd94675421365a9b7ae781fabeb04ec210230a8551d874b4a3633195c1ba80d0fd5d4e6cf7"
            "917b07f00379893490f795fbe210242f82d15933cf3487567405699910eae5c4b5b24821eeaceeac0ea"
            "da231a760421024be1e5f4fd6c4248b05df752d19754aad4ca663f62f20fd7ac54616899870ebc21024"
            "d5cae14247c53ec7943a78ddb016a939e98756526587ec4bb72789334e698292102ae0826124c98c4e2"
            "55c1a6cc404ff6d2448a0d9f853e6d72d6b02d9ad2d356502102cd014c5921c2f40c0b8de3cf32f9b67"
            "89737e2a06677c4da7325623bcb0af89421033f63c02d09195b9c7efb7b75e18da8b768b5c3e0517082"
            "98d6580634284c28122103410a5da3477482eea7be703bd81d00d4498b7babfbd25f7c930a137a5025c"
            "0b721035b0322eeec4356d59edf4b6213cf78409c6f2e05c26e65b04c503f98a38ec78b21037ff295f8"
            "45fabf9eb4ada869bfa62bde1ede38f074b12bf12a2a2f214282cef82103aef77f1780440ba2445aef6"
            "d3ecf5d0b8dae3b6f22abc44734e1d4c257dc631f2103cd01c7cd59d6956bf07f1e7acba7c41a126ba5"
            "49c07d0c88988c94846ecd88005fae",
        },
    };
    // clang-format on

    for (size_t test_case_index = 0;
         test_case_index < sizeof(tests) / sizeof(test_case_redeemscript_multisig_p2wsh);
         test_case_index++) {
        const test_case_redeemscript_multisig_p2wsh* test_case = &tests[test_case_index];
        multisig_t multisig = {
            .threshold = test_case->threshold,
            .xpubs_count = test_case->xpubs_count,
        };
        for (size_t xpub_idx = 0; xpub_idx < test_case->xpubs_count; xpub_idx++) {
            size_t written;
            uint8_t deser[BIP32_SERIALIZED_LEN + BASE58_CHECKSUM_LEN];
            assert_int_equal(
                wally_base58_to_bytes(
                    test_case->xpubs[xpub_idx],
                    BASE58_FLAG_CHECKSUM,
                    deser,
                    sizeof(deser),
                    &written),
                WALLY_OK);
            memcpy(multisig.xpubs[xpub_idx], deser, sizeof(multisig.xpubs[xpub_idx]));
        }

        uint8_t script[520];
        size_t script_size = sizeof(script);
        assert_true(btc_common_pkscript_from_multisig(
            &multisig,
            test_case->keypath_change,
            test_case->keypath_address,
            script,
            &script_size));
        assert_true(script_size <= sizeof(script));

        char script_hex[2 * script_size + 1];
        util_uint8_to_hex(script, script_size, script_hex);
        assert_string_equal(script_hex, test_case->expected_script_hex);
    }
}

static void _test_btc_common_pkscript_from_multisig_unhappy(void** state)
{
    multisig_t multisig = {
        .threshold = 1,
        .xpubs_count = 2,
    };
    uint8_t deser[BIP32_SERIALIZED_LEN + BASE58_CHECKSUM_LEN];
    size_t written;
    assert_int_equal(
        wally_base58_to_bytes(
            "xpub6EMfjyGVUvwhpc3WKN1zXhMFGKJGMaSBPqbja4tbGoYvRBSXeTBCaqrRDjcuGTcaY95JrrAnQvDG3pdQPd"
            "tnYUCugjeksHSbyZT7rq38VQF",
            BASE58_FLAG_CHECKSUM,
            deser,
            sizeof(deser),
            &written),
        WALLY_OK);
    memcpy(multisig.xpubs[0], deser, sizeof(multisig.xpubs[0]));
    assert_int_equal(
        wally_base58_to_bytes(
            "xpub6ERxBysTYfQyY4USv6c6J1HNVv9hpZFN9LHVPu47Ac4rK8fLy6NnAeeAHyEsMvG4G66ay5aFZii2VM7wT3"
            "KxLKX8Q8keZPd67kRGmrD1WJj",
            BASE58_FLAG_CHECKSUM,
            deser,
            sizeof(deser),
            &written),
        WALLY_OK);
    memcpy(multisig.xpubs[1], deser, sizeof(multisig.xpubs[1]));
    uint8_t script[520];
    size_t script_size;

    multisig_t invalid = multisig;
    invalid.xpubs_count = 0;
    script_size = sizeof(script);
    assert_false(btc_common_pkscript_from_multisig(&invalid, 1, 2, script, &script_size));

    invalid = multisig;
    invalid.threshold = 0;
    script_size = sizeof(script);
    assert_false(btc_common_pkscript_from_multisig(&invalid, 1, 2, script, &script_size));

    invalid = multisig;
    invalid.threshold = multisig.xpubs_count + 1;
    script_size = sizeof(script);
    assert_false(btc_common_pkscript_from_multisig(&invalid, 1, 2, script, &script_size));

    invalid = multisig;
    script_size = 0;
    assert_false(btc_common_pkscript_from_multisig(&invalid, 1, 2, script, &script_size));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(_test_btc_common_pkscript_from_multisig),
        cmocka_unit_test(_test_btc_common_pkscript_from_multisig_unhappy),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
