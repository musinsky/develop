// Author: Jan Musinsky
// 25/05/2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_SIZE   (100 * (EVENT_SIZE + 64))

int main(int argc, char **argv)
{
  if (argc != 2 || strcmp(argv[1], "--help") == 0) {
    fprintf(stderr, "Usage: %s monitordir\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int fd = inotify_init();
  if (fd == -1) {
    perror("inotify_init");
    exit(EXIT_FAILURE);
  }

  int wd = inotify_add_watch(fd, argv[1], IN_ALL_EVENTS);
  //int wd = inotify_add_watch(fd, argv[1], IN_CLOSE_WRITE | IN_MOVED_TO);
  if (wd == -1) {
    fprintf(stderr, "Cannot watch '%s'\n", argv[1]);
    perror("inotify_add_watch");
    exit(EXIT_FAILURE);

    // ls -l /proc/sys/fs/inotify/

    //    perror("inotify_add_watch");
    //    exit(EXIT_FAILURE);


    // fanotify(7) currently does not support for create, delete, and move events
    // https://github.com/proger/fanotify_watch
  }
  printf("Monitoring %s dir (fd = %d, wd = %d)\n", argv[1], fd, wd);

  ssize_t nread;
  char buffer[BUF_SIZE];
  struct inotify_event *event;
  const char *fext = ".dat";

  while (1) {
    printf("waiting for events ...\n");
    nread = read(fd, buffer, BUF_SIZE);
    if (nread <= 0) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nread; ) {
      event = (struct inotify_event *)&buffer[i];

      // events sent by the kernel
      if (event->mask & IN_Q_OVERFLOW)
        printf("IN_Q_OVERFLOW\n");
      if (event->mask & IN_IGNORED) {
        printf("IN_IGNORED\n");
        exit(EXIT_FAILURE);
      }

      // supported events
      if (event->len) {
        printf("len = %d\n", event->len);
        printf("name = %s\n", event->name);
        printf("mask = 0x%08X\n", event->mask);

        if (!(event->mask & IN_ISDIR)) { // only files
          if (event->mask & IN_CLOSE_WRITE) {
            printf("IN_CLOSE_WRITE = %s\n", event->name);
          }
          if (event->mask & IN_MOVED_TO) {
            printf("IN_MOVED_TO = %s\n", event->name);
          }

          const char *ext = strrchr(event->name, '.');
          if (ext && (strcmp(ext, fext) == 0)) printf("=> %s\n", event->name);
        }
      }
      i += EVENT_SIZE + event->len;
    }
  }

  inotify_rm_watch(fd, wd);
  close(fd);
  exit(EXIT_SUCCESS);
}
