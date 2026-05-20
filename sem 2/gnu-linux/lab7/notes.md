# lab7 — Модули ядра Linux

## Что такое `модуль ядра`?

Модуль ядра (Loadable Kernel Module, LKM) — это объектный файл (`.ko`), который может быть **динамически загружен в адресное пространство ядра и выгружен из него без перезагрузки системы**.

- Модули расширяют функциональные возможности ядра без необходимости перезагрузки системы
- Одна из разновидностей модулей ядра — **драйверы устройств** — позволяют ядру взаимодействовать с аппаратурой компьютера

> При отсутствии поддержки модулей нам пришлось бы писать монолитные ядра[^1] и добавлять новые возможности прямо в ядро.

**Чем модуль отличается от обычной программы:**

| Аспект | Обычная программа | Модуль ядра |
|---|---|---|
| Пространство выполнения | userspace | kernelspace |
| Точка входа | `main()` | `init_module()` / `module_init()` |
| Доступные функции | libc (`printf`, `malloc`, …) | только API ядра (`printk`, `kmalloc`, …) |
| Ошибка указателя | segfault → падает процесс | kernel panic / зависание системы |
| Загрузка | `execve()` | `insmod` / `modprobe` |

## Зачем нужны модули ядра?

- **Драйверы устройств**: клавиатуры, сетевые карты, USB-устройства подключаются и отключаются динамически — без модулей пришлось бы перекомпилировать ядро каждый раз
- **Файловые системы**: ext4, btrfs, ntfs3 — каждая реализована как модуль и подгружается только при необходимости
- **Сетевые протоколы и фильтры** (netfilter / iptables)
- **Отладка и трассировка**: модули могут вставлять хуки в вызовы ядра в runtime
- Уменьшение размера базового ядра: в памяти находится только то, что реально используется

## Как модули попадают в ядро?

**Схема загрузки через `modprobe`:**

```
Запрос модуля (имя или alias)
        │
        ▼
/etc/modprobe.d/  ←── конфигурация, параметры, алиасы
        │
        ▼
/lib/modules/$(uname -r)/modules.dep  ←── дерево зависимостей (строится depmod -a)
        │
        ▼
insmod для каждой зависимости → insmod для запрошенного модуля
        │
        ▼
Ядро вызывает init_module() модуля
```

### Ключевые утилиты

- **`lsmod`**: Просмотр списка загруженных модулей. Информацию берёт напрямую из виртуального файла `/proc/modules`. Показывает имя, размер в памяти и количество пользователей (use count).
- **`kmod`**: Современный набор утилит для управления модулями. В актуальных дистрибутивах команды `lsmod`, `insmod`, `modprobe` и др. являются симлинками на один бинарник `/usr/bin/kmod`.
- **`insmod <путь_к_файлу>`**: Низкоуровневая утилита для вставки модуля. Требует **точный путь** к `.ko` файлу и **не умеет** разрешать зависимости. Делает системный вызов `finit_module()` / `init_module()`.
- **`rmmod <имя_модуля>`**: Удаление модуля из ядра. Требует, чтобы use count был равен 0 (никто не использует). Ядро вызывает функцию завершения (`cleanup_module` / `module_exit`).
- **`modprobe <имя_модуля>`**: «Умная» утилита для управления модулями. Основная логика:
    1. **Поиск**: Ищет модуль по имени или псевдониму (alias) в стандартном каталоге `/lib/modules/$(uname -r)/`.
    2. **Зависимости**: Сверяется с файлом `/lib/modules/$(uname -r)/modules.dep`. Если модулю «А» для работы нужен модуль «Б», `modprobe` сначала автоматически загрузит «Б».
    3. **Конфигурация**: Проверяет настройки в `/etc/modprobe.d/`. Там могут быть описаны параметры модуля или алиасы (например, привязка идентификатора устройства к конкретному драйверу).
    4. **Загрузка**: В конечном итоге вызывает `insmod` для каждого необходимого компонента.
- **`depmod -a`**: Анализирует все модули в `/lib/modules/$(uname -r)/` и заново строит дерево зависимостей, обновляя файл `modules.dep`. Обычно вызывается автоматически при установке нового ядра или драйверов.
- **`modinfo <модуль>`**: Выводит метаданные модуля: лицензию, автора, параметры, зависимости — из ELF-секций `.modinfo`.
- **`dmesg`**: Читает кольцевой буфер сообщений ядра (kernel ring buffer), куда пишет `printk`.

