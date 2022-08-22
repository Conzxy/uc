#include "../base64_decoder.h"

#include <gtest/gtest.h>

using namespace uc;

Base64Decoder decoder;

void test_decode(char const *text, size_t len, char const *corret_raw) {
  std::vector<unsigned char> raw;
  decoder.Decode(text, sizeof text-1, raw, false);
  
  EXPECT_EQ(std::equal(raw.begin(), raw.end(), corret_raw), true);;
}

TEST(decoder, simple) {
  char const text[] = "TWFu";
  char const raw[] = "Man";
  test_decode(text, sizeof text, raw);
}

TEST(decoder, long) {
  char const text[] = "TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu";
  char const raw[] = "Many hands make light work.";
  test_decode(text, sizeof text, raw);
} 
