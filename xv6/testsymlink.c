#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void) {
  int fd;
  int all_ok = 1;

  fd = open("target", O_CREATE | O_RDWR);
  if (fd < 0) {
    printf(1, "FAIL: cannot create target file\n");
    all_ok = 0;
    goto done;
  }
  write(fd, "hello, world", 12);
  close(fd);

  if (symlink("target", "link") < 0) {
    printf(1, "FAIL: cannot create symlink 'link'\n");
    all_ok = 0;
    goto done;
  }

  fd = open("link", O_RDONLY);
  if (fd < 0) {
    printf(1, "FAIL: cannot open symlink 'link'\n");
    all_ok = 0;
    goto done;
  }

  char buf[13];
  int n = read(fd, buf, 12);
  if (n != 12) {
    printf(1, "FAIL: read length through symlink = %d (expected 12)\n", n);
    close(fd);
    all_ok = 0;
    goto done;
  }
  buf[12] = '\0';
  if (strcmp(buf, "hello, world") != 0) {
    printf(1, "FAIL: symlink content mismatch: '%s'\n", buf);
    close(fd);
    all_ok = 0;
    goto done;
  }
  close(fd);
  printf(1, "PASS: read-through-symlink\n");

  
  if (symlink("cycle", "cycle") < 0) {
    printf(1, "FAIL: cannot create cycle symlink\n");
    all_ok = 0;
    goto done;
  }

  fd = open("cycle", O_RDONLY);
  if (fd >= 0) {
    printf(1, "FAIL: cycle not detected (open succeeded)\n");
    close(fd);
    all_ok = 0;
    goto done;
  }
  printf(1, "PASS: cycle-detection\n");

done:
  if (all_ok) {
    printf(1, "TESTS PASSED\n");
  } else {
    printf(1, "TESTS FAILED\n");
  }
  exit();
}