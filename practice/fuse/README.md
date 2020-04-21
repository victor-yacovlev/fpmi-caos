# Реализация файловых систем без написания модулей ядра

## Общие сведения

Файловые системы обычно реализуются в виде модулей ядра, которые работают в адресном пространстве ядра. 

Монтирование осуществляется командой [`mount(8)`](http://man7.org/linux/man-pages/man8/mount.8.html), которой необходимо указать:

* *точку монтирования* - каталог в виртуальной файловой системе, в котором будет доступно содержимое смонтированной файловой системы;
* *тип файловой системы* - один из поддерживаемых типов: `ext2`, `vfat` и др. Если не указать тип файловой системы, то ядро попытается автоматически определить её тип, но ему обычно это не всегда удаётся;
* *устройство для монтирования* - как правило, блочное, устройство для монтирования реальных устройств, либо URI для сетевых ресурсов, либо имя файла для монтирования образа.

Вызов команды `mount` без параметров отображает список примонтированных файловых систем. 

Постоянные файловые системы, которые монтируются при загрузке системы, перечислены в файле `/etc/fstab`, формат которого описан в [`fstab(5)`](http://man7.org/linux/man-pages/man5/fstab.5.html). Если точка монтирования указана в этом файле, то для монтирования файловой системы достаточно указать команде `mount` только точку монтирования.

Некоторые типы файловых систем реализованы в виде сервисов, которые работают в пространстве пользователя, как обычные процессы. Ядро взаимодействует с ними, используя файл символьного устройства `/dev/fuse`. Когда ядру необходимо обслужить запрос к виртуальной файловой системе, то в случае, если точка монтирования содержит файловую систему FUSE, ядро формирует запрос в специальном формате, и отправляет его тому процессу, который открыл файл `/dev/fuse` и зарегистрировал открытый файловый дескриптор в качестве параметра системного вызова [`mount(2)`](http://man7.org/linux/man-pages/man2/mount.2.html). После этого процесс обязан сформировать ответ, который будет обработан модулем ядра `fuse.ko` и ядро выполнит запрошенную файловую операцию.

Подсистема FUSE реализована в Linux и FreeBSD. 

## Пример реализации - файловая система SSH

Протокол SSH предназначен для терминального доступа к любой UNIX-системе, на которой запущен сервис `sshd`. Авторизация осуществляется через пароль, в этом случае нужно будет его ввести после запуска команды `ssh`, либо с использованием асимметричных RSA-ключей.

Использование ключей обычно является более безопасным (при условии, что полностью запрещена авторизация по паролю), поскольку случайно подобранный ключ намного сложнее подобрать по словарю, чем пароль. Для создания пары ключей используется команда `ssh-keygen`, которая создает пару файлов `~/.ssh/id_rsa` и `~/.ssh/id_rsa.pub` , первый из которых является приватным ключом, а второй - публичным. Содержимое публичного ключа можно добавить отдельной строкой в текстовый файл `~/.ssh/authorized_keys` на целевом хосте, и после этого можно будет подключаться без ввода пароля. Для копирования ключа на удаленный сервер в большинстве дистрибутивов предусмотрен скрипт `ssh-copy-id`.

С помощью ssh можно не только интерактивно взаимодействовать с удаленным хостом, но и выполнять отдельные команды, если указывать из последними аргументами.

**Пример:** создание каталога на удаленном хосте и копирование в него файла с локального компьютера.

```bash
# ssh user  @ host    "command to execute"
> ssh victor@10.0.2.4 "mkdir ~/some_dir"

# get contents   pipe                    write contents to file
> cat /bin/bash   |  ssh victor@10.0.2.4 "cat >~/some_dir/bash" 

# set file attributes
> ssh victor@10.0.2.4 "chmod a+x ~/some_dir/bash"

# ensure file has been copied
> ssh ssh victor@10.0.2.4 "ls -l ~/some_dir/"
total 1024
-rwxr-xr-x  1 victor  victor  1012552 Apr 21 10:17 bash
```

Таким образом, используя ssh в сочетании со стандартными командами POSIX, можно реализовать произвольные файловые операции над удаленной файловой системой. Этот подход реализован в реализации файловой системы [sshfs](https://github.com/libfuse/sshfs).

```bash
# local directory for remote contents
> mkdir remote_host

# mount   user@host    :path    local mount point
> sshfs victor@10.0.2.4:/       remote_host
```

При этом, реализация `sshfs` использует только `fork+exec` для запуска команды `ssh`, и работает в адресном пространстве пользователя, а не ядра, и использует обычные сторонние библиотеки:

```bash
> ldd /usr/bin/sshfs
        linux-vdso.so.1 (0x00007ffff1985000)
        libfuse.so.2 => /lib64/libfuse.so.2 (0x00007f33e781e000)
        libgthread-2.0.so.0 => /usr/lib64/libgthread-2.0.so.0 (0x00007f33e761c000)
        libglib-2.0.so.0 => /usr/lib64/libglib-2.0.so.0 (0x00007f33e7305000)
        libpthread.so.0 => /lib64/libpthread.so.0 (0x00007f33e70e7000)
        libc.so.6 => /lib64/libc.so.6 (0x00007f33e6d2d000)
        libdl.so.2 => /lib64/libdl.so.2 (0x00007f33e6b29000)
        libpcre.so.1 => /usr/lib64/libpcre.so.1 (0x00007f33e689c000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f33e7c71000)
```

## Библиотека libfuse

### Реализация демона FUSE

Библиотека [libfuse](https://github.com/libfuse/libfuse) реализует функциональность взаимодействия с модулем ядра `fuse.ko` через файловый дескриптор файла `/dev/fuse`. Поскольку проект существует достаточно давно, программный интерфейс (API) библиотеки претерпел множество изменений, и перед включением заголовочного файла необходимо указать версию программного интерфейса, который будет использован:

```c
#define FUSE_USE_VERSION 30   // API version 3.0
#include <fuse.h>
```

В дальнейшем значение макроса `FUSE_USE_VERSION` будет использовано препроцессором при обработке файла `fuse.h` для условной подстановки соответствующих определенной версии сигнатур функций. До версии 3.0 изменений накопилось очень много, поэтому используется отдельная библиотека `libfuse3.so` вместо `libfuse.so`, в дальнейшем мы будем использовать именно её. 

Для FUSE не реализован пакет CMake, но существует описание в формате `pkg-config`, которое можно использовать из CMake:

```cmake
find_package(PkgConfig REQUIRED)
pkg_check_modules(FUSE REQUIRED fuse3)

link_libraries(${FUSE_LIBRARIES})         # -lfuse3 -lpthread
include_directories(${FUSE_INCLUDE_DIRS}) # -I/usr/include/fuse3
compile_options(${FUSE_CFLAGS_OTHER})     # empty since fuse 3.0
```

Реализация файловой системы - это программа-демон, которая ожидает запросы от ядра и обслуживает их. Реализация тривиальной программы:

```c
static struct fuse_operations operations = {
    // pointers to callback functions
};

int main(int argc, char *argv[]) {
    // arguments to be preprocessed before passing to /sbin/mount.fuse3
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    
    // run daemon
    int ret = fuse_main(
        args.argc, args.argv, // arguments to be passed to /sbin/mount.fuse3
        &operations,          // pointer to callback functions
        NULL                  // optional pointer to user-defined data
    );
    return ret;
}
```

Демон использует стандартный для монтирования набор аргументов командной строки, аналогичный команде [`mount.fuse(8)`](http://man7.org/linux/man-pages/man8/mount.fuse.8.html). Как минимум, требуется один позиционный аргумент - это точка монтирования. После выполнения операции монтирования, демон продолжает работать в фоновом режиме, используя `fork`. Для работы в текущем процессе (foreground) используется опция `-f`, что бывает полезно для отладки. Опция `-u` означает операцию отключения ранее зарегистрированной точки монтирования.

Набор операций, который используется при обработке запросов от ядра может быть не полным, то есть не покрывать всю функциональность файловой системы, или даже пустым. В этом случае, при попытке обращения к файловой системе, возникнет ошибка. Так, для примера выше, команда `ls` завершит работу с ошибкой:

```bash
# start empty FUSE implementation
> ./my_filesystem work_dir

# try to get filesystem contents
> ls work_dir
ls: cannot access 'work_dir': Function not implemented

# umount filesystem and stop the daemon
> fusermount3 -u work_dir
```

### Реализация функциональности

Реализация функциональности файловой системы определяется указателями на соответствующий функции в структуре `struct fuse_operations`, причем для большинства полей имена совпадают с именами соответствующих системных вызовов (за исключением системного вызова  `stat`, поле которого называется `getattr`). Для большинства из функций возвращаемым значением является целое число: `0` в случае успеха, и отрицательное значение в случае ошибки. Значение модуля кода ошибки соответствует ожидаемому коду ошибки, который будет записан в `errno` после выполнения соответствующего системного вызова. Так, ошибке "файл не найден" соответствует возвращаемое значение `-ENOENT`, где значение константы `ENOENT` определено в заголовочном файле `<errno.h>` .

**Полное описание** callback-функций доступно по ссылке: [https://libfuse.github.io/doxygen/structfuse__operations.html](https://libfuse.github.io/doxygen/structfuse__operations.html).

#### Получение списка файлов

Рассмотрим тривиальную файловую систему, которая содержит ровно два файла: `a.txt` и `b.txt`  с одинаковым содержимым. Как минимум, необходимо иметь возможность узнать содержимое файловой системы, реализовав для этого [`readdir(2)`](http://man7.org/linux/man-pages/man2/readdir.2.html) для чтения содержимого каталога, и [`stat(2)`](http://man7.org/linux/man-pages/man2/stat.2.html) для получения атрибутов файлов, включая атрибуты самого корневого каталога, иначе будет невозможно узнать о том, что он действительно является каталогом, и тем самым - получить его содержимое.

```c
// contents to be accessed by reading files
static const char DummyData[] = "Hello, World!\n";

// callback function to be called after 'stat' system call
int my_stat(const char *path, struct stat *st, struct fuse_file_info *fi)
{
    // check if accessing root directory
    if (0==strcmp("/", path)) {
        st->st_mode = 0555 | S_IFDIR;  // file type - dir, access read only
        st->st_nlink = 2;              // at least 2 links: '.' and parent
        return 0;                      // success!
    }
    if (0!=strcmp("/a.txt", path) && 0!=strcmp("/b.txt", path)) {
        return -ENOENT; // error: we have no files other than a.txt and b.txt
    }

    st->st_mode = S_IFREG | 0444;    // file type - regular, access read only
    st->st_nlink = 1;                // one link to file
    st->st_size = sizeof(DummyData); // bytes available
    return 0;                        // success!
}

// callback function to be called after 'readdir' system call
int my_readdir(const char *path, void *out, fuse_fill_dir_t filler, off_t off,
               struct fuse_file_info *fi, enum fuse_readdir_flags flags)
{
    if (0 != strcmp(path, "/")) {
        return -ENOENT;  // we do not have subdirectories
    }

    // two mandatory entries: the directory itself and its parent
    filler(out, ".", NULL, 0, 0);
    filler(out, "..", NULL, 0, 0);

    // directory contents
    filler(out, "a.txt", NULL, 0, 0);
    filler(out, "b.txt", NULL, 0, 0);

    return 0;  // success
}

struct fuse_operations operations = {
    .readdir = my_readdir,   // callback function pointer for 'readdir'
    .getattr = my_stat,      // callback function pointer for 'stat'
};
```

Теперь реализация файловой системы позволяет получить содержимое каталога:

```bash
> ./my_filesystem work_dir

> ls -l work_dir
total 0
-r--r--r-- 1 root root 15 Jan  1  1970 a.txt
-r--r--r-- 1 root root 15 Jan  1  1970 b.txt

# try to get file contents - still not implemented 'open' and 'read'
> cat work_dir/a.txt
cat: work_dir/a.txt: Function not implemented

# umount filesystem and stop the daemon
> fusermount3 -u work_dir
```

Обратите внимание, что даты создания файлов - 1 января 1970 года, - это соответствует значению 0 для формата времени в UNIX, а владелец файла и группа - пользователь и группа `root`, численные значения `uid` и `gid` которых равны 0. Эти поля могут быть также заполнены при реализации `stat`.

Кроме того, утилита `ls` отображает `total 0`, поскольку это значение в выводе является количеством занятых блоков на диске, и эта информация отсутствует в атрибутах файлов.

#### Чтение данных

Для того, чтобы прочитать данные из файла, его нужно, как минимум, успешно открыть, и кроме того, реализовать функцию чтения, которая соответствует поведению системного вызова `read`.

```c
// callback function to be called after 'open' system call
int my_open(const char *path, struct fuse_file_info *fi)
{    
    if (0!=strcmp("/a.txt", path) && 0!=strcmp("/b.txt", path)) {
        return -ENOENT;   // we have only two files in out filesystem
    }
    if (O_RDONLY != (fi->flags & O_ACCMODE)) {
        return -EACCES;   // file system is read-only, so can't write
    }
    return 0;  // success!
}

// contents of file
static const char DummyData[] = "Hello, World!\n";

// callback function to be called after 'read' system call
int my_read(const char *path, char *out, size_t size, off_t off,
         struct fuse_file_info *fi)
{
    // 'read' might be called with arbitary arguments, so check them
    if (off > sizeof(DummyData))
        return 0;
    // reading might be called within some non-zero offset
    if (off+size > sizeof(DummyData))
        size = sizeof(DummyData) - off;
    const void *data = DummyData + off;
    // copy contents into the buffer to be filled by 'read' system call
    memcpy(out, data, size);
    // return value is bytes count (0 or positive) or an error (negative)
    return size;
}

// register functions as callbacks
struct fuse_operations operations = {
    .readdir = my_readdir,
    .getattr = my_stat,
    .open    = my_open,
    .read    = my_read,
};
```

Теперь можно прочитать содержимое файла:

```bash
> ./my_filesystem work_dir

# get file contents - OK
> cat work_dir/a.txt
Hello, World!

# try to create new file - still not implemented
> touch work_dir/new_file.txt
touch: cannot touch 'work_dir/new_file.txt': Function not implemented

# umount filesystem and stop the daemon
> fusermount3 -u work_dir
```

### Опции монтирования

У монтирования файловых систем могут быть опции, например точка монтирования и файл с образом, либо устройство, либо любой другой источник данных для файловой системы. Некоторые опции являются обязательными для всех FUSE-систем, например указание точки монтирования, а часть из них - быть специфичными для реализации определенных файловых систем.

Функция, реализующая работу FUSE-демона `fuse_main` принимает два аргумента: количество опций `argc` и массив строк `argv`, по аналогии с функцией `main`. Если какие-то опции не распознаны `fuse_main`, либо их не достаточно для монтирования файловой системы, то эта функция завершается с ошибкой.

Для выделения, специфичных для конкретной файловой системы, опций используется модифицируемый список опций `fuse_args`, который инициализируется макросом `FUSE_ARGS_INIT(argc, argv)`.

Для извлечения специфичных опций по некоторым шаблонам используется функция `fuse_opt_parse`, которая принимает описания опций, которые необходимо распознать, выполняет разбор аргументов командной строки, и извлекает обработанные опции из массива `argv` структуры `fuse_args`, чтобы они потом не попали в `fuse_main`.

Описанием одной опции является структура  `fuse_opt`, которая содержит:

* текстовую строку, содержащую шаблон аргумента, и возможно, форматную строку для значения, которое необходимо извлечь;
* смещение в байтах относительно начала структуры, которую заполняет функция `fuse_opt_parse`, если параметр встретился среди аргументов командной строки;
* целочисленное значение, которое будет записано в структуру; игнорируется в случае, если шаблон содержит формат значения, который нужно извлечь.

Для разбора опций необходимо определить массив из структур `fuse_opt`, где последний элемент, по аналогии со строками, заполнен нулями, и вызвать `fuse_opt_parse`, передав указатель на структуру с опциями, которую необходимо заполнить по результатам их разбора.

```c
int main(int argc, char *argv[])
{
    // initialize modificable array {argc, argv}
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

    // struct to be filled by options parsing
    typedef struct {
        char *src;
        int  help;
    } my_options_t;

    my_options_t my_options;
    memset(&my_options, 0, sizeof(my_options));

    // options specifications
    struct fuse_opt opt_specs[] = {
        // pattern: match --src then string
        // the string value to be written to my_options_t.src
        { "--src %s", offsetof(my_options_t, src)   , 0     },
        // pattern: match --help
        // if found, 'true' value to be written to my_options_t.help
        { "--help"  , offsetof(my_options_t, help)  , true  },
        // end-of-array: all zeroes value
        { NULL      , 0                             , 0     }
    };
	
    // parse command line arguments, store matched by 'opt_specs'
    // options to 'my_options' value and remove them from {argc, argv}
    fuse_opt_parse(&args, &my_options, opt_specs, NULL);

    if (my_options.help) {
        show_help_and_exit();
    }

    if (my_options.src) {
        open_filesystem(my_options.src);
    }

    // pass rest options but excluding --src and --help to mount.fuse3 
    int ret = fuse_main(args.argc, args.argv, &operations, NULL);

    return ret;
}
```

