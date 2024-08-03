# Spiski
Reverse engineering конкурсных списков поступления на бюджет в вузы России.

В пограничных случаях — когда документы уже сданы, но непонятно, проходишь ли по конкурсу — официальная рекомендация звучит так: «Смотрите конкурсные списки». Проблема в том, что в этих конкурсных списках невозможно ничего однозначно понять.

## Результат
Общая таблица распределения абитуриентов по направлениям с учётом количества мест на каждом направлении. Можно выяснять, кто куда попал. Можно подводить самую разную статистику.

## Что делает программа
Чтобы понять, проходит ли конкретный абитуриент на бюджет, нужно распределить по направлениям всех, кто выше его по баллам.

Общий алгорифм такой.
1. Собираем данные по направлениям, на которые можно подать заявление. Нужны: id направления (нпр., код специальности), количество доступных бюджетных мест.
2. Собираем все заявления. По каждому заявлению: id кандидата (СНИЛС), направление, приоритет, баллы на это направление.
3. Сортируем список заявлений: по убыванию баллов, потом по id кандидатов, потом по приоритетам. Так мы получим очередь кандидатов, где каждая следующая запись — это либо новый человек с меньшими баллами, либо следующий по важности приоритет того же человека. Важно: люди не группируются в таком списке, потому что у одного и того же человека на разные направления могут быть разные баллы.
4. Проходим очередь от самых сильных заявлений к самым слабым.
5. Для каждого следующего заявления смотрим, можно ли его удовлетворить. Можно, если человек ещё никуда не записан, и на направлении, указанном в заявлении, ещё есть места.
  1. Если да — записываем человека на направление, и уменьшаем количество доступных мест на этом направлении.
  2. Если нет — выбрасываем заявление (знаем, что для этого человека могут сработать следующие заявления).
6. Доходим до конца списка.

В итоге у нас — список кандидатов, к каждому из которых приписано лучшее из доступных ему направлений. Здесь «лучшее» означает «направление из самого первого заявления, которое удалось удовлетворить, когда мы шли по очереди баллов и приоритетов».

## Как подготовить
* data/ — CSV-файлы с данными: направления + все поданные заявления. Собирать руками или с помощью Selenium, как повезёт конкретно в вашем вузе.
* bash/ — скрипты инициации (можно не трогать).
* sql/ — скрипты создания баз данных; могут отличаться на разных машинах (особенно много проблем с загрузкой данных из файлов).
* c/ — основная логика распределения; в макросах препроцессора — реквизиты доступа к базе данных.

## Как запустить
Точно работает на Linux Mint 22 + MySQL 8 + gcc.
1. Команда make создаст базу с нужными данными и откомпилирует C-программу.
2. Запустить программу — build/main.
3. Результат распределения смотреть в SQL-таблице candidates.

## Ограничения
Все ограничения связаны с одноразовой природой проекта. Он просто не должен работать долго и счастливо.
1. Код не проверялся на утечки памяти.
2. Никакие ошибки времени исполнения никак не обрабатываются.
3. Отдельно обратите внимание, что неявно предполагается консистентность данных. Тут могут быть сюрпризы.
