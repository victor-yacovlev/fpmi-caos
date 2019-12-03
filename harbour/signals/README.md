# Signals. Part 1

## Introduction
A signal is a short message transmission mechanism (signal number), typically interrupting the process to which it was sent.

Signals can be sent to the process:
 * by the kernel, usually in case of a critical execution error;
 * by other process;
 * to itself.

Signal numbers start with 1. The value 0 has a special purpose (see below about `kill`). Some signal numbers correspond to POSIX-standard names and destinations, which are described in detail by `man 7 signal`.

When a signal is received, the process can:
 1. Ignore it. It is possible for all signals except `SIGSTOP` and `SIGKILL`.
 2. Process with a separate function. Except `SIGSTOP` and `SIGKILL`.
 3. Perform the default action specified by the POSIX standard signal assignment. Typically, this is a process shutdown.

By default, all signals except `SIGCHILD` (informing about the termination of the child process) and `SIGURG` (informing about the receiving of the TCP segment with priority data), lead to the termination of the process.

If a process was terminated with a signal rather than using the `exit` system call, it is considered to have an undefined return code. The parent process can monitor this situation using the `WIFSIGNALED` and `WTERMSIG`macros:

```
pid_t child = ...
...
int status;
waitpid(child, &status, 0);
if (WIFEXITED(status)) {
    // the child process was terminated via `exit`
    int code = WEXITSTATUS(status); // return code
}
if (WIFSIGNALED(status)) {
    // the child process was terminated by a signal
    int signum = WTERMSIG(status); // signal number
}
```

You can send a signal to any process using the `kill` command. By default, the `SIGTERM` signal is sent, but you can specify which signal to send as an option. In addition, some signals are sent by the terminal, for example Ctrl+C sends a `SIGINT` signal and Ctrl+\ sends a `SIGQUIT ` signal.


## User-defined signals

Initially, POSIX reserved two signal numbers that could be used at the discretion of the user: `SIGUSR1` and `SIGUSR2`.

In addition, Linux provides a range of signals with numbers from `SIGRTMIN` to `SIGRTMAX`, which can be used at the discretion of the user.

The default action for all "user-defined signals" signals is to shut down the process.


## Sending signals programmatically

### System call `kill`

Similar to the command of the same name, `kill` is intended to send a signal to any process.

```
int kill(pid_t pid, int signum); // returns 0 or -1 if error
```

You can only send signals to processes that belong to the same user as the user on which the 'kill' system call is executed. The exception is the `root` user, who can do everything. If you try to send a signal to another user's process  `kill` will return `-1`.

The process number may be less than `1` in cases:
 * `0` - send a signal to all processes of the current process group;
 * `-1` - send a signal to all user processes (use with caution!);
 * negative value `-PID` - send signal to all processes of `PID ' group.

 
The signal number can be set to `0`, in which case no signal will be sent, and `kill` will return `0` if the process (group) with the specified `pid` exists and there are rights to send signals.

### `Raise` and `abort` functions

The `raise` function is designed to send a process signal to itself. The standard library function `abort` sends itself a `SIGABRT` signal, and is often used to generate exceptions that can be diagnosed at runtime, such as by the `assert` function.

### System call `alarm`

The system call `alarm` starts a timer, after which the process will send itself a signal `SIGALRM`.

```
unsigned int alarm(unsigned int seconds);
```

You can cancel the previously set timer by calling `alarm` with the parameter `0`. The return value is the number of seconds of the previous timer set.

## Обработка сигналов

Сигналы, которые можно перехватить, то есть все, кроме `SIGSTOP` и `SIGKILL`, можно обработать программным способом. Для этого необходимо зарегистрировать функцию-обработчик сигнала.

### Системный вызов `signal`
```
#include <signal.h>

// Этот тип определен только в Linux!
typedef void (*sighandler_t)(int);

