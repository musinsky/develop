* https://www.gnu.org/software/libc/manual/html_node/Setting-an-Alarm.html
* https://www.gnu.org/software/libc/manual/html_node/Handler-Returns.html
* http://www.linuxprogrammingblog.com/all-about-linux-signals


# NOTES
- avoid use signal(), use sigaction(2) instead
- signal() can only attach ONE function to each signal !!!


- alarm() and setitimer(2) share the same timer; calls to one will interfere with use of the other
- POSIX.1-2008 marks getitimer() and setitimer() obsolete, recommending the use of the POSIX timers API (timer_gettime(2), timer_settime(2), etc.) instead

---
* https://stackoverflow.com/questions/2150291/how-do-i-measure-a-time-interval-in-c

- POSIX.1-2008 marks gettimeofday() as obsolete, recommending the use of clock_gettime(2) instead
