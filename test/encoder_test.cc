#include "../base64_encoder.h"

#include <gtest/gtest.h>

using namespace uc;

Base64Encoder encoder;

TEST(encoder, simple) {
  char const data[] = "Man";
  std::string output;

  encoder.Encode(data, sizeof data-1, output, false);
  EXPECT_EQ(output, "TWFu");
}

TEST(encoder, long) {
  char const data[] = "Many hands make light work.";
  std::string output;
  encoder.Encode(data, sizeof data-1, output, false);
  EXPECT_EQ("TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu", output);
}
