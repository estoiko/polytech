#set document(title: [Лабораторная работа $numero$6])

#set text(
  font: "New Computer Modern",
  size: 13pt,
  lang: "ru"
)

#set page(
  paper: "a4",
  margin: (
    left: 30mm,
    right: 15mm,
    top: 20mm,
    bottom: 20mm,
  ),
)

#set heading(numbering: "1.1.")
#show heading: set block(spacing: 1em)

#set par(
  first-line-indent: 1.25cm,
  spacing: 1.2em,
  justify: true,
)

#show link: underline

#show title: set text(size: 13pt)
#show title: set align(center)
#show title: set block(below: 1.2em, above: 14em)

// -----< main page >-----
#align(top + center, block[
  #set par(justify: false)
  Санкт-Петербургский политехнический университет Петра Великого \
  Институт компьютерных наук и технологий \
  Высшая школа программной инженерии
])

#title[
  ОТЧЁТ \
  по лабораторной работе $numero$6 \
  по дисциплине “Системное программное обеспечение GNU/Linux” \
  *Конфигурация и установка ядра Linux* \
]

\ \ \ \ \ \ \ \ 

#grid(
  columns: (4fr, 4fr),
  align(left)[
    Выполнил \
    студент гр. 5130904/50005 \
  ],
  align(right)[
    Стойко Е. А. \
  ]
)

#grid(
  columns: (4fr, 4fr),
  align(left)[
    Руководитель
  ],
  align(right)[
    Петров А. В. \
  ]
)

#align(bottom + center, block[
  Санкт-Петербург 2026 г. 
])

#pagebreak()

#outline()

#set page(
  numbering: "1",
  // header: align(right)[
  //   Конфигурация и установка ядра Linux
  // ],
)

Современные операционные системы семейства Linux позволяют гибко настраивать ядро под конкретную аппаратную платформу и задачи пользователя. Конфигурация и сборка ядра дают возможность оптимизировать систему, исключить ненужные драйверы, повысить производительность и получить практические навыки администрирования.

Актуальность работы обусловлена необходимостью понимания принципов конфигурации и сборки ядра Linux, а также оценки влияния числа потоков компиляции на производительность сборки.

= Цель 
Сконфигурированное и собранное ядро Linux для платформы x86_64 на базе Ubuntu 24.04.3 LTS, а также данные об оптимальном числе потоков сборки.

= Задачи
+ Подготовить систему к сборке ядра.
+ Установить исходные коды ядра дистрибутива.
+ Выполнить конфигурацию ядра.
+ Собрать ядро в виде Debian-пакетов.
+ Разработать сценарий автоматизированной сборки с различным числом потоков.
+ Проанализировать зависимость времени сборки от числа потоков.

= Сведения о системе

== Аппаратная платформа
IRBIS NB267 \
Архитектура: x86_64 \
Процессор: Intel Pentium J3710 @ 1.60GHz \
Ядра: 4 \
ОЗУ: 3.7 GiB \
Swap: 3.7 GiB \

== Программная платформа
ОС: Ubuntu 24.04.3 LTS (Noble Numbat) \

#pagebreak()

= Выполнение работы

== Подготовка системы
Были установлены необходимые для сборки зависимости @canonical_build_kernel:

```sh
sudo apt update && \
    sudo apt build-dep -y linux linux-image-unsigned-$(uname -r) && \
    sudo apt install -y fakeroot llvm libncurses-dev dwarves
```

== Получение исходного кода ядра
Для получения возможности загрузить исходный код ядра строка `deb-src` была добавлена
в файл `/etc/apt/sources.list.d/ubuntu.sources` @canonical_enable_sources.
\ Исходный код ядра был получен из репозитория дистрибутива:

```sh
sudo apt source linux-image-unsigned-$(uname -r)
```

== Конфигурация ядра
Во избежание конфликтов ядер в `Makefile` было изменено имя версии ядра на `custom` @canonical_build_kernel: 

```sh 
VERSION = 6
PATCHLEVEL = 14
SUBLEVEL = 11
EXTRAVERSION = -custom  
```

Для конфигурации использовалась текущая конфигурация системы:

```sh 
cp /boot/config-$(uname -r) .config
make menuconfig
```

Для ускорения сборки была отключена отладочная информация @debian_kernel_common_tasks_debug:

```sh
scripts/config --disable DEBUG_INFO
scripts/config --disable DEBUG_INFO_DWARF_TOOLCHAIN_DEFAULT
```

== Сборка ядра

Сборка выполнялась с использованием цели сборки bindeb-pkg @debian_kernel_bindebpkg:

```sh
make -j4 bindeb-pkg
```

