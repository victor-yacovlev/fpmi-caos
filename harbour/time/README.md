## Working with time

### Current time

Time in UNIX systems is defined as the number of seconds that have elapsed since January 1, 1970, and the time is considered as Greenwich mean time (GMT) without daylight saving time (DST).

32-bit systems should cease to exist normally on January 19, 2038, as there will be an overflow of the signed integer type to store the number of seconds.

The `time` function returns the number of seconds since the beginning of the epoch. The argument of the function (which can be passed `NULL`) is a pointer to the variable where you want to write the result.

In case when it is required an accuracy higher than 1 second, you can use the `gettimeofday` system call, which allows you to get the current time as a structure:
```
struct timeval {
  time_t      tv_sec;  // seconds
  suseconds_t tv_usec; // microseconds
};
```

In this case, despite the fact that the structure defines a field for microseconds, the real accuracy will be about 10-20 milliseconds for Linux.

Higher accuracy can be reached with the `clock_gettime` system call.