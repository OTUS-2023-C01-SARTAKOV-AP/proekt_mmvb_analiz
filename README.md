# Оптимизация производительности БД PostgreSQL путём автоматического распараллеливания задач (декабрь 2023).

<p> &nbsp; </p> 

В данном разделе выложен ПРОЕКТ №_1 (основной/главный из двух сдаваемых в декабре 2023) 
на языке Си (чистый Си), в рамках обучения на онлайн курсе “Программист С” 
компании otus.ru с января 2023 по февраль 2024 года.

<p> &nbsp; </p> 


# ЗАДАЧА ПРОЕКТА. 

Встала задача делать ограмные вычисления в несколько потоков. \
Сперва пытался сделать это силами самой БД Cache InterSystems (2009-2016 гг). \
Очень много сил уходило на создание отдельных потоков (под каждый поток делался свой отдельный код). Что либо потом переделать было просто нереально, проще было написать заново. \
Кроме того, сам запуск этих функций как отдельных потоков был еще той головной болью. Это было единственной попыткой создать многопоточное приложение силами БД. 

Позже, решил попробовать Python. Но с ним так же были проблемы. 
Сам по себе он довольно таки медленный, а если хочень быстрее - нужно изучать кучу вспомогательных к нему приложений. 
Да и результаты по скорости работы не сказать что получались выдающиеся. Они не сильно отличались от скорости работы БД PostgreSQL (дела много тестов - отличия были небольшие и PostgreSQL в 70% был быстрее, иногда ощутимо быстрее). Да и стыковка разных библиотек на Python в одном проекте ещё та головная боль. 

Но в 2021 году очень сильно было "надо" при работе с большими объемами вычислений. Помятуя всё свои ранние опыты - решил действовать кардинально: **изучить Си, как очень быстрый по скорости работы и минимальным ресурсам в работе**. 


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
> * В виду того, что нагрузки и вычисления планируются большие (около 15 млн точек с данными за 1 цикл) и делается все это на ноутбуке, требовалось создать защитный стопор, который принудительно снижал бы нагрузку на CPU путём увеличения паузы между функциями/командами. По умолчанию стандартная пауза = 1/100 млн секунды. То есть создать защиту по предельной нагрузке или по предельной температуре центрального процессора. Способ кажется странным, но в данном проекте он показал себя очень эффективным и работоспособным. 
> * Решить проблему с масштабируемостью проекта (многопоточностью) без особых затрат по переделке проекта/кода. То есть ввел цифру 1, тогда это однопоточный режим. Ввёл цифру 6 - тогда используется 6 потоков. Ввёл 32 - получи 32 независимых потока (если система имеет столько ядер). В проекте стоит ограничение на 12 потоков.
> * Сделать так, чтобы один цикл полных расчётов для принятия решения занимал не более 7 секунд. \
**В идеале не более 5 секунд.**

<p> &nbsp; </p> 

## Что умеет проект.
> * Получать из БД исторические данные (минутные срезы) в том числе в многопоточном режиме. 
> * Переводить минутки в заданные тайм фрэймы: м1, м2, м3, м4, м5, м6, м10,м12, м15, м20, м30. Опять же в многопоточном режиме.
> * Создавать сложные структуры, соединения с БД, балансировку нагрузки (методом увеличения задержек между функциями). 
> * Работать с указанным количеством потоков при выборке и расчетах. Отличия в 1 и 12 потоков оказались чуть более 3-х раз, хотя процессор 6-ти ядерный и имеет 12 потоков. Думал, что получу большую прибавку скорости в 12 поточном режиме. Оптимизация возможно, но даст прибавку в скорости не более 7-12% максимум (основные блоки где можно было разделить на потоки - были внедрены). 
> * Вывод инструкций при старте программы. 
> * Настраиваемые ключи при запуске программы. 

<p> &nbsp; </p> 


# Что получилось:

Всего ПК имеет 6 реальных ядер и 12 с режимом Hyper threading technology. \
Хочется обратить особое внимание на то, что PostgreSQL быстро работает только при выделении 2-4 Гб оперативной памяти (у меня из 16 Гб выделено 5 Гб).\
При этом, для работы самой Си программы потребовалось около 20 Мб на всё (открытая версия проекта)!\