#rect[
```txt
SYNC    include/config/auto.conf.cmd
  GEN     debian
dpkg-buildpackage --build=binary --no-pre-clean --unsigned-changes -R'make -f debian/rules' -j1 -a$(cat debian/arch)
dpkg-buildpackage: info: source package linux-upstream
dpkg-buildpackage: info: source version 6.14.11-custom-4

                                ...

dpkg-genchanges: info: binary-only upload (no source code included)
 dpkg-source --after-build .
dpkg-buildpackage: info: binary-only upload (no source included)
21624.88user 2650.75system 3:22:57elapsed 199%CPU (0avgtext+0avgdata 1358924maxresident)k
17311856inputs+29975544outputs (646major+152745133minor)pagefaults 0swaps
```
]

После завершения сборки были получены пакеты:

```sh
linux-headers-6.14.11-custom_6.14.11-custom-4_amd64.deb  
linux-image-6.14.11-custom-dbg_6.14.11-custom-4_amd64.deb
linux-image-6.14.11-custom_6.14.11-custom-4_amd64.deb    
linux-libc-dev_6.14.11-custom-4_amd64.deb
```

== Установка ядра

Установка выполняется командой:

```sh
sudo dpkg -i linux-*.deb
```
В директории `/boot` были созданы файлы: 

```sh
config-6.14.11-custom 
initrd.img-6.14.11-custom 
System.map-6.14.11-custom 
vmlinuz-6.14.11-custom
```

Проверка версии ядра:

```sh
sudo reboot
uname -r

6.14.11-custom
```

#pagebreak()

= Разработка сценария автоматизированной сборки

В системе имеется 4 логических ядра (`N = 4`).
Согласно заданию, тестирование проводилось для диапазона
`2N+1 = 2*4+1 = 9`. \ Использовался следующий сценарий:

```sh
#!/bin/bash

N=$(nproc)

for ((I=1; I<=2*N+1; I++)); do
    make clean >/dev/null 2>&1

    echo "j=$I"
    sudo time make -j$I bindeb-pkg >/dev/null 2>&1
done
```

= Анализ результатов сборки

Оптимальным числом потоков для данной системы оказалось 5, так как увеличение числа потоков сверх него не приводит к ускорению сборки. 
Сборка на оптимальном количестве потоков заняла 2 часа 27 минут.

#figure(
  image("linux-lab6-graph.png", width: 80%),
  caption: [Время сборки ядра в зависимости от числа потоков],
  supplement: [Рисунок],
) <jgraph>

= Трудности и ошибки, возникшие во время выполнения <difficult>

== Ошибка «certs/x509_certificate_list»
В процессе сборки возникла ошибка:

```sh
No rule to make target 'debian/canonical-certs.pem', needed by 'certs/x509_certificate_list'
```

Причиной является включённая поддержка криптографической подписи модулей ядра (Secure Boot).

При сборке из исходников отсутствуют официальные сертификаты Canonical, поэтому процесс формирования списка доверенных ключей завершается ошибкой. Проблема была решена отключением опций подписи модулей в конфигурации ядра.

```sh
scripts/config --disable CONFIG_SYSTEM_TRUSTED_KEYS
scripts/config --disable CONFIG_MODULE_SIG
```

== Ошибка «bad shim signature»

После установки собранного ядра, при загрузке возникла ошибка «bad shim signature», связанная с включённым механизмом Secure Boot. Поскольку самостоятельно собранное ядро не подписано ключом Canonical, загрузка была заблокирована. Проблема была устранена отключением Secure Boot в настройках UEFI. @reddit_bad_shim_signature

= Вывод
// 1. достигнутая цель;
// 2. решённые задачи;
// 3. возникшие трудности;
// 4. итоги по необходимому числу потоков для эффективной сборки ядра на
// вашей системе;
// 5. выводы, которые вы сделали для себя.
// 
В ходе лабораторной работы было сконфигурировано и собрано ядро Linux для платформы x86_64 на базе Ubuntu 24.04.3 LTS.

Были решены следующие задачи:

- система подготовлена к сборке
- установлены исходные коды ядра
- выполнена конфигурация
- разработан сценарий автоматизированной сборки
- проведён анализ зависимости времени сборки от числа потоков

В ходе работы возникли некоторые трудности, которые были успешно преодолены (см. @difficult).

При использовании большого количества потоков компиляции наблюдалось замедление сборки. Причиной является ограниченный объём оперативной памяти (3.7 ГБ).

В ходе работы было выяснено, что оптимальным числом потоков для данной системы является 5. Экспериментально было установлено, что увеличение числа потоков выше определённого значения не ускоряет, а замедляет сборку.

При превышении доступного объёма ОЗУ система начинает активно использовать swap-раздел. Поскольку доступ к диску значительно медленнее, чем к оперативной памяти, процессы компиляции переходят в режим ожидания ввода-вывода, что снижает общую производительность.

#bibliography(
  "ref.bib", 
  style: "gost-r-705-2008-numeric", 
  title: [Список использованных источников],
)