#include "seven_segment_display.h"
#include "esphome.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace seven_segment_display {

static const char *const TAG = "display.seven_segment";
const uint8_t SSEG_UNKNOWN_CHAR = 0b11111111;

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D   X
// XABCDEFG
const uint8_t SSEG_ASCII_TO_RAW[] PROGMEM = {
    0b00000000,           // ' ', ord 0x20
    0b10110000,           // '!', ord 0x21
    0b00100010,           // '"', ord 0x22
    SSEG_UNKNOWN_CHAR,  // '#', ord 0x23
    SSEG_UNKNOWN_CHAR,  // '$', ord 0x24
    0b01001001,           // '%', ord 0x25
    SSEG_UNKNOWN_CHAR,  // '&', ord 0x26
    0b00000010,           // ''', ord 0x27
    0b01001110,           // '(', ord 0x28
    0b01111000,           // ')', ord 0x29
    0b01000000,           // '*', ord 0x2A
    SSEG_UNKNOWN_CHAR,  // '+', ord 0x2B
    0b00010000,           // ',', ord 0x2C
    0b00000001,           // '-', ord 0x2D
    0b10000000,           // '.', ord 0x2E
    SSEG_UNKNOWN_CHAR,  // '/', ord 0x2F
    0b01111110,           // '0', ord 0x30
    0b00110000,           // '1', ord 0x31
    0b01101101,           // '2', ord 0x32
    0b01111001,           // '3', ord 0x33
    0b00110011,           // '4', ord 0x34
    0b01011011,           // '5', ord 0x35
    0b01011111,           // '6', ord 0x36
    0b01110000,           // '7', ord 0x37
    0b01111111,           // '8', ord 0x38
    0b01111011,           // '9', ord 0x39
    0b01001000,           // ':', ord 0x3A
    0b01011000,           // ';', ord 0x3B
    SSEG_UNKNOWN_CHAR,  // '<', ord 0x3C
    SSEG_UNKNOWN_CHAR,  // '=', ord 0x3D
    SSEG_UNKNOWN_CHAR,  // '>', ord 0x3E
    0b01100101,           // '?', ord 0x3F
    0b01101111,           // '@', ord 0x40
    0b01110111,           // 'A', ord 0x41
    0b00011111,           // 'B', ord 0x42
    0b01001110,           // 'C', ord 0x43
    0b00111101,           // 'D', ord 0x44
    0b01001111,           // 'E', ord 0x45
    0b01000111,           // 'F', ord 0x46
    0b01011110,           // 'G', ord 0x47
    0b00110111,           // 'H', ord 0x48
    0b00110000,           // 'I', ord 0x49
    0b00111100,           // 'J', ord 0x4A
    SSEG_UNKNOWN_CHAR,  // 'K', ord 0x4B
    0b00001110,           // 'L', ord 0x4C
    SSEG_UNKNOWN_CHAR,  // 'M', ord 0x4D
    0b00010101,           // 'N', ord 0x4E
    0b01111110,           // 'O', ord 0x4F
    0b01100111,           // 'P', ord 0x50
    0b11111110,           // 'Q', ord 0x51
    0b00000101,           // 'R', ord 0x52
    0b01011011,           // 'S', ord 0x53
    0b00000111,           // 'T', ord 0x54
    0b00111110,           // 'U', ord 0x55
    0b00111110,           // 'V', ord 0x56
    0b00111111,           // 'W', ord 0x57
    SSEG_UNKNOWN_CHAR,  // 'X', ord 0x58
    0b00100111,           // 'Y', ord 0x59
    0b01101101,           // 'Z', ord 0x5A
    0b01001110,           // '[', ord 0x5B
    SSEG_UNKNOWN_CHAR,  // '\', ord 0x5C
    0b01111000,           // ']', ord 0x5D
    SSEG_UNKNOWN_CHAR,  // '^', ord 0x5E
    0b00001000,           // '_', ord 0x5F
    0b00100000,           // '`', ord 0x60
    0b01110111,           // 'a', ord 0x61
    0b00011111,           // 'b', ord 0x62
    0b00001101,           // 'c', ord 0x63
    0b00111101,           // 'd', ord 0x64
    0b01001111,           // 'e', ord 0x65
    0b01000111,           // 'f', ord 0x66
    0b01011110,           // 'g', ord 0x67
    0b00010111,           // 'h', ord 0x68
    0b00010000,           // 'i', ord 0x69
    0b00111100,           // 'j', ord 0x6A
    SSEG_UNKNOWN_CHAR,  // 'k', ord 0x6B
    0b00001110,           // 'l', ord 0x6C
    SSEG_UNKNOWN_CHAR,  // 'm', ord 0x6D
    0b00010101,           // 'n', ord 0x6E
    0b00011101,           // 'o', ord 0x6F
    0b01100111,           // 'p', ord 0x70
    SSEG_UNKNOWN_CHAR,  // 'q', ord 0x71
    0b00000101,           // 'r', ord 0x72
    0b01011011,           // 's', ord 0x73
    0b00000111,           // 't', ord 0x74
    0b00011100,           // 'u', ord 0x75
    0b00011100,           // 'v', ord 0x76
    SSEG_UNKNOWN_CHAR,  // 'w', ord 0x77
    SSEG_UNKNOWN_CHAR,  // 'x', ord 0x78
    0b00100111,           // 'y', ord 0x79
    SSEG_UNKNOWN_CHAR,  // 'z', ord 0x7A
    0b00110001,           // '{', ord 0x7B
    0b00000110,           // '|', ord 0x7C
    0b00000111,           // '}', ord 0x7D
    0b01100011,           // '~', ord 0x7E (degree symbol)
};

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D   X
// XABCDEFG
// Character to segment mapping (ASCII to 7-segment)
const uint8_t SevenSegmentDisplay::char_map[95] = {
  0b00000000,            // ' ', ord 0x20
  0b10110000,            // '!', ord 0x21
  0b00100010,            // '"', ord 0x22
  0b00000000,            // '#', ord 0x23
  0b00000000,            // '$', ord 0x24
  0b01001001,            // '%', ord 0x25
  0b00000000,            // '&', ord 0x26
  0b00000010,            // ''', ord 0x27
  0b01001110,            // '(', ord 0x28
  0b01111000,            // ')', ord 0x29
  0b01000000,            // '*', ord 0x2A
  0b00000000,            // '+', ord 0x2B
  0b00010000,            // ',', ord 0x2C
  0b00000001,            // '-', ord 0x2D
  0b10000000,            // '.', ord 0x2E
  0b00000000,            // '/', ord 0x2F
  0b01111110,            // '0', ord 0x30
  0b00110000,            // '1', ord 0x31
  0b01101101,            // '2', ord 0x32
  0b01111001,            // '3', ord 0x33
  0b00110011,            // '4', ord 0x34
  0b01011011,            // '5', ord 0x35
  0b01011111,            // '6', ord 0x36
  0b01110000,            // '7', ord 0x37
  0b01111111,            // '8', ord 0x38
  0b01111011,            // '9', ord 0x39
  0b01001000,            // ':', ord 0x3A
  0b01011000,            // ';', ord 0x3B
  0b00000000,            // '<', ord 0x3C
  0b00001001,            // '=', ord 0x3D
  0b00000000,            // '>', ord 0x3E
  0b01100101,            // '?', ord 0x3F
  0b01101111,            // '@', ord 0x40
  0b01110111,            // 'A', ord 0x41
  0b00011111,            // 'B', ord 0x42
  0b01001110,            // 'C', ord 0x43
  0b00111101,            // 'D', ord 0x44
  0b01001111,            // 'E', ord 0x45
  0b01000111,            // 'F', ord 0x46
  0b01011110,            // 'G', ord 0x47
  0b00110111,            // 'H', ord 0x48
  0b00110000,            // 'I', ord 0x49
  0b00111100,            // 'J', ord 0x4A
  0b00000000,            // 'K', ord 0x4B
  0b00001110,            // 'L', ord 0x4C
  0b00000000,            // 'M', ord 0x4D
  0b00010101,            // 'N', ord 0x4E
  0b01111110,            // 'O', ord 0x4F
  0b01100111,            // 'P', ord 0x50
  0b11111110,            // 'Q', ord 0x51
  0b00000101,            // 'R', ord 0x52
  0b01011011,            // 'S', ord 0x53
  0b00000111,            // 'T', ord 0x54
  0b00111110,            // 'U', ord 0x55
  0b00111110,            // 'V', ord 0x56
  0b00111111,            // 'W', ord 0x57
  0b00000000,            // 'X', ord 0x58
  0b00100111,            // 'Y', ord 0x59
  0b01101101,            // 'Z', ord 0x5A
  0b01001110,            // '[', ord 0x5B
  0b00000000,            // '\', ord 0x5C
  0b01111000,            // ']', ord 0x5D
  0b00000000,            // '^', ord 0x5E
  0b00001000,            // '_', ord 0x5F
  0b00100000,            // '`', ord 0x60
  0b01110111,            // 'a', ord 0x61
  0b00011111,            // 'b', ord 0x62
  0b00001101,            // 'c', ord 0x63
  0b00111101,            // 'd', ord 0x64
  0b01001111,            // 'e', ord 0x65
  0b01000111,            // 'f', ord 0x66
  0b01011110,            // 'g', ord 0x67
  0b00010111,            // 'h', ord 0x68
  0b00010000,            // 'i', ord 0x69
  0b00111100,            // 'j', ord 0x6A
  0b00000000,            // 'k', ord 0x6B
  0b00001110,            // 'l', ord 0x6C
  0b00000000,            // 'm', ord 0x6D
  0b00010101,            // 'n', ord 0x6E
  0b00011101,            // 'o', ord 0x6F
  0b01100111,            // 'p', ord 0x70
  0b00000000,            // 'q', ord 0x71
  0b00000101,            // 'r', ord 0x72
  0b01011011,            // 's', ord 0x73
  0b00000111,            // 't', ord 0x74
  0b00011100,            // 'u', ord 0x75
  0b00011100,            // 'v', ord 0x76
  0b00000000,            // 'w', ord 0x77
  0b00000000,            // 'x', ord 0x78
  0b00100111,            // 'y', ord 0x79
  0b00000000,            // 'z', ord 0x7A
  0b00110001,            // '{', ord 0x7B
  0b00000110,            // '|', ord 0x7C
  0b00000111,            // '}', ord 0x7D
  0b01100011,            // '~', ord 0x7E (degree symbol)
};

