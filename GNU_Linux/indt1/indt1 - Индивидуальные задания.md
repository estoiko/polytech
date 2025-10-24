---
course: "[[GNU Linux]]"
type: individual task
date:
author: estoiko
---
#linux #bash

## Индивидуальные практические задания по использованию команды find

1. В `/var/log` найти файлы `*.log` размером больше 1 МБ, изменённые за последние 7 дней, и подсчитать строки в каждом (`wc`).
```bash
# -l - вывод только кол-ва строк (без кол-ва слов и байт)
find /var/log -name "*.log" -size +1M -mtime -7 -exec wc -l {} \;
```
2. В домашнем каталоге найти скрытые каталоги глубиной не выше 2 и вывести их пути.
```bash
find . -maxdepth 2 -type d -name ".*"
```
3. В `/usr/bin` найти обычные файлы, изменённые более 180 дней назад, и вывести имя и время изменения.
```bash
find /usr/bin -type f -mtime +180 -printf "%f %t\n"
```
4. В `/home` найти файлы `*.py` , исключая каталоги `.venv` и `__pycache__` , и вывести путь.
```bash
find /home -name "*.py" ! -path "*/.venv/*" ! -path "*/__pycache__/*"
```
5. В `/tmp` найти пустые файлы и пустые каталоги и удалить их с подтверждением.
```bash
# при использовании -empty условие -type f,d - избыточно
find /tmp -empty -exec rm -f -i {} \;
```
6. В `/var` найти файлы размером больше 100 МБ и вывести их размер в байтах и путь.
```bash
# %s - размер в байтах
# %h - путь до файла (без имени файла), %f - имя файла
# %p - полный путь
find /var -size +100M -printf "%s\t%p\n" 
```
7. В `/etc` найти файлы `*.conf`, изменённые за последние 24 часа, и вывести время изменения.
```bash
find /etc -name "*.conf" -mtime -1 -printf "%t\n"
```
8. В `/usr/lib` вывести список каталогов глубиной до 2 уровней.
```bash
find /usr/lib -maxdepth 2 -type d -printf "%f\n"
```
9. В `/var` найти файлы больше 10 МБ, изменённые ровно 2–3 дня назад, и вывести размер в байтах.
```bash
find /var -size +10M -mtime +2 -mtime -3 -printf "%s\n"
```
10. В `/opt` найти каталоги с именем `cache` на глубине 2–4 и удалить их содержимое, не удаляя сами каталоги.
```bash
# нужно ли удалять каталоги вида ../cache/cache/.. ? 
find /opt -mindepth 2 -maxdepth 4 -type d -name "cache" -exec rm -rf {}/* \;
```
11. В дереве проекта исключить `build` и `vendor` , найти файлы без расширения и вывести размер и путь.
```bash
find . -type f ! -name "*.*" ! -path "*/build/*" ! -path "*/vendor/*" -printf "%s\t%p\n"
```

*(22.09.25 проверить в utm)*
12. В `/usr/share` вывести каталоги с именем `locale` не глубже 3 уровней.
```bash
find /usr/share -maxdepth 3 -type d -name "locale" 
```
13. В `/` найти блочные и символьные устройства и вывести путь и тип.
```bash
find / -type b -type c -printf "%p\t%y\n" 
```
14. В `/var/www` вывести файлы, изменённые раньше, чем `baseline.txt`.
```bash
find /var/www -type f -newer baseline.txt
```
15. В `/tmp` найти обычные файлы старше 2 дней и обнулить их (`truncate`).
```bash
find /tmp -type f -mtime +2 -exec truncate -s 0 {} \;
```