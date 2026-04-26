#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void) {
  int fd;

  // Create a file
  fd = open("target", O_CREATE | O_RDWR);
  if (fd < 0) {
    printf(1, "Failed to create target file\n");
    exit();
  }
  write(fd, "hello, world", 12);
  close(fd);

  // Create a symbolic link
  if (symlink("target", "link") < 0) {
    printf(1, "Failed to create symlink\n");
    exit();
  }

  // Open the symbolic link
  fd = open("link", O_RDONLY);
  if (fd < 0) {
    printf(1, "Failed to open symlink\n");
    exit();
  }

  char buf[13];
  read(fd, buf, 12);
  buf[12] = '\0';
  printf(1, "Read from symlink: %s\n", buf);
  close(fd);

  // Test cycle detection
  // create a self-referential symlink to test cycle detection
  if (symlink("cycle", "cycle") < 0) {
    printf(1, "Failed to create cycle symlink\n");
    exit();
  }

  fd = open("cycle", O_RDONLY);
  if (fd >= 0) {
    printf(1, "Cycle detection failed\n");
    close(fd);
    exit();
  }

  printf(1, "All tests passed\n");
  exit();
}