// запросы к БД, которые не касаются получения котировок с ценами 
// это моежт быть запись данных в БД для вывода графиков
// или торговые поручения
// или проверка результата выставления торгвых поручений.
// в общем множество разных запросов для работы с БД, 
// которые не выделены в отдельные группы по работе с котировками (истрическими данными)

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <wctype.h> 
#include <wchar.h> 
#include <time.h> 
#include <libpq-fe.h>
#include <ctype.h>
#include <iso646.h>
#include <stdbool.h>
#include <uchar.h> 
#include <locale.h>
#include <math.h>
#include <threads.h> 
#include <unistd.h>
#include <errno.h> 

#include "глобальные_переменные.h" 
#include "extern_активация.h"
#include "нагрузка_пк.h" 
#include "входные_данные.h" 
#include "ошибки_обработка.h" 
#include "системные_функции.h"
#include "файлы_действия.h"
#include "предстарт_наполнение.h"
#include "запросы_к_бд.h" 


// проверка текущего соединения
/* 
 PQstatus Возвращает состояние подключения.
    ConnStatusType PQstatus(const PGconn *conn);
    CONNECTION_OK
    CONNECTION_BAD
    и прочие варианты  стр  861 / 889  в 15 инструкции по PostgreSQL

// PQreset(связь_сервер_quik) - переустановка соединения с теми же параметрами что и раньше
      
void PQreset(PGconn *conn); // переподключение к БД с теми же настройками
 */








// static PGconn *соединение; // этот переменная для соединения с БД (указатель на имя и сам факт подсоединения к БД 
PGconn* fбд_соединение_бд(void)
{
    char настройки_соединения[1024]=""; 
    strcat(настройки_соединения, " dbname=");
    strcat(настройки_соединения, глоб_об_ключи_запуска.arws_db);
    
        if (0==strcmp("localhost", глоб_об_ключи_запуска.arws_host))
        {  
            // при наличии записи host=localhost в ответах на ошибку - ВСЕ ЗАПИСИ ПОВТОРЯЮТСЯ ДВА РАЗА !!!! одинаковые строки и каждый раз парами!
            //strcat(настройки_соединения, " host="); 
            //strcat(настройки_соединения, глоб_об_ключи_запуска.arws_host); 
        }
        else
        {
            strcat(настройки_соединения, " hostaddr="); 
            strcat(настройки_соединения, глоб_об_ключи_запуска.arws_host); 
        }

    strcat(настройки_соединения, " port="); 
        strcat(настройки_соединения, глоб_об_ключи_запуска.порт_бд); 
    strcat(настройки_соединения, " user=");
        strcat(настройки_соединения, глоб_об_ключи_запуска.arws_user);
    strcat(настройки_соединения, " password=");
        strcat(настройки_соединения, глоб_об_ключи_запуска.arws_пароль);
    
    
    PGconn *соединение = PQconnectdb(настройки_соединения); 
        // " dbname=mmvb_robot port=5432 user=postgres password=1TTRG")
    
    PQsetErrorContextVisibility(соединение,  PQSHOW_CONTEXT_ALWAYS);
    PQsetErrorVerbosity(соединение,  PQERRORS_VERBOSE);
    char er_msg[1000]="# ";
    strcat(er_msg, PQerrorMessage(соединение));   
    
    
    if (PQstatus(соединение) == CONNECTION_OK)
    {
        //printf("\t\t Соединение с сервером БД установлено.\n");
    }
    else if (PQstatus(соединение) == CONNECTION_BAD)
    {
            // запись в текстовой файл об ошибке !!!!
        printf("Соединение с сервером БД НЕТ!!!!!!!!\n");
        fо_текст_ошибки(er_msg, -10);     
        PQfinish(соединение);
        
         // void PQreset(PGconn *conn); 
    }
    
    return соединение;
}








// ========================================================================================
// создаёт список акций для работы сегодня. Просто все акции, которые удовлетворяют условиям:
// 1) торгуется больше 90 дней
// 2) оборот более 5 млн_руб/в день за последние 20 дней
// 3) ограничение последнего торгвого дня. Если торги ВЧЕРА по ней отсутствовали (а по ГАЗПРОМ и СБЕР были),
//         то такую акцию игнорируем
int fбд_получить_список_secid(PGconn *связь_сервер_quik)
{
    PGresult *результат_запроса;  
    char* п_запрос_1="with t0 as ( "
        " select distinct secid from  акции_рф.t_скачанных_дат where is_primary = true " 
        " and время_шаг = 'D1' and история_начало is not null and история_начало <= (now()-interval '3 month')::date "
        " ), " 
        " t1 as (" 
        " select secid from справочники.t_средние_объемы_торгов " 
        " where средний_v_руб>(5000*1000) and is_activ=true "   // and листинг is not null  он при дневном обновлении - обнуляется!!!!
        " )" 
        " select t1.secid as secid from t0 as t0 inner join t1 as t1 on t0.secid = t1.secid"
        " order by secid "; 
    
    результат_запроса = PQexec(связь_сервер_quik, п_запрос_1); 
    int row_number = 0;
    
    if (PQresultStatus(результат_запроса) == PGRES_TUPLES_OK) // успешное завершение команды, которая возвращает данные 
    {         
        // возвращает число строк (кортежей) в полученной выборке (срок с результатами)
        row_number = PQntuples(результат_запроса);
        глоб_список_secid_штук=row_number;
        
        for (int i=0; i<row_number; i++)
        {
            // возвращает значение одного поля (столбца) из одной строки, содержащейся в PGresult. То есть значения одной ячейки.
            strcpy(глоб_список_secid[i], PQgetvalue(результат_запроса, i, 0)); // PQgetvalue(результат_запроса, i, 0)
            // printf("%i) инструмент=%s\n", i, PQgetvalue(результат_запроса, i, 0));
        }
    }
    else if (PQresultStatus(результат_запроса) != PGRES_TUPLES_OK)
    {
        PQsetErrorContextVisibility(связь_сервер_quik,  PQSHOW_CONTEXT_ALWAYS);
        PQsetErrorVerbosity(связь_сервер_quik,  PQERRORS_VERBOSE);
        char er_msg[5000]="# ";
        strcat(er_msg, PQresultErrorMessage(результат_запроса)); // ОДИНАКОВО!!! = strcat(er_msg, PQerrorMessage(связь_сервер_quik));  
        strcat(er_msg, " ");
            //char *Pconst PGresult *res,    PGVerbosity verbosity,  PGContextVisibility show_context);
            // strcat(er_msg, PQresultVerboseErrorMessage(результат_запроса, PQERRORS_VERBOSE, PQSHOW_CONTEXT_ALWAYS));  = ОДИНАКОВО!!!  strcat(er_msg, PQresultErrorMessage(результат_запроса));
        
        printf("\t ОШИБКА: при запросе: \n\t %s\n", er_msg); // PQerrorMessage(связь_сервер_quik)
        
        fо_текст_ошибки(er_msg, -30); 
        PQclear(результат_запроса);  
    
        return -30; 
    }
    
    return 0;
}


