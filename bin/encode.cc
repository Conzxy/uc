#include "../base64_encoder.h"

#include <stdio.h>
#include <vector>
#include <assert.h>

using namespace uc;

#define BUFFER_SIZE 4096

int main() {
  Base64Encoder encoder;

  std::vector<char> input;
  char buf[BUFFER_SIZE];
  input.reserve(BUFFER_SIZE);
  for (;;) {
    auto count = fread(buf, 1, sizeof buf, stdin);
    if (count < BUFFER_SIZE) {
      if (feof(stdin)) {
        input.insert(input.end(), buf, buf+count);
        break;
      } else if (ferror(stdin)) {
        perror("Error occurred");
        exit(1);
      } else {
        assert(false && "Unexpected error occurred");
      }
    }
    input.insert(input.end(), buf, buf+sizeof buf);
  }

  std::string output;
  encoder.Encode(input.data(), input.size(), output);
  fwrite(output.data(), 1, output.size(), stdout);
}