// Setter methods for output pins
void SevenSegmentDisplay::set_digit1(output::BinaryOutput *digit) { this->digit1 = digit; }
void SevenSegmentDisplay::set_digit2(output::BinaryOutput *digit) { this->digit2 = digit; }
void SevenSegmentDisplay::set_digit3(output::BinaryOutput *digit) { this->digit3 = digit; }
void SevenSegmentDisplay::set_digit4(output::BinaryOutput *digit) { this->digit4 = digit; }
void SevenSegmentDisplay::set_segment_a(output::BinaryOutput *segment) { this->segment_a = segment; }
void SevenSegmentDisplay::set_segment_b(output::BinaryOutput *segment) { this->segment_b = segment; }
void SevenSegmentDisplay::set_segment_c(output::BinaryOutput *segment) { this->segment_c = segment; }
void SevenSegmentDisplay::set_segment_d(output::BinaryOutput *segment) { this->segment_d = segment; }
void SevenSegmentDisplay::set_segment_e(output::BinaryOutput *segment) { this->segment_e = segment; }
void SevenSegmentDisplay::set_segment_f(output::BinaryOutput *segment) { this->segment_f = segment; }
void SevenSegmentDisplay::set_segment_g(output::BinaryOutput *segment) { this->segment_g = segment; }
void SevenSegmentDisplay::set_dp(output::BinaryOutput *segment) { this->dp = segment; }

