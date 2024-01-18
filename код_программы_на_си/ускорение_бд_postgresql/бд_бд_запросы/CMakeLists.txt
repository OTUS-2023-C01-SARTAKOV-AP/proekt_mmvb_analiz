
add_library(zaprosi_in_db запросы_к_бд.c запросы_к_бд.h запросы_к_бд_предторги.c) 
target_include_directories(zaprosi_in_db PUBLIC .)



find_package(PostgreSQL)

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


set (IMPORTED True)
find_file(PostgreSQL 16.0) 	# OK

message(STATUS "\nИщем библиотеку PostgreSQL: <libpq-fe.h> и ...*/libpq.so ")
message(STATUS "PostgreSQL_FOUND (find_package)= ${PostgreSQL_FOUND}")		# Верный, если PostgreSQL найден.
message(STATUS "PostgreSQL_LIBRARIES = ${PostgreSQL_LIBRARIES}")		# библиотеки PostgreSQL необходимы для соединения
message(STATUS "PostgreSQL_INCLUDE_DIRS = ${PostgreSQL_INCLUDE_DIRS}") 		# каталоги заголовков PostgreSQL
message(STATUS "PostgreSQL_LIBRARY_DIRS = ${PostgreSQL_LIBRARY_DIRS}") 		# каталоги ссылок для библиотек PostgreSQL
message(STATUS "PostgreSQL_VERSION_STRING = ${PostgreSQL_VERSION_STRING}") 	# версия PostgreSQL найдена