Всего для анализа подходит 165 инструментов.\
размер памяти на 1 элемент таймфрэйм_m1: 10160\
для всей структуры таймфрэйм_шаблон: 1679344 (всего 11 ТаймФрэймов)\
**ИТОГО = 11*(10’600 +1’679’244) = 18’588’284 байт**


Однопоточный режим, выборка из БД исторических данных и укрупнение тафмфрэймов по 165 акциям для генерации 11 таймфрэймов: 
![Однопоточный режим, выборка из БД по 165 акциям для генерации 11 таймфрэймов](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/th_1__gen_tf.png)

Здесь картина радует глаз. В одно поточном режиме программа на Си делает выборки из БД Postgres и компоновки таймфрэймов за 7 секунд ВСЕГО. Похожие расчеты в БД делались около 12-14 минут.
Разница составляет 100 раз! 

<p> &nbsp; </p> 

Выборка в 6 потоков из БД исторических данных и укрупнение тафмфрэймов по 165 акциям для генерации 11 таймфрэймов: 
![Выборка в 6 потоков из БД по 165 акциям для генерации 11 таймфрэймов](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/th_6__gen_tf.png)

<p> &nbsp; </p> 

Выборка в 12 потоков из БД исторических данных и укрупнение тафмфрэймов по 165 акциям для генерации 11 таймфрэймов: 
![Выборка в 12 потоков из БД по 165 акциям для генерации 11 таймфрэймов](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/th_12__gen_tf.png)

Отличия между 6 и 12 потоков есть, но не очень значительная -  2.85 и 3.04 секунды, что составляет чуть менее 7%. 
<p> &nbsp; </p> 
<p> &nbsp; </p> 



----

 **Выборка из БД и расчет индикаторов** (полный аналог ночному коду в БД, который работает 15 минут). Однопоточный режим в Си программе. Выборка по 165 акциям для генерации всех таймфрэймов: 
![Однопоточный режим, выборка и расчеты.](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/extren_start_01.png)

То есть в однопоточном режиме программы Си, ВСЕ расчёты занимают примерно 17 секунд. В БД те же самые расчеты занимают от 14 до 15 минут. Итого, чистая разница в
производительности составляет для ОДНОПОТОЧНОГО режима более 50 раз ! (17 секунд и от 14 минут).

<p> &nbsp; </p> 


**Выборка из БД и расчет индикаторов** (полный аналог ночному коду в БД, который работает 15 минут). Запуск 6 потоков в Си программе. Выборка по 165 акциям для генерации всех таймфрэймов: 
![6 потоков. выборка и расчеты](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/extren_start_06.png)

Для 6-ти потоков эта разница уже составляет более 135 раз (6.2 сек и от 14 минут).

<p> &nbsp; </p> 

**Выборка из БД и расчет индикаторов** (полный аналог ночному коду в БД, который работает 15 минут). Запуск 12 потоков в Си программе. Выборка по 165 акциям для генерации всех таймфрэймов: 
![12 потоков. выборка и расчеты](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/extren_start_12.png)

Для 12-ти потоков эта разница уже составляет более 155 раз (5.4 секунды и более 14 минут). 14*60/5.4 = 155.\
В боее масштабных вычислениях, отличия между 6 и 12 потоками уже больше и составляет около 14%. Отличия достигают почти 1 секунды, но вот нагрузка на ноутбук при этом возрастает в разы и в 12 поточном режиме - всегда срабатывает ограничитель нагрузки ПК. В общем - скорость быстрее, но стоит ли 1 секунда разницы перегрева машины - это вопрос. 

<p> &nbsp; </p> 

**Итого – ЗАДАЧА №1 выполнена очень успешно. Реальное время сократилось до 5.5 секунд при старте программы вместо прежних 14 минут! Это явный прогресс.**

<p> &nbsp; </p> 

**Выборка из БД и расчет индикаторов**, код, который работает 15 минут. Выборка по 165 акциям для генерации всех таймфрэймов: 
![Выборка и расчеты в самой БД](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/db_viborka.png)

В разное время суток скорость выполнения одного и того же кода в БД отличается. Так в ночное время код по полной предстартовой выборке и расчётам занимает от 13м 30с до 14м 20с. В то же время в дневное время, этот же код уже работает около 15м 30с. Поэтому цифры в описании могут расходиться. 
<p> &nbsp; </p> 
<p> &nbsp; </p> 



