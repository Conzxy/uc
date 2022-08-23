#include "base64_encoder.h"

#include <assert.h>

#include "file_util.h"

using namespace uc;

/* Construct a map: {base64 index -> printable ascii code} */
static char base64_apphabet[64];
static int generate_base64_alphabet() noexcept {
  int cur = 0;
  for (int i = 0; i < 26; ++i)
    base64_apphabet[cur++] = 'A' + i;
  for (int i = 0; i < 26; ++i)
    base64_apphabet[cur++] = 'a' + i;
  for (int i = 0; i < 10; ++i)
    base64_apphabet[cur++] = '0' + i;
  base64_apphabet[cur++] = '+';
  base64_apphabet[cur++] = '/';
  assert(cur == sizeof base64_apphabet);
  return 0;
}

static char GetBase64Char(unsigned char index) noexcept {
  assert(index < 64 && index >= 0);
  static int dummy = generate_base64_alphabet();
  return base64_apphabet[index]; 
}

/*
 * |  char 1   |  char 2   |  char 3   |
 * | ****** ** | **** **** | ** ****** |
 * |       |         |         |       |
 */

static inline unsigned char GetBase64Index1(unsigned char const *raw) noexcept {
  return raw[0] >> 2;
}

static inline unsigned char GetBase64Index2(unsigned char const *raw) noexcept {
  return ((0x03 & raw[0]) << 4) + (raw[1] >> 4);
}

static inline unsigned char GetBase64Index3(unsigned char const *raw) noexcept {
  return ((0x0f & raw[1]) << 2) + (raw[2] >> 6);
}

static inline unsigned char GetBase64Index4(unsigned char const *raw) noexcept {
  return 0x3f & raw[2];
}

static inline unsigned char GetBase64IndexPadding1(unsigned char const *raw) noexcept {
  return (0x0f & raw[1]) << 2;
}

static inline unsigned char GetBase64IndexPadding2(unsigned char const *raw) noexcept {
  return (0x03 & raw[0]) << 4;
}

void Base64Encoder::Encode(void const *raw_data, size_t len, std::string &text, bool newline) {
  size_t written = 0;
  unsigned char const *binary = (unsigned char const*)raw_data;
  auto final_size = size_t((len * 4) / 3.0);
  text.reserve(final_size + (newline ? 
        (final_size / 64 + ((final_size & 63) == 0 ? 0 : 1)) : 0));

  size_t i = 0;
  /* Avoid underflow 
   * Not len - i >= 3 */
  for (; len >= 3 + i; ) {
    text.push_back(GetBase64Char(GetBase64Index1(binary)));
    text.push_back(GetBase64Char(GetBase64Index2(binary)));
    text.push_back(GetBase64Char(GetBase64Index3(binary)));
    text.push_back(GetBase64Char(GetBase64Index4(binary)));
    binary += 3;
    i += 3;
    written += 4;
    if (newline && ((written & 63) == 0))
      text.push_back('\n');
  }
 
  /* | x..x | xx |
   * | 3n   i   len |
   * padding = 3 - left = 3 - (len - i) */
  int padding = 3 - (len - i);
  assert(padding >= 0 && padding <= 3);
  
  if (padding == 1) {
    text.push_back(GetBase64Char(GetBase64Index1(binary)));
    text.push_back(GetBase64Char(GetBase64Index2(binary)));
    text.push_back(GetBase64Char(GetBase64IndexPadding1(binary)));
    text.push_back('=');
  } else if (padding == 2) {
    text.push_back(GetBase64Char(GetBase64Index1(binary)));
    text.push_back(GetBase64Char(GetBase64IndexPadding2(binary)));
    text.push_back('=');
    text.push_back('=');
  }
  
  // The last new line don't put only when
  // the new line has been put, i.e.,
  // the old written % 64 == 0, and the data is empty
  if (newline && len > 0 && ((written & 63) != 0))
    text.push_back('\n');
}

/** Must be the multipiler of 3 */
static constexpr size_t BUFSIZE = 1024 * 3;

void Base64Encoder::EncodeFromFile(char const *filename, std::string &text, bool newline) {
  FILE *fp = fopen(filename, "r");
  if (!fp) return;
  DeferFclose df(fp);
  
  char buf[BUFSIZE];
  size_t count = 0;
  for (;;) {
    count = fread(buf, 1, sizeof buf, fp);
    if (count < sizeof buf) {
      if (feof(fp)) {
        Encode(buf, count, text, newline);
        break;
      } else if (ferror(fp)) {
        return;
      }
    }
    assert(count == sizeof buf);
    Encode(buf, count, text, newline);
  }
}
