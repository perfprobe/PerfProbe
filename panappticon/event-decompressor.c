#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "minilzo/minilzo.h"

struct globalArgs_t {
  int singleBuffer;  /* -1 option */
  int ignoreExtra;   /* -I option */
} globalArgs;

static const char *optString = "1I";

void display_usage( void ) {
  fputs("Usage: event-decompressor -1I\n", stderr);
  fputs("Decompresses a kernel event stream. Reads from stdin and writes to stdout.\n", stderr);
  fputs("Options:\n", stderr);
  fputs("  -1      process only the first buffer in the stream\n", stderr);
  fputs("  -I      ignore extra data after first buffer, if -1 is specified.\n", stderr);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  FILE* stream = stdin; 
  FILE* ostream = stdout;

  int opt = 0;

  int ret = 0;
  unsigned int len;
  lzo_uint uncmp_len;
  size_t remain;

  unsigned char* uncompressed;
  unsigned char* compressed;

  globalArgs.singleBuffer = 0; /* false */
  globalArgs.ignoreExtra = 0; /* false */

  opt = getopt(argc, argv, optString);
  while (opt != -1) {
    switch (opt) {
    case '1':
      globalArgs.singleBuffer = 1; /* true */
      break;
    case 'I':
      globalArgs.ignoreExtra = 1; /* true */
      break;
    case '?':
      display_usage();
      break;
    default:
      /* Can't get here */
      break;
    }

    opt = getopt(argc, argv, optString);
  }

  while (fread(&len, 4, 1, stream)) {
    if (ferror(stream))
      goto errif;
    if (feof(stream))
      goto out;

    compressed = malloc(len);
    if (!compressed)
      goto errmem;
    uncmp_len = 10 * len; // Hopefully 10X is enough...
    uncompressed = malloc(uncmp_len); 
    if (!uncompressed)
      goto errmem;

    remain = len;
    remain -= len * fread(compressed, len, 1, stream);
    if (remain) {
      if (ferror(stream))
	goto errif;
      else if (feof(stream))
	goto erreof;
      else 
	goto err;
    }
    ret = lzo1x_decompress_safe(compressed, len, uncompressed, &uncmp_len, NULL);
    if (ret)
      goto errlzo;
    
    remain = uncmp_len;
    remain -= uncmp_len * fwrite(uncompressed, uncmp_len, 1, ostream);
    if (remain) {
      if (ferror(stream))
	goto errof;
      else
	goto err;
    }
    
    free(compressed);
    compressed = NULL;

    free(uncompressed);
    uncompressed = NULL;

    if (globalArgs.singleBuffer) {
      fread(&len, 1, 1, stream);
      if (!feof(stream) && !globalArgs.ignoreExtra)
	fputs("warning: stopping after first buffer in stream.\n", stderr);
      return 0;
    }

  }

  return 0;
  
 err:
  fprintf(stderr, "Unknown error\n");
  ret = 1;
  goto out;

 errlzo:
  fprintf(stderr, "Decompression error: %d\n", ret);
  ret = 1;
  goto out;

 errof:
  fprintf(stderr, "Error while writing output: %d.\n", ferror(ostream));
  ret = 1;
  goto out;

 errif:
  fprintf(stderr, "Error while reading input: %d.\n", ferror(stream));
  ret = 1;
  goto out;

 erreof:
  fprintf(stderr, "Unexpected end-of-file.\n");
  ret = 1;
  goto out;

 errmem:
  fprintf(stderr, "Failed to allocate memory.\n");
  ret = 1;
  goto out;

 out:
  if (uncompressed)
    free(uncompressed);
  if (compressed)
    free(compressed);
  return ret;
}