// ESPHome setup method
void SevenSegmentDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Seven Segment Display...");
  if (!this->digit1 || !this->digit2 || !this->digit3 || !this->digit4 ||
      !this->dp || !this->segment_a || !this->segment_b || !this->segment_c ||
      !this->segment_d || !this->segment_e || !this->segment_f || !this->segment_g) {
    ESP_LOGE("SevenSegmentDisplay", "One or more output pins are not initialized properly!");
  }
  this->clear_display();
}

// // ESPHome loop method
// void SevenSegmentDisplay::loop() {
//   unsigned long now = millis();
//   if (now - last_update_time >= digit_update_interval) {
//     last_update_time = now;
//     set_active_digit(-1); // Turn off all digits
//     // Update the current digit
//     char ch = text[current_digit];
//     uint8_t segments = char_map[ch - 32]; //Get the segment data
//     update_segments(segments);
//     set_active_digit(current_digit); // Activate the current digit
//     current_digit = (current_digit + 1) % 4; // Move to the next digit
//   }
// }

void SevenSegmentDisplay::update() {
  for (uint8_t &i : this->buffer_)
    i = 0;
  if (this->writer_.has_value())
    (*this->writer_)(*this);
}

void SevenSegmentDisplay::loop() { this->display(); }

float SevenSegmentDisplay::get_setup_priority() const { return setup_priority::PROCESSOR; }

void SevenSegmentDisplay::display() {
  unsigned long now = millis();
  if (now - last_update_time >= digit_update_interval) {
    last_update_time = now;
    set_active_digit(-1); // Turn off all digits
    // Update the current digit
    //char ch = text[current_digit];
    //uint8_t segments = char_map[ch - 32]; //Get the segment data
    update_segments(buffer_[current_digit]);
    set_active_digit(current_digit); // Activate the current digit
    current_digit = (current_digit + 1) % 4; // Move to the next digit
  }
}

// Set the text to be displayed
// void SevenSegmentDisplay::set_text(const std::string &new_text) {
//   text = new_text.substr(0, 4);  // Ensure the text fits 4 digits
// }

