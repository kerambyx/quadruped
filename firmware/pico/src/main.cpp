#include <cstdio>
#include <cstring>

#include "Quadruped.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

#define UART_ID uart1
#define UART_BAUD_RATE 9600

#define UART_TX 4
#define UART_RX 5

// servo pulse limits (µs)
static constexpr uint16_t S_MIN = 560;
static constexpr uint16_t S_MAX = 2600;

// leg geometry
static constexpr float COXA_LEN = 0.0f;
static constexpr float FEMUR_LEN = 10.0f;
static constexpr float TIBIA_LEN = 10.0f;

// GPIO assignments
// JointConfig: { gpio, minPulse, maxPulse, minAngle, maxAngle, offset, inverted }
static const QuadrupedConfig ROBOT_CONF = {
    .frontLeft =
        {
            .coxa = {18, S_MIN, S_MAX, 0.0f, 180.0f, 0.0f, false},
            .femur = {17, S_MIN, S_MAX, 12.0f, 180.0f, 0.0f, true},
            .tibia = {16, S_MIN, S_MAX, 0.0f, 110.0f, 0.0f, false},
            .coxaLength = COXA_LEN,
            .femurLength = FEMUR_LEN,
            .tibiaLength = TIBIA_LEN,
        },
    .rearLeft =
        {
            .coxa = {19, S_MIN, S_MAX, 0.0f, 180.0f, 0.0f, false},
            .femur = {20, S_MIN, S_MAX, 12.0f, 180.0f, 0.0f, true},
            .tibia = {21, S_MIN, S_MAX, 0.0f, 110.0f, 0.0f, false},
            .coxaLength = COXA_LEN,
            .femurLength = FEMUR_LEN,
            .tibiaLength = TIBIA_LEN,
        },
    .frontRight =
        {
            .coxa = {13, S_MIN, S_MAX, 0.0f, 180.0f, -7.2f, false},
            .femur = {14, S_MIN, S_MAX, 12.0f, 180.0f, -7.2f, false},
            .tibia = {15, S_MIN, S_MAX, 0.0f, 110.0f, 0.0f, true},
            .coxaLength = COXA_LEN,
            .femurLength = FEMUR_LEN,
            .tibiaLength = TIBIA_LEN,
        },
    .rearRight =
        {
            .coxa = {12, S_MIN, S_MAX, 0.0f, 180.0f, 0.0f, false},
            .femur = {10, S_MIN, S_MAX, 12.0f, 180.0f, 7.2f, false},
            .tibia = {11, S_MIN, S_MAX, 0.0f, 110.0f, -3.6f, true},
            .coxaLength = COXA_LEN,
            .femurLength = FEMUR_LEN,
            .tibiaLength = TIBIA_LEN,
        },
    .bodyLength = 15.0f,
    .bodyWidth = 12.0f,
};

static void readline(char* buf, size_t maxlen) {
  size_t n = 0;
  while (n < maxlen - 1) {
    const int c = getchar();
    if (c == '\n' || c == '\r') break;
    buf[n++] = static_cast<char>(c);
  }
  while (n > 0 && (buf[n - 1] == '\r' || buf[n - 1] == ' ')) --n;
  buf[n] = '\0';
}

int main() {
  setvbuf(stdout, NULL, _IONBF, 0);

  stdio_init_all();

  //  initialize UART
  uart_init(UART_ID, UART_BAUD_RATE);

  gpio_set_function(UART_RX, GPIO_FUNC_UART);
  gpio_set_function(UART_TX, GPIO_FUNC_UART);

  uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);

  printf("UART READY");

  static Quadruped robot(ROBOT_CONF);

  printf("Initializing...\n");
  uart_puts(UART_ID, "Initializing.\n");

  robot.init();

  char bt_cmd[64];
  size_t bt_index = 0;

  printf("Ready.\n");
  printf(
      "Commands: home | stand | walk | look_up | look_down | tilt_right | "
      "crouch\n\n");

  uart_puts(UART_ID, "Ready.\n");
  uart_puts(UART_ID,
            "Commands: home | stand | crouch | walk | up | "
            "down | right | off | on\n");
  while (true) {
    if (uart_is_readable(UART_ID)) {
      char c = uart_getc(UART_ID);

      if (c == '\r') continue;
      if (c == '\n') {
        bt_cmd[bt_index] = '\0';

        if (strcmp(bt_cmd, "home") == 0) {
          robot.home();
          printf("Homed.\n");
          uart_puts(UART_ID, "Homed.\n");
        } else if (strcmp(bt_cmd, "stand") == 0) {
          robot.stand();
          printf("Standing.\n");
          uart_puts(UART_ID, "Standing.\n");

        } else if (strcmp(bt_cmd, "walk") == 0) {
          printf("Walking...\n");
          uart_puts(UART_ID, "Walking...\n");

          robot.trot(10);
          printf("Done.\n");
          uart_puts(UART_ID, "Done.\n");

        } else if (strcmp(bt_cmd, "up") == 0) {
          robot.setStance(15.0f, 0.0f, 0.0f);
          printf("Looking up.\n");
          uart_puts(UART_ID, "Looking up.\n");

        } else if (strcmp(bt_cmd, "down") == 0) {
          robot.setStance(-15.0f, 0.0f, 0.0f);
          printf("Looking down.\n");
          uart_puts(UART_ID, "Looking down.\n");

        } else if (strcmp(bt_cmd, "right") == 0) {
          robot.setStance(0.0f, 15.0f, 0.0f);
          printf("Tilting right.\n");
          uart_puts(UART_ID, "Tilting right.\n");

        } else if (strcmp(bt_cmd, "crouch") == 0) {
          robot.setStance(0.0f, 0.0f, -5.0f);
          printf("Crouching.\n");
          uart_puts(UART_ID, "Crouching.\n");

        } else if (strcmp(bt_cmd, "off") == 0) {
          robot.rearLeftLeg().disable();
          robot.rearRightLeg().disable();
          robot.frontLeftLeg().disable();
          robot.frontRightLeg().disable();
          printf("Powered off.\n");
          uart_puts(UART_ID, "Powered off.\n");

        }

        else if (strcmp(bt_cmd, "on") == 0) {
          robot.rearLeftLeg().enable();
          robot.rearRightLeg().enable();
          robot.frontLeftLeg().enable();
          robot.frontRightLeg().enable();
          printf("Powered on.\n");
          uart_puts(UART_ID, "Powered on.\n");

        }

        // else if (strncmp(cmd, "angles ", 7) == 0) {
        //   float c, f, t;
        //   if (sscanf(cmd + 7, "%f,%f,%f", &c, &f, &t) == 3) {
        //     robot.rearLeftLeg().testAngles(c, f, t);
        //     printf("Set coxa=%.2f femur=%.2f tibia=%.2f\n", c, f, t);
        //   } else {
        //     printf("Usage: angles <coxa>,<femur>,<tibia>\n");
        //   }
        // }

        else if (bt_index > 0) {
          // only print if sent something unrecognized
          printf("Unknown BT command: %s\n", bt_cmd);
          printf(
              "Commands: home | stand | crouch | walk | up | "
              "down | right | off | on\n");

          uart_puts(UART_ID, "Unknown BT command: ");
          uart_puts(UART_ID, bt_cmd);
          uart_putc(UART_ID, '\n');
          uart_puts(UART_ID,
                    "Commands: home | stand | crouch | walk | up | "
                    "down | right | off | on\n");
        }

        bt_index = 0;

      } else if (bt_index < sizeof(bt_cmd) - 1) {
        bt_cmd[bt_index++] = c;
      }
    }
  }
}
