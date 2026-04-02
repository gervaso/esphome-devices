#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/hal.h"
#include "esphome/core/time.h"

#include <vector>

namespace esphome {
namespace seven_segment_display {

class SevenSegmentDisplay;

using sseg_writer_t = std::function<void(SevenSegmentDisplay &)>;

class SevenSegmentDisplay : public PollingComponent {
  public:
    void set_writer(sseg_writer_t &&writer) { this->writer_ = writer;}

    // Setters for pins
    void set_digit1(output::BinaryOutput *digit);
    void set_digit2(output::BinaryOutput *digit);
    void set_digit3(output::BinaryOutput *digit);
    void set_digit4(output::BinaryOutput *digit);
    void set_segment_a(output::BinaryOutput *segment);
    void set_segment_b(output::BinaryOutput *segment);
    void set_segment_c(output::BinaryOutput *segment);
    void set_segment_d(output::BinaryOutput *segment);
    void set_segment_e(output::BinaryOutput *segment);
    void set_segment_f(output::BinaryOutput *segment);
    void set_segment_g(output::BinaryOutput *segment);
    void set_dp(output::BinaryOutput *segment);
    void set_inverted(bool inverted) { this->inverted_ = inverted; }
    void set_length(uint8_t length) { this->length_ = length; }

    // ESPHome lifecycle methods
    void setup() override;
    void loop() override;

    // Required display logic methods
    void clear_display();
    //void draw() override;
    //void print(const char *text) override;

    float get_setup_priority() const override;

    void update() override;

    /// Evaluate the printf-format and print the result at the given position.
    uint8_t printf(uint8_t pos, const char *format, ...) __attribute__((format(printf, 3, 4)));
    /// Evaluate the printf-format and print the result at position 0.
    uint8_t printf(const char *format, ...) __attribute__((format(printf, 2, 3)));

    /// Print `str` at the given position.
    uint8_t print(uint8_t pos, const char *str);
    /// Print `str` at position 0.
    uint8_t print(const char *str);

    // Display control methods
    //void set_text(const std::string &new_text);

    void display();

    /// Evaluate the strftime-format and print the result at the given position.
    uint8_t strftime(uint8_t pos, const char *format, ESPTime time) __attribute__((format(strftime, 3, 0)));
    /// Evaluate the strftime-format and print the result at position 0.
    uint8_t strftime(const char *format, ESPTime time) __attribute__((format(strftime, 2, 0)));

  protected:
    bool inverted_;
    uint8_t length_;
    uint8_t buffer_[4] = {0};
    optional<sseg_writer_t> writer_{};

  private:
    // Helper methods
    void update_segments(uint8_t segments);
    void set_active_digit(int digit);

    // Output pins for digits and segments
    output::BinaryOutput *digit1;
    output::BinaryOutput *digit2;
    output::BinaryOutput *digit3;
    output::BinaryOutput *digit4;
    output::BinaryOutput *dp;
    output::BinaryOutput *segment_a, *segment_b, *segment_c, *segment_d, *segment_e, *segment_f, *segment_g;

    // State variables for multiplexing
    std::string text = "    ";
    int current_digit = 0;
    unsigned long last_update_time = 0;
    unsigned long digit_update_interval = 2;  // Time per digit (milliseconds)

    // Character to segment map (ASCII to 7-segment)
    static const uint8_t char_map[95];
};

} // namespace seven_segment_display
} // namespace esphome