// Update the segment outputs
void SevenSegmentDisplay::update_segments(uint8_t segments) {
  this->segment_a->set_state(segments & 0b01000000);
  this->segment_b->set_state(segments & 0b00100000);
  this->segment_c->set_state(segments & 0b00010000);
  this->segment_d->set_state(segments & 0b00001000);
  this->segment_e->set_state(segments & 0b00000100);
  this->segment_f->set_state(segments & 0b00000010);
  this->segment_g->set_state(segments & 0b00000001);
  this->dp->set_state(segments & 0b10000000);
}

// Activate a specific digit
void SevenSegmentDisplay::set_active_digit(int digit) {
  this->digit1->set_state(digit == 0);
  this->digit2->set_state(digit == 1);
  this->digit3->set_state(digit == 2);
  this->digit4->set_state(digit == 3);
}
// Clear the display for the next frame
void SevenSegmentDisplay::clear_display() {
  this->digit1->set_state(false);
  this->digit2->set_state(false);
  this->digit3->set_state(false);
  this->digit4->set_state(false);
  this->segment_a->set_state(false);
  this->segment_b->set_state(false);
  this->segment_c->set_state(false);
  this->segment_d->set_state(false);
  this->segment_e->set_state(false);
  this->segment_f->set_state(false);
  this->segment_g->set_state(false);
  this->dp->set_state(false);
}

uint8_t SevenSegmentDisplay::print(uint8_t start_pos, const char *str) {
  // ESP_LOGV(TAG, "Print at %d: %s", start_pos, str);
  uint8_t pos = start_pos;
  bool use_dot = false;
  for (; *str != '\0'; str++) {
    uint8_t data = SSEG_UNKNOWN_CHAR;
    if (*str >= ' ' && *str <= '~')
      data = progmem_read_byte(&SSEG_ASCII_TO_RAW[*str - ' ']);

    if (data == SSEG_UNKNOWN_CHAR) {
      ESP_LOGW(TAG, "Encountered character '%c' with no representation while translating string!", *str);
    }
    // Remap segments, for compatibility with MAX7219 segment definition which is
    // XABCDEFG, but TM1637 is // XGFEDCBA
    // if (this->inverted_) {
    //   // XABCDEFG > XGCBAFED
    //   data = ((data & 0x80) || use_dot ? 0x80 : 0) |  // no move X
    //          ((data & 0x40) ? 0x8 : 0) |              // A
    //          ((data & 0x20) ? 0x10 : 0) |             // B
    //          ((data & 0x10) ? 0x20 : 0) |             // C
    //          ((data & 0x8) ? 0x1 : 0) |               // D
    //          ((data & 0x4) ? 0x2 : 0) |               // E
    //          ((data & 0x2) ? 0x4 : 0) |               // F
    //          ((data & 0x1) ? 0x40 : 0);               // G
    // } else {
    //   // XABCDEFG > XGFEDCBA
    //   data = ((data & 0x80) ? 0x80 : 0) |  // no move X
    //          ((data & 0x40) ? 0x1 : 0) |   // A
    //          ((data & 0x20) ? 0x2 : 0) |   // B
    //          ((data & 0x10) ? 0x4 : 0) |   // C
    //          ((data & 0x8) ? 0x8 : 0) |    // D
    //          ((data & 0x4) ? 0x10 : 0) |   // E
    //          ((data & 0x2) ? 0x20 : 0) |   // F
    //          ((data & 0x1) ? 0x40 : 0);    // G
    // }
    use_dot = *str == '.';
    if (use_dot) {
      if ((!this->inverted_) && (pos != start_pos)) {
        this->buffer_[pos - 1] |= 0b10000000;
      }
    } else {
      if (pos >= 4) {
        ESP_LOGE(TAG, "String is too long for the display!");
        break;
      }
      this->buffer_[pos++] = data;
    }
  }
  return pos - start_pos;
}
uint8_t SevenSegmentDisplay::print(const char *str) { return this->print(0, str); }
uint8_t SevenSegmentDisplay::printf(uint8_t pos, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[64];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    return this->print(pos, buffer);
  return 0;
}
uint8_t SevenSegmentDisplay::printf(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[64];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    return this->print(buffer);
  return 0;
}

uint8_t SevenSegmentDisplay::strftime(uint8_t pos, const char *format, ESPTime time) {
  char buffer[64];
  size_t ret = time.strftime(buffer, sizeof(buffer), format);
  if (ret > 0)
    return this->print(pos, buffer);
  return 0;
}
uint8_t SevenSegmentDisplay::strftime(const char *format, ESPTime time) { return this->strftime(0, format, time); }

}  // namespace seven_segment_display
}  // namespace esphome
