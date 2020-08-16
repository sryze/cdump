/*
 * Copyright (c) 2019-2020 Sergey Zolotarev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FILE *file;
  FILE *out_file;
  unsigned char c;
  int i;

  if (argc < 3) {
    fprintf(stderr, "Usage: cdump file var_name [out_file]\n");
    return EXIT_FAILURE;
  }

  if (argc >= 4) {
    out_file = fopen(argv[3], "w");
    if (out_file == NULL) {
      fprintf(stderr, "Could not open output file: %s\n", strerror(errno));
      return EXIT_FAILURE;
    }
  } else {
    out_file = stdout;
  }

  file = fopen(argv[1], "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open input file: %s\n", strerror(errno));
    return EXIT_FAILURE;
  }

  fprintf(out_file,
      "/* This file was automatically generated by cdump. DO NOT EDIT. */\n\n");
  fprintf(out_file, "static const char %s[] = {\n", argv[2]);

  i = 0;
  while ((fread(&c, sizeof(c), 1, file)) != 0) {
    if (i == 0) {
      fprintf(out_file, "\t\"");
    }
    fprintf(out_file, "\\x%02x", c);
    i += 4;
    if (i >= 68) {
      fputs("\"\n", out_file);
      i = 0;
    }
  }

  if (i != 0) {
    fputs("\"\n", out_file);
  }
  fprintf(out_file, "\n};\n");

  return EXIT_SUCCESS;
}