sighandler_t signal(int signum, sighandler_t handler); // для Linux
void (*signal(int signum, void (*func)(int))) (int); // по стандарту POSIX
```

Системный вызов `signal` предназначен для того, чтобы зарегистрировать функцию в качестве обработчика определенного сигнала. Первым аргументом является номер сигнала, вторым - указатель на функцию, которая принимает единственный аргумент - номер пришедшего сигнала (т.е. одну функцию можно использовать сразу для нескольких сигналов), и ничего не возвращает.

Два специальных значения функции-обработчика `SIG_DFL` и `SIG_IGN` предназанчены для указания обработчика по умолчанию (т.е. отмены ранее зарегистрированного обработчика) и установки игнорирования сигнала.

Системный вызов `signal` возвращает указатель на ранее установленный обработчик.

### System-V v.s. BSD

В стандартах, родоначальниками которых были UNIX System-V и BSD UNIX, используется различное поведение обработчика сигнала, зарегистрированного с помощью `signal`. При определении одного из макросов препроцессора: `_BSD_SOURCE`, `_GNU_SOURCE` или `_DEFAULT_SOURCE` (что подразумевается опцией компиляции `-std=gnu99` или `-std=gnu11`), используется семантика BSD; в противном случае (`-std=c99` или `-std=c11`) - семантика System-V.

Отличия BSD от System-V:
 * В System-V обработчик сигнала выполяется один раз, после чего сбрасывается на обработчик по умолчанию, а в BSD - остается неизменным.
 * В BSD обработчик сигнала не будет вызван, если в это время уже выполняется обработчик того же самого сигнала, а в System-V это возможно.
 * В System-V блокирующие системные вызовы (например, `read`) завершают свою работу при поступлении сигнала, а в BSD большинство блокирующих системных вызовов возобновляют свою работу после того, как обработчик сигнала заверщает свою работу.

По этой причине, системный вызов `signal` считается устаревшим, и в новом коде использовать его запрещено, за исключением двух ситуаций:

```
signal(signum, SIG_DFL); // сброс на обработчик по умолчанию
signal(signum, SIG_IGN); // игнорирование сигнала
```

### Системный вызов `sigaction`

Системный вызов `sigaction`, в отличии от `signal`, в качестве второго аргумента принимает не указатель на функцию, а указатель на структуру `struct sigaction`, с которой, помимо указателя на функцию, хранится дополнительная информация, описывающая семантику обработки сигнала. Поведение обработчиков, зарегистрированных с помощью `sigaction`, не зависит от операционной системы.

```
int sigaction(int signum,
              const struct sigaction *restrict act,
              struct sigaction *oldact);
```

Третьим аргументов является указатель на структуру, описывающую обработчик, который был зарегистрирован для этого. Если эта информация не нужна, то можно передать значение `NULL`.

Основные поля структуры `struct sigaction`:
 * `sa_handler` - указатель на функцию-обработчик с одним аргументом типа `int`, могут быть использованы значения `SIG_DFL` и `SIG_IGN`;
 * `sa_flags` - набор флагов, опиывающих поведение обработчика;
 * `sa_sigaction` - указатель на функцию-обработчик с тремя параметрами, а не одним (используется, если в флагах присутствует `SA_SIGINFO`).

Некоторые флаги, которые можно передавать в `sa_flags`:
 * `SA_RESTART` - продолжать выполнение прерванных системных вызовов (семантика BSD) после завершения обработки сигнала. По умолчанию (если флаг отсутствует) используется семантика System-V.
 * `SA_SIGINFO` - вместо функции из `sa_handler` нужно использовать функцию с тремя параметрами `int signum, siginfo_t *info, void *context`, которой помимо номера сигнала, передается дополнительная информация (например PID отправителя) и пользовательский контекст.
 * `SA_RESETHAND` - после выполнения обработчика сбросить на обработчик по умолчанию (семантика System-V). По умолчанию (если флаг отсутствует) используется семантика BSD.
 * `SA_NODEFER` - при повторном приходе сигнала во время выполени обработчика он будет обработан немедленно (семантика System-V). По умолчанию (если флаг отсутствует) используется семантика BSD.

## Асинхронность обработки сигналов

Сигнал может прийти процессу в любой момент времени. При этом, выполнение текущего кода будет прервано, и будет запущен обработчик сигнала.

Таким образом, возникает проблема "гонки данных", которая часто встречается в многопоточном программировании.

Существует безопасный целочисленный (32-разрядный) тип данных, для которого гарантируется атомарность чтения/записи при переключении между выполнением основной программы и выполнением обработчика сигнала: `sig_atomic_t`, объявленный в `<signal.h>`.

Кроме того, во время выполнения обработчика сигналов запрещено использовать не потоко-безопасные функции (большинство функций стандартной библиотеки). В то же время, использование системных вызовов - безопасно.
