# HTTP Proxy Learning System

Обучающая система HTTP-прокси - курсовая работа на C++17 с использованием Qt 6.

## Технический стек

- **Язык**: C++17
- **Фреймворк**: Qt 6 (Widgets, SQL)
- **Сборка**: qmake (.pro файлы)
- **База данных**: PostgreSQL
- **ОС**: Linux

## Структура проекта

```
HttpProxyCourse/
├── CourseProject.pro      # Конфигурация qmake
├── data/                  # SQL скрипты и данные
│   └── schema.sql        # Схема базы данных
├── src/                   # Исходный код
│   ├── main.cpp          # Точка входа приложения
│   └── db/               # Слой работы с БД
│       ├── DatabaseManager.h
│       └── DatabaseManager.cpp
├── tests/                 # Тесты
│   └── test_db_connection.cpp
└── bin/                   # Скомпилированные бинарники
```

## Сборка и запуск

### Предварительные требования

1. Qt 6 с модулями: core, gui, widgets, sql
2. PostgreSQL с драйвером QPSQL
3. Компилятор с поддержкой C++17

### Установка зависимостей (Ubuntu/Debian)

```bash
sudo apt-get install qt6-base-dev qt6-tools-dev qmake6 build-essential
sudo apt-get install postgresql postgresql-contrib libpq-dev libqt6sql6-psql
```

### Настройка базы данных

```bash
# Запуск PostgreSQL
sudo service postgresql start

# Создание базы данных
sudo -u postgres createdb course_db

# Выполнение SQL схемы
sudo -u postgres psql -d course_db -f data/schema.sql
```

### Компиляция

```bash
qmake6 CourseProject.pro
make
```

### Запуск

```bash
./bin/CourseProject
```

## Реализованные компоненты

### Сессия №1 ✅

- [x] Структура проекта с необходимыми директориями
- [x] Конфигурация qmake с модулями Qt и C++17
- [x] SQL схема с таблицами `users` и `study_progress`
- [x] DatabaseManager как Singleton с подключением к PostgreSQL
- [x] Тестирование подключения к БД
- [x] Успешная компиляция и тестирование

### Планируемые сессии

- **Сессия №2**: GUI интерфейс и система аутентификации
- **Сессия №3**: HTTP-прокси функциональность
- **Сессия №4**: Обучающие модули и тестирование

## База данных

### Таблица `users`
- `id` - уникальный идентификатор (SERIAL PRIMARY KEY)
- `login` - логин пользователя (TEXT UNIQUE)
- `password_hash` - хеш пароля (TEXT)
- `role` - роль пользователя (TEXT, по умолчанию 'student')
- `created_at` - время создания (TIMESTAMP)

### Таблица `study_progress`
- `user_id` - ID пользователя (INT FK)
- `chapter_id` - ID главы (INT)
- `status` - статус изучения (TEXT, по умолчанию 'not_started')
- `last_score` - последний результат (INT, по умолчанию 0)
- `updated_at` - время обновления (TIMESTAMP)

## Автор

Курсовая работа по дисциплине "Программирование"