---

## Часть 1. Простейший модуль ядра — `hello-1.c`

```c
#include <linux/module.h>   /* Необходим для любого модуля ядра */
#include <linux/kernel.h>   /* KERN_ALERT и другие уровни логирования */

int init_module(void)
{
    printk(KERN_ALERT "Hello world 1.\n");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_ALERT "Goodbye world 1.\n");
}

MODULE_LICENSE("GPL");
```

### Что здесь происходит на уровне ядра

- Ядро загружает `.ko` как ELF-объект в своё адресное пространство
- Вызывает `init_module()` — модуль регистрирует себя (в данном примере просто печатает)
- При `rmmod` ядро вызывает `cleanup_module()` и выгружает код из памяти

### Ответы на вопросы

**Что за вход/выход? Куда и откуда?**
`init_module()` — функция инициализации, вызывается ядром в момент загрузки модуля (`insmod`). `cleanup_module()` — функция завершения, вызывается ядром при выгрузке модуля (`rmmod`). Это не обычные функции — их адреса записываются в специальные ELF-секции `.init.text` и `.exit.text`, откуда ядро их и вызывает.

**Почему нужна лицензия? (`MODULE_LICENSE("GPL")`)**
Начиная с ядра 2.4 введён механизм "tainted kernel" (загрязнённое ядро). Если модуль загружается без лицензии или с проприетарной — ядро выставляет флаг tainted. Это означает: разработчики ядра могут отказать в помощи при отладке такой системы. Начиная с ядра ~5.x `MODULE_LICENSE()` стал **обязательным** (сборка завершается ошибкой без него). GPL-лицензия также открывает доступ к ряду экспортированных символов ядра, доступных только GPL-модулям (`EXPORT_SYMBOL_GPL`).

**Почему вход возвращает значение, а выход — нет?**
`init_module()` возвращает `int` — код ошибки. Это стандартное соглашение Linux: `0` = успех, отрицательное значение = код ошибки (например, `-ENOMEM`, `-EINVAL`). Если вернуть ненулевое значение, ядро считает инициализацию неудавшейся и **не загружает модуль**. `cleanup_module()` возвращает `void` — выгрузка модуля не может быть «отменена» с точки зрения ядра, нет смысла возвращать статус.

**Что если вход вернёт не 0?**
Модуль не загрузится в ядро (init не сработает, вернётся код ошибки). Однако функция `init_module()` всё равно **выполнится до конца** (включая `printk` до `return`), но ядро проигнорирует выделенные ресурсы — поэтому важно освобождать всё, что успело выделиться до точки ошибки, прямо внутри `init_module`.

**Что такое `__init` и `__exit`?**
Макросы-атрибуты, объявленные в `<linux/init.h>`:
- `__init` помещает функцию в специальную секцию `.init.text`. После завершения инициализации всех **встроенных** (`obj-y`) модулей ядро **освобождает эту память** — отсюда строка `Freeing unused kernel memory: NNNk freed` при загрузке. Для загружаемых модулей (`.ko`) эффекта на память нет, но использовать всё равно нужно — это явный сигнал «функция нужна только при инициализации».
- `__exit` — аналогично для функции завершения. Для встроенных модулей она вообще **не компилируется** (встроенный модуль нельзя выгрузить), для загружаемых — компилируется как обычно.
- `__initdata` — то же самое, но для переменных: данные освобождаются после инициализации.

---

## Часть 2. `module_init()` и `module_exit()` — `hello-2.c`

```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>     /* Здесь определены module_init() и module_exit() */

static int __init hello_2_init(void)
{
    printk(KERN_ALERT "Hello, world 2\n");
    return 0;
}

static void __exit hello_2_exit(void)
{
    printk(KERN_ALERT "Goodbye, world 2\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);

MODULE_LICENSE("GPL");
```

### Зачем это нужно

Начиная с ядра 2.3.13 имена `init_module` и `cleanup_module` перестали быть обязательными. Макросы `module_init(fn)` и `module_exit(fn)` просто записывают указатель на вашу функцию в нужную ELF-секцию (`.init.text` / `.exit.text`), заменяя стандартные имена.

