Розробити на Qt Widgets віджет типу TreeView.

При наведені на дочірній елемент (вкладений) з’являються кнопка “+” (на картинці “x” замість неї). При натисканні спрацьовує пустий слот з параметрами int mcc, int mnc. 
Для view використовувати QTreeView. Батьківський елемент Країна (ім’я з таблиці country). Іконки країн можна взяти [тут](https://drive.google.com/a/ozglob.com/file/d/1KzLfJHOd2i0p_fU-iwe_FpAr8b9LqvDc/view?usp=sharing)

Його дочерні елементи - оператори цієї країни (таблиця operators)
Для відображення іконок використовувати клас QStyledItemDelegate. Іконки зберігаються у папці Icons. 
Назву іконки оператора в форматі MCC_MNC.png. Іконки операторів можна взяти [тут](https://drive.google.com/open?id=1PaOCM9zxeDyHA4WGtd7ysrC4we9elu-W) 
Дані в модель підтягуються з SQLite бази, приклад бази [тут](https://drive.google.com/a/ozglob.com/file/d/1JVvlHALcBBdch74-tXGrM6d4MgoRKBZB/view?usp=sharing). 

Таблиця operators. Країна визначається по параметру mcc. Зовнішній ключ на таблицю countries. З countries брати ім’я країни. Шлях до бази задається в конструкторі. Вважаємо, що завжди буде зберігатись поряд з exe, з ім’ям system.db
Фільтрація, сортування - не потрібні.
За подвійним кліком на вкладений елемент відкривається вікно редактора цього елемента.
Параметри mcc, mnc - заблоковані для зміни. Змінити можна тільки Name.
При натисканні save дані зберігаються в моделі. View апдейтить конкретний рядок. Також вiдразу записує зміни у БД.

При натисканні на “+” (внизу віджета) з’являється таке ж вікно, як і для редагування оператора, тільки з незаповненими полями. В даному випадку доступні для редагування всі поля.

Після заповнення знаки запитання змінюються на іконки, якщо така найдена. При натисканні  save - апдейтим view. Додаєм запис у базу.
