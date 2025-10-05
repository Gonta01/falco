import re

input_file = "fprime-falco/HAL_MMCU/firmware/src/config/default/peripheral/pio/plib_pio.c"
output_file = "fprime-falco/Falco/Drv/I2c0Driver/I2C0DriverFuncs.template.cpp"

# List of registers we care about
registers = [
    "PIO_PDR",
    "PIO_PER",
    "PIO_MDER",
    "PIO_MDDR",
    "PIO_OER",
    "PIO_ODR"
]

# Bitwise transformations for GPIO config
gpio_expr = {
    "PIO_PDR":  "orig & ~0x18",
    "PIO_PER":  "orig | 0x18",
    "PIO_MDER": "orig | 0x18",
    "PIO_MDDR": "orig & ~0x18",
    "PIO_OER":  "orig | 0x18",
    "PIO_ODR":  "orig & ~0x18",
}

# Extract a clean Python-evaluable expression from a C value string
def parse_c_expr(c_expr):
    # Remove 'U' suffix and spaces
    c_expr = c_expr.replace(" ", "").replace("U", "")
    # Replace ~0x... with bitwise NOT of int
    if c_expr.startswith("~0x"):
        value = int(c_expr[1:], 16)
        return ~value & 0xFFFFFFFF  # emulate 32-bit wraparound
    elif c_expr.startswith("0x"):
        return int(c_expr, 16)
    else:
        try:
            return int(c_expr, 0)
        except ValueError:
            return 0

# Pattern to match assignments to PIO_PORT_A
pattern_template = r"\(\(pio_registers_t\s*\*\)\s*PIO_PORT_A\)->({reg})\s*=\s*([^;]+);"

# Extract values from the file
reg_values = {}

with open(input_file, "r") as f:
    content = f.read()

for reg in registers:
    pattern = pattern_template.format(reg=reg)
    match = re.search(pattern, content)
    if match:
        raw_val = match.group(2).strip()
        try:
            parsed = parse_c_expr(raw_val)
            reg_values[reg] = parsed
        except Exception as e:
            print(f"⚠️ Could not parse value for {reg}: '{raw_val}', defaulting to 0x0")
            reg_values[reg] = 0
    else:
        reg_values[reg] = 0  # default

# Format a line for I2C (original)
def generate_line_i2c(reg, value):
    return f"    ((pio_registers_t*)PIO_PORT_A)->{reg} = {hex(value)};"

# Format a line for GPIO (modified)
def generate_line_gpio(reg, orig):
    expr = gpio_expr[reg]
    try:
        value = eval(expr.replace("orig", str(orig)))
    except Exception as e:
        print(f"⚠️ Eval failed for {reg} with orig={orig}")
        value = 0
    return f"    ((pio_registers_t*)PIO_PORT_A)->{reg} = {hex(value)};"

# Write output file
with open(output_file, "w") as f:
    f.write("void configureGPIOPins() {\n")
    for reg in registers:
        f.write(generate_line_gpio(reg, reg_values[reg]) + "\n")
    f.write("}\n\n")

    f.write("void configureI2cPins() {\n")
    for reg in registers:
        f.write(generate_line_i2c(reg, reg_values[reg]) + "\n")
    f.write("}\n")

print("✅ Done: I2C0DriverFuncs.template.cpp generated with correct values.")