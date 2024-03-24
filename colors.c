
char* file_colors[] = {
    "\e[0;32m",
    "\033[38:5:4m",
    "\e[0;34m",
    "\033[38:5:208m",
    "\033[38:5:255m",
    "\e[0;32m",
    "\033[38:5:73m",
    "\033[0m",
};

typedef enum colors {
  green,
  blue,
  orange,
  light_green,
  light_blue,
  white,
} colors;

typedef enum renderable_colors {
  size = green,
  time = blue,
  dir = blue,
  source = orange,
  reg = white,
  executable = green, // size and exe should be diff colors
  lnk = light_blue,
  reset_colors = white,
} renderable_colors;

struct color {
  char** fg;
  char** bg;
};