**Преимущества:**
- Можно давать функциям осмысленные имена
- Ключевое слово `static` — функция не экспортируется в глобальное пространство имён ядра (нет конфликтов с другими модулями)
- Явное указание `__init` / `__exit` рядом с `static` — читаемо и правильно

**Важно:** функции должны быть объявлены **выше** вызова `module_init()`/`module_exit()`, иначе — ошибка компиляции.

---

## Часть 3. `__init`, `__initdata`, `__exit` — `hello-3.c`

```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int hello3_data __initdata = 3;  /* переменная только для инициализации */

static int __init hello_3_init(void)
{
    printk(KERN_ALERT "Hello, world %d\n", hello3_data);
    return 0;
}

static void __exit hello_3_exit(void)
{
    printk(KERN_ALERT "Goodbye, world 3\n");
}

module_init(hello_3_init);
module_exit(hello_3_exit);
MODULE_LICENSE("GPL");
```

### Что нового

`__initdata` — атрибут для переменных. Переменная `hello3_data` попадает в секцию `.init.data`. Для встроенных модулей после инициализации ядро освобождает эту память. Смысл: не держать в памяти данные, которые нужны только на старте системы.

Форматная строка `%d` в `printk` работает так же, как в `printf` — это намеренное сходство, но реализация совершенно другая (нет libc).

---

## Часть 4. Лицензирование и документирование — `hello-4.c`

```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_AUTHOR "Peter Jay Salzman <p@dirac.org>"
#define DRIVER_DESC   "A sample driver"

static int __init init_hello_4(void)
{
    printk(KERN_ALERT "Hello, world 4\n");
    return 0;
}

static void __exit cleanup_hello_4(void)
{
    printk(KERN_ALERT "Goodbye, world 4\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");
```

### Макросы описания модуля

Все эти макросы создают записи в ELF-секции `.modinfo` внутри `.ko` файла. Ядро их **не использует** — они нужны для утилит userspace:

| Макрос | Назначение | Просмотр |
|---|---|---|
| `MODULE_LICENSE("GPL")` | Тип лицензии | `modinfo`, контроль tainted |
| `MODULE_AUTHOR(...)` | Автор модуля | `modinfo -F author` |
| `MODULE_DESCRIPTION(...)` | Описание | `modinfo -F description` |
| `MODULE_SUPPORTED_DEVICE(...)` | Описание устройств | `modinfo` |
| `MODULE_VERSION(...)` | Версия | `modinfo -F version` |

Посмотреть можно так:
```bash
modinfo ./hello-4.ko
# или
objdump -s -j .modinfo hello-4.ko
```

**Допустимые строки лицензий:**

| Строка | Значение |
|---|---|
| `"GPL"` | GNU GPL v2 или выше |
| `"GPL v2"` | Строго GPL v2 |
| `"Dual BSD/GPL"` | BSD или GPL |
| `"Dual MIT/GPL"` | MIT или GPL |
| `"Proprietary"` | Проприетарный (tainted!) |

---

## Часть 5. Параметры командной строки — `hello-5.c`

```c
#include <linux/module.h>
#include <linux/moduleparam.h>  /* module_param() */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>         /* S_IRUSR и другие флаги прав доступа */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peter Jay Salzman");

static short int myshort = 1;
static int       myint   = 420;
static long int  mylong  = 9999;
static char     *mystring = "blah";

/*
 * module_param(имя_переменной, тип, права_доступа_sysfs)
 *
 * Права доступа: если ненулевые — файл параметра появится в
 * /sys/module/<имя_модуля>/parameters/<имя_параметра>
 * и его можно будет читать/писать прямо из userspace без перезагрузки.
 */
module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");

module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");

module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");

module_param(mystring, charp, 0000);   /* charp = char pointer, insmod выделит память сам */
MODULE_PARM_DESC(mystring, "A character string");

static int __init hello_5_init(void)
{
    printk(KERN_ALERT "Hello, world 5\n=============\n");
    printk(KERN_ALERT "myshort  = %hd\n", myshort);
    printk(KERN_ALERT "myint    = %d\n",  myint);
    printk(KERN_ALERT "mylong   = %ld\n", mylong);
    printk(KERN_ALERT "mystring = %s\n",  mystring);
    return 0;
}

static void __exit hello_5_exit(void)
{
    printk(KERN_ALERT "Goodbye, world 5\n");
}

module_init(hello_5_init);
module_exit(hello_5_exit);
```

