#include "../base64_decoder.h"

#include <stdio.h>
#include <vector>
#include <assert.h>

using namespace uc;

#define BUFFER_SIZE 4096

int main() {
  Base64Decoder decoder;

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

  std::vector<unsigned char> output;
  switch (decoder.Decode(input.data(), input.size(), output)) {
  case uc::BE_INVALID_ENCODING:
    fprintf(stderr, "Invalid encoding");
    return 1;
  case uc::BE_INVALID_LENGTH:
    fprintf(stderr, "Invalid format");
    return 1;
  case uc::BE_FILE_ERROR:
    fprintf(stderr, "File error");
    return 1;
  case uc::BE_OK:
    fwrite(output.data(), 1, output.size(), stdout);
  }
  return 0;
}
