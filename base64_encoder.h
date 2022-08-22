#ifndef UC_BASE64_ENCODER_H__
#define UC_BASE64_ENCODER_H__

#include <string>

namespace uc {

/**
 * \breif Encoder of base64 coding
 *
 * Currently, this is a empty class
 * For scalability, I define it be a class
 */
class Base64Encoder {
 public:
  Base64Encoder() = default;
  ~Base64Encoder() = default;
  
  /**
   * \param[in] raw Raw format data, i.e. native binary
   * \param[out] text Store the ascii ocde translated from raw
   * \param new_line Put a newline to the \p text every 64 character(inspired by openssl enc), if the last line does not hold 64 bytes, put a newline character also
   */
  void Encode(void const *raw, size_t len, std::string &text, bool newline=true);

  /**
   * \param[in] filename the name of the file store the raw format data
   * \param[out] text like Encode()
   * \param new_line like Encode()
   */
  void EncodeFromFile(char const *filename, std::string &text, bool newline=true);
};

} // uc

#endif // UC_BASE64_ENCODER_H__