### Как передаются параметры

```bash
# При загрузке через insmod:
sudo insmod hello-5.ko mystring="bebop" myshort=255

# При загрузке через modprobe (параметры можно прописать в /etc/modprobe.d/):
sudo modprobe hello_5 myint=100
```

### Как работает `module_param()` на уровне ядра

`module_param(имя, тип, права)` — это макрос, который:
1. Создаёт запись в специальной секции ELF, описывающую: имя параметра, тип, адрес переменной
2. При `insmod` ядро читает строку аргументов, парсит `имя=значение`, находит нужную запись и **копирует значение в переменную до вызова `init_module()`**
3. Если `права != 0` — создаёт файл в sysfs: `/sys/module/<модуль>/parameters/<имя>`, через который параметр можно менять в runtime (если переменная доступна для записи)

### Поддерживаемые типы

| Тип в `module_param` | C-тип |
|---|---|
| `bool` | `bool` |
| `invbool` | `bool` (инвертированный) |
| `byte` | `unsigned char` |
| `short` | `short` |
| `ushort` | `unsigned short` |
| `int` | `int` |
| `uint` | `unsigned int` |
| `long` | `long` |
| `ulong` | `unsigned long` |
| `charp` | `char *` (память выделяет ядро) |

### Массивы как параметры

```c
int myarray[4];
int count;
module_param_array(myarray, int, &count, S_IRUSR);
/* При передаче: insmod mod.ko myarray=1,2,3 */
```

`count` заполнится фактическим числом переданных элементов.

### `MODULE_PARM_DESC()`

Описание параметра — попадает в `.modinfo`, видно через `modinfo`:
```
parm: myshort:A short integer (short)
parm: myint:An integer (int)
```

---

## Часть 6. Модуль из нескольких файлов — `start.c` + `stop.c`

```c
/* start.c */
#include <linux/kernel.h>
#include <linux/module.h>

int init_module(void)
{
    printk("Hello, world - this is the kernel speaking\n");
    return 0;
}
```

```c
/* stop.c */
#include <linux/kernel.h>
#include <linux/module.h>

void cleanup_module()
{
    printk("<1>Short is the life of a kernel module\n");
}
```

```makefile
# Makefile
obj-m += startstop.o
startstop-objs := start.o stop.o
```

### Как это работает

`obj-m += startstop.o` — говорит kbuild: «собери модуль с именем `startstop.ko`».
`startstop-objs := start.o stop.o` — говорит kbuild: «этот модуль состоит из двух объектных файлов».

kbuild скомпилирует `start.c → start.o`, `stop.c → stop.o`, затем слинкует их в `startstop.ko`. Это стандартный способ разбить большой модуль на файлы.

---

## Компиляция. Makefile.

```makefile
obj-m += hello-1.o
obj-m += hello-2.o
obj-m += hello-3.o
obj-m += hello-4.o
obj-m += hello-5.o
obj-m += startstop.o
startstop-objs := start.o stop.o
```

```sh
make -C /lib/modules/$(uname -r)/build M=$PWD modules
```

### Разбор флагов команды

| Флаг | Значение |
|---|---|
| `-C /lib/modules/$(uname -r)/build` | Перейти в каталог с Makefile **ядра** (там вся система сборки kbuild) и выполнять make оттуда |
| `M=$PWD` | Сообщить kbuild, где находятся **исходники внешнего модуля** — сюда kbuild вернётся для сборки |
| `modules` | Цель сборки: собрать внешние модули (`.ko`) |

### `obj-m +=`

`obj-m` — переменная kbuild. Означает: «эти объекты собрать как **загружаемые модули**» (Module). В противовес:
- `obj-y` — встроить прямо в ядро (built-in)
- `obj-n` — не собирать вообще

Строки `obj-$(CONFIG_FOO)` в официальных Makefile-ах ядра раскрываются в `obj-y` или `obj-m` в зависимости от конфигурации (файл `.config`).

### Артефакты сборки

