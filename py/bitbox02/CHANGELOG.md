# Changelog

## 6.2.0
- btc_sign: allow displaying BTC values in the 'sat' unit
- require hidapi 0.14.0 to fix a bug on macOS 13.3 which lists two BitBox02s instead of one

## 6.1.1
- Update protobuf dependency to >= 3.20, for better compatibility

## 6.1.0
- Add `eth_sign_typed_msg()`
- Update protobuf dependency to >= 3.21
- Regenerate protobuf files with protoc v3.21.2

## 6.0.0
- Offset the recoverable ID by 27 in the signature returned by `eth_sign_msg()`.
- Rename `BTCOutputExternal.hash` to `BTCOutputExternal.payload`.
- Support P2TR (taproot) receive addresses and transaction inputs
- `eth_pub()`, `eth_sign()` and `eth_sign_msg()` now take the network chain ID instead of a ETHCoin enum value for network identification
