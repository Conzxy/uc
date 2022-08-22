#ifndef UC_BASE64_DECODER_H__
#define UC_BASE64_DECODER_H__

#include <vector>
#include <string>

namespace uc {

enum Base64ErrorCode {
  BE_OK = 0,
  BE_INVALID_ENCODING, /** Not valid base64 ascii code */
  BE_INVALID_LENGTH, /** Not a valid length(4n except newline character */
  BE_FILE_ERROR, /** File(e.g. fread(), fopen()) error */
};

/**
 * \brief Decorder of base64 encoding
 */
class Base64Decoder {
 public:
  Base64Decoder() = default;
  ~Base64Decoder() = default;
  
  /**
   * \param[in] text Store the base64 encoding content
   * \param[in] len Length of the \p text
   * \param[out] raw Store the translated raw format data
   * \param new_line There is a new line character every 64 bytes
   */
  Base64ErrorCode Decode(char const *text, size_t len, 
                         std::vector<unsigned char> &raw,
                         bool newline=true);
  
  /** std::string version of Decode(text, len, raw, newline) */
  Base64ErrorCode Decode(std::string const &text, 
                         std::vector<unsigned char> &raw,
                         bool newline=true) 
  { return Decode(text.data(), text.size(), raw, newline); }

  /**
   * \param[in] filename The name of the file store the base64 encoding content
   * \param[out] raw Like Decode(text, len, raw, newline)
   * \param new_line Like Decode(text, len, raw, newline)
   */
  Base64ErrorCode DecodeFromFile(char const *filename, 
                                 std::vector<unsigned char> &raw, bool newline=true);
};

} // uc

#endif // UC_BASE64_DECODER_H__