| Файл | Что это |
|---|---|
| `hello-1.ko` | **Итоговый модуль ядра** — это нужно загружать |
| `hello-1.o` | Объектный файл из вашего `.c` |
| `hello-1.mod.c` | Автосгенерированный файл с метаданными модуля |
| `hello-1.mod.o` | Скомпилированные метаданные |
| `Module.symvers` | Экспортируемые символы модуля |
| `modules.order` | Порядок модулей для `modprobe` |
| `.*.cmd` | Кэш команд для инкрементальной сборки |

---

## Добавление (удаление) модуля в (из) ядра

```sh
sudo insmod hello-1.ko        # загрузить модуль
dmesg | tail -5               # увидеть вывод printk (сообщения ядра)
lsmod | grep hello            # убедиться, что модуль в списке
sudo rmmod hello_1            # выгрузить (имя с подчёркиванием вместо дефиса!)
dmesg | tail -5               # увидеть Goodbye...
```

**Почему `rmmod hello_1`, а не `hello-1`?**
Ядро хранит имена модулей с заменой дефиса на подчёркивание. Это видно в `lsmod`.

---

## `printk()` — логирование в ядре

```c
printk(KERN_ALERT "Hello world 1.\n");
```

`printk` — аналог `printf` для ядра. Принципиальные отличия:
- **Нет `stdout`** — вывод идёт в кольцевой буфер ядра (kernel ring buffer), читается через `dmesg`
- **Уровни приоритета** — определяют, попадёт ли сообщение на консоль
- Строка уровня (`KERN_ALERT` и т.п.) конкатенируется с форматной строкой **без запятой** — это строковая склейка на этапе компиляции

Если уровень приоритета выше (числово меньше), чем `console_loglevel`, сообщение появляется прямо на консоли.

| Имя | Число | pr_-алиас | Когда использовать |
|---|---|---|---|
| `KERN_EMERG`   | 0 | `pr_emerg()`  | Система нежизнеспособна |
| `KERN_ALERT`   | 1 | `pr_alert()`  | Требуется немедленное действие |
| `KERN_CRIT`    | 2 | `pr_crit()`   | Критическое состояние |
| `KERN_ERR`     | 3 | `pr_err()`    | Ошибки |
| `KERN_WARNING` | 4 | `pr_warn()`   | Предупреждения |
| `KERN_NOTICE`  | 5 | `pr_notice()` | Нормально, но требует внимания |
| `KERN_INFO`    | 6 | `pr_info()`   | Информационные сообщения |
| `KERN_DEBUG`   | 7 | `pr_debug()`  | Отладочные сообщения |
| `KERN_DEFAULT` |   |               | Уровень по умолчанию |
| `KERN_CONT`    | c | `pr_cont()`   | Продолжение предыдущей строки |

`pr_info("msg\n")` — современный короткий способ записи вместо `printk(KERN_INFO "msg\n")`.

---

## Полезные команды для отладки

```bash
# Текущий console_loglevel и настройки:
cat /proc/sys/kernel/printk
# Вывод: текущий  дефолтный  минимальный  boot-default

# Показать все сообщения ядра в реальном времени:
sudo dmesg -w

# Информация о модуле (лицензия, параметры, зависимости):
modinfo ./hello-5.ko

# Список загруженных модулей с размером и зависимостями:
lsmod

# Содержимое секции .modinfo в бинарнике:
objdump -s -j .modinfo hello-4.ko
```

---

## Полезные ссылки

- [The Linux Kernel Module Programming Guide (актуальная версия)](https://sysprog21.github.io/lkmpg/) — именно отсюда взяты примеры из методички, обновлено под ядра 5.x/6.x
- [Linux kernel license rules](https://www.kernel.org/doc/html/latest/process/license-rules.html#id1)
- [Linux Device Drivers, 3rd ed.](https://lwn.net/Kernel/LDD3/) — бесплатно, главы 2–3 по теме лабы
- [kernel.org — документация по kbuild](https://www.kernel.org/doc/html/latest/kbuild/modules.html)

[^1]: **Монолитное ядро** — архитектура, при которой всё ядро (драйверы, файловые системы, сетевой стек) компилируется в единый бинарный образ и полностью выполняется в одном адресном пространстве. Противоположность — микроядро, где сервисы работают в userspace. Linux — монолитное ядро с поддержкой динамически загружаемых модулей (гибридный подход).
