# Нагрузка ПК по 29 параметрам на языке Си + БД PostgreSQL 15 (декабрь 2023).

<p> &nbsp; </p>   

В данном разделе выложен итоговый проект №_2 (второй из двух сдаваемых в декабре 2023) 
на языке Си (чистый Си), в рамках обучения на онлайн курсе “Программист С” 
компании otus.ru с января 2023 по февраль 2024 года.

<p> &nbsp; </p>   






## ЗАДАЧА ПРОЕКТА.

Первоначально задумывался другой проект (ПРОЕКТ  по языку Си + PostgreSQL на тему:
"Оптимизация производительности БД PostgreSQL путём автоматического распараллеливания задач" = №_1). Он так же реализован.

**Минус - проект №_1 должен располагаться на ноутбуке**. 

Встала задача снизить нагрузку на ПК, но не в лоб, а с максимальными нагрузками, указанные пользователем, 
то есть нагрузка должна динамически меняется в зависимости от нескольких факторов (температура и нагрузка процессора). 
Были попытки использовать сторонние ресурсы, но они мне не подошли. 
Поробнее об этом в описании к проекту в папке 
[описание_инструкции](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_nagruzka_PC/tree/main/%D0%BE%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5_%D0%B8%D0%BD%D1%81%D1%82%D1%80%D1%83%D0%BA%D1%86%D0%B8%D0%B8). 

В ходе реазизации этой мелкой задачи, она переросла в отдельный проект с анализом множества факторов нагрузок. 
Последние, еще до старта основного проекта №_1, помогли опимизировать настройки БД PostgreSQL ещё более тонко/ювелирно.


<p> &nbsp; </p>  


## Что умеет проект.

> 1.  Проект реализован для Debian 10/11. Возможна работа на Ubuntu.
> 2.  Все данные запрашиваются напрямую из ядра ОС минуя программы обработчики (типа TOP Htop и прочие похожие).
> 3.  В связи с этим нагрузка  в простое, с учетом нагрузки самой ОС около 0.3% - 0.4%.
> 4.  Работа с нагрузкой процессора.
> 5.  Работа с температурой процессора.
> 6.  Работа с оперативной памятью системы (процент использования и другие параметры).
> 7.  Работа с SWOP диском (памятью). Были ли записи и чтения с него.
> 8.  Отправка данных 29 индикаторов из программы в БД PostgreSQL 15.
> 9.  Каждые 5 минут новая запись в БД, каждые 1 минута обновления записи в БД.
> 10. Опрос всех параметров нагрузки ПК совершается каждые 1-15 секунд (настраивается пользователем).
> 11. Программа легко делается демоном (настройки для автозапуска при перезагрузке системы приведены).
> 12. Есть отладочный (тестовый) режим, при котором отладочная информация выводится на экране монитора (в терминале).
> 13. Есть свой собственный журнал учета лог-записей. Он так же настраивается по указанному адресу.
> 14. Тестовые проверки при запуске программы.
> 15. Есть возможность очистки памяти два раза в сутки от кэшей БД (у меня БД ночь получает/скачивает много данных, которые дальше не нужны). 
> 16. Вывод графиков на сайте Apache2 через код/скрипты на чистом Python. Приведен код для отображения графиков на сайте. Установку самого сервера Apache 2 делаете самостоятельно.
> 17. В архиве лежит сама БД для PostgreSQL со всеми настройками (слепок БД).
> 18. Программа имеет настраиваемые параметры  при запуске (11 ключей).

<p> &nbsp; </p>  

Более подробное описание самой программы, настроек, проблем - изложены в инструкции и описании к проекту. Можно скачать *.pdf файл.
[Описание Си проекта нагрузки ПК.pdf](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_nagruzka_PC/blob/main/%D0%BE%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5_%D0%B8%D0%BD%D1%81%D1%82%D1%80%D1%83%D0%BA%D1%86%D0%B8%D0%B8/%D0%9E%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5__%D0%A1%D0%B8_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82_%D0%BD%D0%B0%D0%B3%D1%80%D1%83%D0%B7%D0%BA%D0%B0_%D0%BF%D0%BA.pdf) 

<p> &nbsp; </p>  

**Внимание!!!** Весь код пишется с учётом работы в системе Debian (Linux). Кодировка = ru_RU.UTF-8. 
БД PostgreSQL имеет кодировку ru_RU.UTF-8. Переменные внутри кода, пути к файлам (имена папок), 
названия файлов могут содержать в себе кириллицу (русские буквы). Такая особенность перешла из работы с БД PostgreSQL.


<p> &nbsp; </p>   

[фото лето 2021 г.]![](https://github.com/OTUS-2023-C01-SARTAKOV-AP/home_work/blob/main/img/photo_circle_small.png) 


<p> &nbsp; </p>    
<p> &nbsp; </p>   

----
### Кратко о себе: 

Меня зовут Сартаков Алексей. Живу в г. Нижний Новгород. 

> *   С 2009 года изучал Cache Intersystems. Одновременно с этим активно работал на html, SVG графиках любой сложности.
> *   В 2016 году стал изучать PostgreSQL. 
> *   С 2019 стал работать с Python. С 2019 немного Python на стороне БД PostgreSQL. С 2020 много Python на стороне сайта [сайт на голом Питоне (не фрэймворки)]. 
> *   С 2021 изучаю Linux (сам работаю в Debian c 2018 года и она у меня как основная система). Больше меня интересует именно уровень программного обеспечения, а не железа. 
> *   С конца 2021 стал активно изучать Си, т.к. Питон не справлялся с теми задачами, которые на него возлагал (по части БД).  К сожалению, самостоятельно изучить Си не смог (не хватило знаний по нюансам языка). 
> *   Поэтому с января 2023 пошёл на курсы языка Си в образовательную платформу otus.ru. Изучил все что хотел и даже в несколько раз больше. Узнал очень много технической документации. 

<p> &nbsp; </p>   

На PostgreSQL с 2016 года. Первые рабочие проекты пошли **уже с 2017 года** (в 2016 были просто обучения с решениями тестовых задач).

<p> &nbsp; </p>    
<p> &nbsp; </p>          

----
### Умею и знаю:

> 1.  Оптимизация БД PostgreSQL (настройки самого сервера, разбор плана запросов).
> 2.  Разумеется все JOIN , подзапросы, окна, CTE (With ...), группировка, индексы. 
> 3.  BackUp базы данных (автоматически по расписанию через демонов) и её восстановление.
> 4.  Права и роли в PostgreSQL (сайт построен с иерархией ролей по доступу к БД).
> 5.  Логическая репликация. Закачка и разбор данных (в автоматическом режиме) из вне в БД.
> 6.  Регулярные выражения. Сематический разбор текста силами самого PostgreSQL (занимался одно время парсингом/разбором данных с сайтов).
> 7.  Контроль ошибок на стороне сервера (БД) с их занесением в отдельные таблицы с полным описанием проблемы.
> 8.  Плотно работаю с pl/pgSQL (90% кода на нём). 
> 9.  Python, SVG, html (в связке как сайт с запросами к БД, код SVG генерируется на стороне сервера [в PosgreSQL]).
> 10.  Вывод результатов в виде сложных графиков.
> 11.  Администрирование Debian (достаточном для написания скриптов, демонов, ролей, прав).
> 12.  С изучением языка Си вышел на более качественный уровень (связка БД PostgreSQL + язык Си). 

<p> &nbsp; </p>   
<p> &nbsp; </p>   

Найти меня можно в телеграмм адресу https://t.me/Sartakov_Aleksey
<p> &nbsp; </p>   
😂 Дата 08 декабря 2023 г.
