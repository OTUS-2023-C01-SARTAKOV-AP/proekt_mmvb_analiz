
add_library(modul_potoki потоки.c потоки.h) 
target_include_directories(modul_potoki PUBLIC .)

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
