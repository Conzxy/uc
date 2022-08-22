# uc
A simple base64 codec(encoder &amp; decoder)

## Usage
### Encode
```cpp
uc::Base64Encoder encoder;
char buf[4096]; // binary data
// ...
std::string base64_text;
// false indicates don't put newline character every 64 bytes
encoder.Encode(buf, sizeof buf, base64_text, false);
```

### Decode
```cpp
uc::Base64Decoder decoder;
char base64_buf[4096]; // base64 encoding data
// ...
std::vector<unsigned char> binary_data;
decoder.Decode(base64_buf, sizeof buf, binary_data, false);
```
More detail please see the `base64_*.*`
## reference
[wikipedia](https://en.wikipedia.org/wiki/Base64)

[rfc2045 section 6.8](http://www.faqs.org/rfcs/rfc2045.html)
