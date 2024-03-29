#include <stdio.h> // perror()
#include <stdlib.h> 
#include <string.h>
#include <wctype.h> 
#include <wchar.h> 
#include <time.h> 
/* #include <libpq-fe.h>   // есть /usr/include/postgresql/ */
#include <ctype.h>
#include <iso646.h>
#include <stdbool.h>
#include <uchar.h>  //  /usr/include/uchar.h
#include <locale.h>
#include <math.h>
#include <threads.h> 
#include <unistd.h> // для sleep() read()
#include <errno.h> // return EXIT_SUCCESS; 
#include <sys/stat.h>




#include "глобальные_переменные.h" 
#include "extern_активация.h"



// создание полной временной метки как текст на русском языке
int fс_дата_время_текст_значение(void)
{
    strcpy(глоб_время_как_текст, " ");
    struct tm об_время = *localtime(&(time_t){time(NULL)});
    // char ars_время[50]=""; указатель на массив приходит из вне
    
    char* dw[7]={
        "воскресение  ",
        "понедельник  ",
        "вторник      ",
        "среда        ",
        "четверг      ",
        "пятница      ",
        "суббота      " };

    strftime(глоб_время_как_текст, 40, "%Y.%m.%d %H:%M:%S, ", &об_время);
    // printf("\n Текущее время %s \n", asctime(&об_время));   // Текущее время Wed Nov  1 08:32:12 2023
    
    strcat(глоб_время_как_текст, dw[об_время.tm_wday]);
    if (1 == глоб_режим_тестов)
    {
        // printf(" Сейчас: %s\n", глоб_время_как_текст); // Нов формула: 2023.11.01 08:32:12, среда  
    }
 
    return 0;
}






// устанавливаем задержку, паузу в  выполнение программы. Пауза может быть любой продолжительности и дробным числом!
//  функция по паузе (устанавливаем требуемое плавающее значение!!!
int fс_пауза_вработе_программы(int секунды, double дробные_секунды)
{
    // printf("\nТекущее время %s \n", asctime(localtime(&об_нагрузка_оперсист.время_начало))); 

    //struct timespec время_начало;
    //struct timespec время_старт;
    //struct timespec время_стоп;
    
    //timespec_get(&время_начало, TIME_UTC);
    //char текущее_время[100];
    //strftime(текущее_время, sizeof текущее_время, "%D %T", gmtime(&время_начало.tv_sec));
    //printf("Время перед паузой: %s.%09ld UTC\n\n", текущее_время, время_начало.tv_nsec);
    
    thrd_sleep ( & ( struct timespec ) {.tv_sec = секунды, .tv_nsec=(дробные_секунды * 1000000000)} , NULL ) ;  // сон 2,5 секунда 
    
    //timespec_get(&время_начало, TIME_UTC);
    //strftime(текущее_время, sizeof текущее_время, "%D %T", gmtime(&время_начало.tv_sec));
    //printf("Время после паузы: %s.%09ld UTC\n\n", текущее_время, время_начало.tv_nsec);
    
    return 0;
}




// устанавливаем задержку паузы вноносекундах при выполнение программы. Для снятие перегрузок ПК в автоматическом режиме.
// пауза расчитывается автоматически в зависимости от нагрузки на ПК и температуры процессора
// в программе void* fп_нагрузка_пк_постоянно(void *arg) 
void fс_автопауза_вциклах()
{
    thrd_sleep ( & ( struct timespec ) {.tv_sec = 0, .tv_nsec=(глоб_коэффиц_паузы * 1000)} , NULL ) ;  // 0.000'001 * 1'000'000'000
    return ;
}




// Счётчик времени (секундомер с наноскундами)
int fс_секундомер_наносек(struct timespec* время_начало)
{
    //struct timespec время_начало; // точность наносекунды (1 млрд-ная)
            //struct tm время_начало = *localtime(&(time_t){time(NULL)}); точность 1 секунда !!!!
    
    //struct timespec время_начало;
    //timespec_get(&время_начало, TIME_UTC);
    
    // .... код
    
    //fс_секундомер_наносек(&время_начало);
     
    struct timespec время_стоп;
    timespec_get(&время_стоп, TIME_UTC);
    char текущее_время[100];
    
    
    strftime(текущее_время, sizeof текущее_время, "%D %T", gmtime(&время_стоп.tv_sec));
    printf("Время завершения программы: %s.%09ld UTC \t", текущее_время, время_стоп.tv_nsec);
    printf("\033[32mЗатрачено времени: %2.6f \033[0m \n", 
        (время_стоп.tv_sec + (float)время_стоп.tv_nsec/1000000000.0) - (время_начало->tv_sec + (float)время_начало->tv_nsec/1000000000.0) );

    return 0;
}




// размер журнала учёта ошибок и действий  в байтах
long int fс_размер_логжурнала(void)
{
    FILE *fp = fopen(глоб_логфайл_путь, "r");
    struct stat st;
    int fd = fileno(fp);    // получаем дескриптор файла
    fstat(fd, &st);
    off_t size = st.st_size;  /* total size, in bytes */
    long int размер_файла = (long int) size;

    return размер_файла;
}






// возвращает время в секундах, насколько время в Москве (+2/+3) больше времени по Гринвичу (+0)
// нужно, чтобы время в секундах формата time_t t = time(NULL)  было приведено к локальному формату (времени)
// а то вместо 00:15:00 по местному времени срабатывает в 03:00:02 (из за разница в 3 часа)
int fс_разница_сек_часовых_поясов(void)
{
    // делаем это каждую поночь, примерно в 00:15
    // 
    int разница_сек = 0;
    time_t время_UTC = time(NULL);
    
    int h_m_s__UTC = ((long int) время_UTC) % (24*3600);
    //int минут_UTC = ((long int) время_UTC) % (3600); // есть отличия от гринвича и в 30 минут (индия, азия)

    struct tm время_локальное = *localtime(&время_UTC);
    
    разница_сек = 3600*время_локальное.tm_hour + 60*время_локальное.tm_min + время_локальное.tm_sec - h_m_s__UTC;
    
    return разница_сек;
}


// вывод данных о процессоре, его загрузке и температуре в цельсиях
int fс_сообщение_о_нагрузке(void)
{
    
    fс_дата_время_текст_значение();
    
    printf("%sНагрузка ПК: %5.2f%%    Температура CPU: %4.1f C (текущая: %3.1f)\n",
    глоб_время_как_текст, глоб_об_нагрузка_оперсист.cpu_експотенц_средн, глоб_об_нагрузка_оперсист.cpu_t_експотенц_средн,
            глоб_об_нагрузка_оперсист.cpu_t_now );
    
    return 0;
}