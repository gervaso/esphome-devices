import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, output
from esphome.const import (
  CONF_ID,
  CONF_LAMBDA,
  CONF_UPDATE_INTERVAL,
  CONF_INVERTED,
  CONF_LENGTH,
)

seven_segment_display_ns = cg.esphome_ns.namespace("seven_segment_display")
SevenSegmentDisplay = seven_segment_display_ns.class_("SevenSegmentDisplay", cg.PollingComponent)
SevenSegmentDisplayRef = SevenSegmentDisplay.operator("ref")

# Configuration keys
CONF_DIGIT1 = "digit1"
CONF_DIGIT2 = "digit2"
CONF_DIGIT3 = "digit3"
CONF_DIGIT4 = "digit4"
CONF_SEGMENT_A = "segment_a"
CONF_SEGMENT_B = "segment_b"
CONF_SEGMENT_C = "segment_c"
CONF_SEGMENT_D = "segment_d"
CONF_SEGMENT_E = "segment_e"
CONF_SEGMENT_F = "segment_f"
CONF_SEGMENT_G = "segment_g"
CONF_DP = "dp"


CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend(
  {
    cv.GenerateID(): cv.declare_id(SevenSegmentDisplay),
    cv.Required(CONF_DIGIT1): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_DIGIT2): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_DIGIT3): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_DIGIT4): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_SEGMENT_A): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_SEGMENT_B): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_SEGMENT_C): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_SEGMENT_D): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_SEGMENT_E): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_SEGMENT_F): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_SEGMENT_G): cv.use_id(output.BinaryOutput),
    cv.Required(CONF_DP): cv.use_id(output.BinaryOutput),
    cv.Optional(CONF_UPDATE_INTERVAL, default="100ms"): cv.update_interval,
    cv.Optional(CONF_INVERTED, default=False): cv.boolean,
    cv.Optional(CONF_LENGTH, default=4): cv.All(cv.uint8_t, cv.Range(min=1, max=4))
  }
).extend(cv.polling_component_schema("1s"))


async def to_code(config):
  # Create the display component
  var = cg.new_Pvariable(config[CONF_ID])

  # Register it as a display
  await display.register_display(var, config)

  # Assign output pins
  cg.add(var.set_digit1(await cg.get_variable(config[CONF_DIGIT1])))
  cg.add(var.set_digit2(await cg.get_variable(config[CONF_DIGIT2])))
  cg.add(var.set_digit3(await cg.get_variable(config[CONF_DIGIT3])))
  cg.add(var.set_digit4(await cg.get_variable(config[CONF_DIGIT4])))
  cg.add(var.set_segment_a(await cg.get_variable(config[CONF_SEGMENT_A])))
  cg.add(var.set_segment_b(await cg.get_variable(config[CONF_SEGMENT_B])))
  cg.add(var.set_segment_c(await cg.get_variable(config[CONF_SEGMENT_C])))
  cg.add(var.set_segment_d(await cg.get_variable(config[CONF_SEGMENT_D])))
  cg.add(var.set_segment_e(await cg.get_variable(config[CONF_SEGMENT_E])))
  cg.add(var.set_segment_f(await cg.get_variable(config[CONF_SEGMENT_F])))
  cg.add(var.set_segment_g(await cg.get_variable(config[CONF_SEGMENT_G])))
  cg.add(var.set_dp(await cg.get_variable(config[CONF_DP])))
  cg.add(var.set_inverted(config[CONF_INVERTED]))
  cg.add(var.set_length(config[CONF_LENGTH]))

  # # Handle optional lambda
  if CONF_LAMBDA in config:
    lambda_ = await cg.process_lambda(
        config[CONF_LAMBDA], [(SevenSegmentDisplayRef, "it")], return_type=cg.void
    )
    cg.add(var.set_writer(lambda_))

