#include "base64_decoder.h"

#include <assert.h>
#include <unordered_map>

using namespace uc;

#define DECODER_DEBUG 0

/* Construct a map: {ascii code -> base64 encoding} */
static std::unordered_map<char, unsigned char> ascii_base64_map;
static inline int generate_ascii_base64_map() {
  unsigned char i = 0;
  for (char c = 'A'; c <= 'Z'; ++c)
    ascii_base64_map[c] = i++;
  for (char c = 'a'; c <= 'z'; ++c)
    ascii_base64_map[c] = i++;
  for (char c = '0'; c <= '9'; ++c)
    ascii_base64_map[c] = i++;
  ascii_base64_map['+'] = i++;
  ascii_base64_map['/'] = i++;
  assert(i == 64);
  return 0;
}

static unsigned char GetBase64(char ascii) noexcept {
  static int dummy = generate_ascii_base64_map();
  auto iter = ascii_base64_map.find(ascii);
  if (iter == ascii_base64_map.end()) return -1;
  return iter->second;
}

/* There are four ascii code,
 * ascii1 -> 00xxxxxx
 * ascii2 -> 00xxxxxx
 * ascii3 -> 00xxxxxx
 * ascii4 -> 00xxxxxx */

/* Set the parameters are base64 encoding character to
 * avoid call multiple GetBase64() and do errror handling in the outter */

static unsigned char GetBase256_1(char ch1, char ch2) noexcept {
  return (ch1 << 2) + (ch2 >> 4);
}

static unsigned char GetBase256_2(char ch2, char ch3) noexcept {
  return (ch2 << 4) + (ch3 >> 2);
}

static unsigned char GetBase256_3(char ch3, char ch4) noexcept {
  return (ch3 << 6) + ch4;
}

Base64ErrorCode Base64Decoder::Decode(char const *text, size_t len,
    std::vector<unsigned char> &raw, bool newline) {
  if (len == 0) return BE_OK;

  auto new_line_count = newline ? (len / 65  + (len % 65 == 0 ? 0 : 1)) : 0;
#if DECODER_DEBUG
  printf("len = %zu\n", len);
  printf("new_line_count = %zu\n", new_line_count);
#endif

  if (((len - new_line_count) & 3) != 0) 
    return BE_INVALID_LENGTH;

  // We can compute the size of decoded content
  raw.reserve(size_t((len - new_line_count) * (3.0/4)));

  char ch1, ch2, ch3, ch4;

  // Remove the last newline character
  if (newline) len--;
  // Handling the last four ascii specially
  const size_t end = text[len-1] == '=' ? len - 4 : len;

  for (size_t i = 0; i < end;) {
    // Omit the newline character
    ch1 = GetBase64(text[i++]);
    ch2 = GetBase64(text[i++]);
    ch3 = GetBase64(text[i++]);
    ch4 = GetBase64(text[i++]);
    if (ch1 == -1 || ch2 == -1 || ch3 == -1 || ch4 == -1) {
      fprintf(stderr, "i = %zu, ch1 = %c, ch2 = %c, ch3 = %c, ch4 = %c\n", i, text[i-4], text[i-3], text[i-2], text[i-1]);
      return BE_INVALID_ENCODING;
    }
  
    raw.push_back(GetBase256_1(ch1, ch2));
    raw.push_back(GetBase256_2(ch2, ch3));
    raw.push_back(GetBase256_3(ch3, ch4));

    if (newline && text[i] == '\n') ++i;
  }
  
  if (text[len-1] == '=') {
    /* | 00xxxxxx | 00xxxxxx | = | = | */
    if (text[len-2] == '=') {
      ch1 = GetBase64(text[len-4]);
      ch2 = GetBase64(text[len-3]);
      raw.push_back(GetBase256_1(ch1, ch2));
    } else {
      ch1 = GetBase64(text[len-4]);
      ch2 = GetBase64(text[len-3]);
      ch3 = GetBase64(text[len-2]);
      raw.push_back(GetBase256_1(ch1, ch2));
      raw.push_back(GetBase256_2(ch2, ch3));
    }
  }

  return BE_OK;
}

static constexpr size_t BUFSIZE = 4096;

Base64ErrorCode Base64Decoder::DecodeFromFile(char const *filename, 
    std::vector<unsigned char> &raw, bool newline) {
  FILE *fp = fopen(filename, "r");
  char buf[BUFSIZE];
  size_t count = 0;
  Base64ErrorCode error_code;
  for (;;) {
    count = fread(buf, 1, sizeof buf, fp);
    if (count < sizeof buf) {
      if (feof(fp)) {
        error_code = Decode(buf, count, raw, newline); 
        if (error_code != BE_OK)
          return error_code;
        break;
      } else if (ferror(fp)) {
        return BE_FILE_ERROR;
      }
    }
    assert(count == sizeof buf);
    error_code = Decode(buf, count, raw, newline);
    if (error_code != BE_OK)
      return error_code;
  }
  fclose(fp);
  return BE_OK;
}