----

## Некоторые нюансы:

Экстренная нагрузка - нагрузка ноутбука во время старта программы при использовании всех "6+6 ядер". Как можно увидеть - нагрузка зашкаливает и иногда программа (блок защиты от перегрева) дважды повышает уровень "коэффициента тормоза"  с 1 до 15 единиц (15 * 1/100 млн).

Графики перегрева ноутбука:
![Экстренная нагрузка - нагрузка ноутбука во время старта программы](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/stop_nagruzka.png)

<p> &nbsp; </p> 

Кроме этого, оказалось, что Си не только быстрый но и очень экономный в ресурсах. На весь проект со всеми наваротами (не публичная часть проекта), уходит всего 120 Мб памяти. PostgreSQL на те же вычисления полностью съедает всю отведённую ему память а это около 5 Гб (это хорошо видно по статистике нагрузки ПК из второго проекта - ссылка чуть ниже) . \
А торговый терминал QUIK потребляет 4 Гб оперативной памяти, работает очень долго и не уверен, что он вообще способен на такие расчеты, не говоря о скорости этих расчётов.

<p> &nbsp; </p> 

Ниже показана схема рабочего проекта взаимодействия компьютеров/блоков в данном проекте: 
![Схема взаимодействия компьютеров/блоков в данном проекте](https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_mmvb_analiz/blob/main/img/schema.png)


Используется два ПК - один Windows, второй Debian 11. Это связано с QUIK терминалом и его невозможностью запустить в полноценном режиме в Linux (даже испольую программу Wine - нет поддержки ODBC протокола). Виртуальные машины так же не подошли - при работе с тиковыми данными в QUIK терминале - они просто подвисают/лагают. Виртуальная машина оказалась критически медленной в моих целях (пробовал 2 самые ходовые). Старый ноутбук 2013 года с родной windows на борту оказался в несколько раз быстрее (ssd диск) чем виртуальная машина с Windows на новом быстром наутбуке на Debian 11 (картинки в папке "img" текущего пректа на github.com).

<p> &nbsp; </p> 


----
### Работа над проектом:

Проект реализовывался с начала сентября 2023 г по 6-8 часов в день. Это был второй мой крупный проект. Открытая часть проекта была закончена в середине ноября 2023. Дальше шли работы над закрытой частью проекта и изменения, исправления ошибок оттуда вносились и в этот открытый проект.

Проект на Си считаю средней сложности (3.5 из 5 звезд сложности). При этом с БД PostgreSQL уже хорошо и плотно работал более 7 лет. С HTML версткой работал уже лет 20. С Питоном года 4. \
Ознакомится со вторым проектом №2:  <a href="https://github.com/OTUS-2023-C01-SARTAKOV-AP/proekt_nagruzka_PC" title="Ускорение работы БД">"Нагрузка ПК с журналированием в БД PostgreSQL и выводом графиков на сайте".</a> 

При работе в проекте использовались PostgreSQL, pl/pgsql, SQL, Python, HTML + сервер-сайта Apache2, Си.

**На частый вопрос** - зачем делать принудительные задержки в коде,  отвечаю: эти ограничения срабатывают примерно в 0,1% от всего времени работы кода. При этом, в обычном режиме нагрузка на процессор около 3-6%, а температура первого ядра процессора 40-42 градуса цельсия. То есть такой странный способ практически мгновенно охлаждает ноутбук от длительного перегрева.
К тому же - основной цикл программы длится 0.20-0.36 сек, что в разы меньше желаемых 5 секунд на полный цикл. Я даже позволил себе сделать задержку в конце каждого цикла, чтобы главный цикл длился 0.5 секуды, т.к быстрее не нужно (для моих задач - смысла быстрее нет никакого), а железо при этом работает практически в режиме простоя. 
Хочу сразу сказать, что БД PostgreSQL 15 в таком же режиме работы, на том же ноутбуке, нагревает его до 56-70 градусов, и создаёт нагрузку около 17%. 

PS историческая справка: в обычном, бесконечном цикле перерасчетов индикаторов на основе БД Cache InterSystems (2016 год) код работал около 5 секунд на 1 полный цикл на 4 акции, то есть 165 акций уже около 206 секунд. **При этом код уже был вручную разделен на 6 потоков**.

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



