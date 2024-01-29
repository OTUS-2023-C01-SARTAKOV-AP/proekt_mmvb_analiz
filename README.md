# Оптимизация производительности БД PostgreSQL путём автоматического распараллеливания задач (декабрь 2023).

<p> &nbsp; </p> 

В данном разделе выложен итоговый проект №_1 (основной/главный из двух сдаваемых в декабре 2023) 
на языке Си (чистый Си), в рамках обучения на онлайн курсе “Программист С” 
компании otus.ru с января 2023 по февраль 2024 года.

<p> &nbsp; </p> 


# ЗАДАЧА ПРОЕКТА. 

Встала задача обрабатывать вычисления в несколько потоков. Сперва пытался сделать это силами самой БД. 
Очень много сил уходило на дробление на потоки. Что либо потом переделать было просто нереально, проще было написать заново. 
Кроме того, сам запуск этих функций как отдельных потоков был еще той задачей.

Позже, решил попробовать Python. Но с ним так же были проблемы. 
Сам по себе он довольно таки медленный, а если хочень быстрее - нужно изучать кучу вспомогательных к нему приложений. 
Да и результаты по скорости работы не сказать что получались выдающиеся. Они не сильно отличались от скорости работы БД PostgreSQL. 

Тогда решил действовать кардинально - изучисть Си, как очень быстрый по скорости работы. 


<p> &nbsp; </p> 

Более подробное описание самой программы, настроек и установки БД, возникших проблемах и их решениях - изложены в инструкции и описании к проекту. Можно скачать *.pdf файл.

[Описание Си проекта нагрузки ПК.pdf](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/%D0%BE%D0%BF%D0%B8%D1%81%D0%B0%D0%BD%D0%B8%D0%B5_%D0%B8%D0%BD%D1%81%D1%82%D1%80%D1%83%D0%BA%D1%86%D0%B8%D0%B8/%D0%A1%D0%B8_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82_2024_%D1%83%D1%81%D0%BA%D0%BE%D1%80%D0%B5%D0%BD%D0%B8%D0%B5_PostgreSQL.pdf) 

<p> &nbsp; </p> 

**Внимание!!!** Весь код пишется с учётом работы в системе Debian (Linux). Кодировка = ru_RU.UTF-8. 
БД PostgreSQL имеет кодировку ru_RU.UTF-8. Переменные внутри кода, пути к файлам (имена папок), 
названия файлов могут содержать в себе кириллицу (русские буквы). Такая особенность перешла из работы с БД PostgreSQL. 


<p> &nbsp; </p> 

## Что я хотел получить от реализации этого проекта:

> * Требовалось, чтобы первоначальная выборка из БД и первоначальные расчеты в однопоточном режиме работали как можно быстрее. 
На начало написания проекта, в БД PostgreSQL 15 эти действия занимали около 15 минут.
> * В виду того, что нагрузки и вычисления планируются большие (около 15 мл точек с данными за 1 цикл) и делается все это на ноутбуке, нужно создать защитный стопор, который принудительно снижает нагрузку на CPU путём увеличения паузы между функциями/командами. По умолчанию стандартная пауза = 1/100 млн секунды. Защита по нагрузке или по температуре центрального процессора.
> * Решить проблему с масштабиоуемостью проекта (многопоточностью) без особых затрат по переделке проекта.
> * Сделать так, чтобы один цикл полных расчётов для принятия решения занимал не более 7 секунд. В идеале не более 5 секунд.

<p> &nbsp; </p> 

## Что умеет проект.
> * Получать из БД исторические данные (минутные срезы) в том числе в многопоточном режиме. 
> * Переводить минутки в заданные тайм фрэймы: м1, м2, м3, м4, м5, м6, м10,м12, м15, м20, м30. 
> * Создавать сложные структуры, соединения с БД, балансировку нагрузки (методом тувеличения задержек). 
> * Работать с указанным количеством потоков при выборке и расчетах. 
> * Инструкции при старте программы. 
> * Настраиваемые ключи при запуске программы. 

<p> &nbsp; </p> 


# Что получилось:

Однопоточный режим, выборка из БД по 165 акциям для генерации 11 таймфрэймов: 
![Однопоточный режим, выборка из БД по 165 акциям для генерации 11 таймфрэймов](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/th_1__gen_tf.png)

<p> &nbsp; </p> 

Выборка в 12 потоков из БД по 165 акциям для генерации 11 таймфрэймов: 
![Выборка в 12 потоков из БД по 165 акциям для генерации 11 таймфрэймов](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/th_12__gen_tf.png)

<p> &nbsp; </p> 

Однопоточный режим в Си программе. Выборка из БД и расчет индикаторов (полный аналог ночному коду в БД, который работает 15 минут). Выборка по 165 акциям для генерации всех таймфрэймов: 
![Однопоточный режим, выборка и расчеты.](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/extren_start_01.png)

<p> &nbsp; </p> 

Запуск 12 потоков в Си программе. Выборка из БД и расчет индикаторов (полный аналог ночному коду в БД, который работает 15 минут). Выборка по 165 акциям для генерации всех таймфрэймов: 
![12 потоков. выборка и расчеты](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/extren_start_12.png)

<p> &nbsp; </p> 

Выборка из БД и расчет индикаторов, код, который работает 15 минут. Выборка по 165 акциям для генерации всех таймфрэймов: 
![12 потоков. Выборка и расчеты в самой БД](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/db_viborka.png)


<p> &nbsp; </p> 

Схема взаимодействия компьютеров/блоков в данном проекте:
![Схема взаимодействия компьютеров/блоков в данном проекте:](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/schema.png)


<p> &nbsp; </p> 

----
### Работа над проектом:

Проект реализовывался 20-25 дней по 6-7 часов в день. Основная часть была закончена в конце августа 2023. 
Дальше шли мелкие уточнения. На Си это был мой первый крупный проект. До этого были мелкие тестовые задачи из книг.

В принципе - особых сложностей не было, т.к. с БД уже хорошо и плотно работал более 7 лет. С HTML версткой работал уже лет 20. С Питоном года 4. \
Сам проект на Си имеет довольно таки простую структуру и никаких заковырок не имеет. Чего не скажешь об основном проекте №1 =  <a href="https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz" title="Ускорение работы БД">Ускорение работы БД.</a> 

При работе в проекте использовались PostgreSQL, SQL, Python, HTML + сервер-сайта Apache2, Си.


<p> &nbsp; </p> 

** Многие вещи не вошли в открытый проект **, но были реализованы в рабочем коде программы. \
Такие как:
> * приём текущих котировок;
> * отсев неизменившихся данных (снижает нагрузку на ЦП процентов на 80);
> * расчеты биржевых индикаторов;
> * вывод данных в БД с автоматическим созданием произвольных таблиц по индикаторам;
> * вывод данных в БД для генерации графиков на сайте;
> * а так же генерация срезов исторических данных с записью в БД. 


<p> &nbsp; </p> 

----
### Кратко о себе: 
![фото лето 2021 г.](https://avatars.githubusercontent.com/u/133401686?v=4) 
Фотография: лето 2021 г.


<p> &nbsp; </p> 

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

Найти меня можно в телеграмм адресу https://t.me/Sartakov_Aleksey

<p> &nbsp; </p> 

😂 Дата 08 декабря 2023 г.

<p> &nbsp; </p> 



