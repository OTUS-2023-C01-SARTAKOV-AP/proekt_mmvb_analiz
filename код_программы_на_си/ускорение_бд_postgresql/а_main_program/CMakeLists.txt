# SET(BUILD_SHARED_LIBS FALSE) означает, что в данном моделу запрещено создавать динамическую библиотеку
# влияет на все модули, после команды выше = BUILD_SHARED_LIBS FALSE или как STATIC в коде ниже

set (IMPORTED True)

include_directories(../а_main_program)
include_directories(../б1_блок_нагрузка_пк)
include_directories(../б2_блок_предстарт_наполнения)
include_directories(../бд_бд_запросы)
include_directories(../н_настр_ключи_при_запуске)
include_directories(../о_ошибки_обработка)
include_directories(../п_потоки_независимые)
include_directories(../с_системные_команды)
include_directories(../ф_файлы_внешние)
include_directories(/usr/include/postgresql) # include <libpq-fe.h> OK

include_directories(/usr/lib/x86_64-linux-gnu) 	# для многопоточности
include_directories(/lib/x86_64-linux-gnu) 	# для многопоточности

add_executable(${PROJECT_NAME} main.c) 	# названием модуля main.c он же потом нужен будет при компиляции проекта.
	# Название ТОЛЬКО латиницей, иначе будет ошибка при компиляции (даже на самом свежем Debian

