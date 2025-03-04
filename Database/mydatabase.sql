-- Создаем таблицу для ссылок
CREATE TABLE IF NOT EXISTS links (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    keyword TEXT NOT NULL UNIQUE,
    open_link TEXT,
    search_in_link TEXT
);

-- Создаем таблицу для программ
CREATE TABLE IF NOT EXISTS programs (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    keyword TEXT NOT NULL UNIQUE,
    command TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS logic (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    keyword TEXT NOT NULL UNIQUE
);

-- Вставляем начальные данные в таблицу links
INSERT INTO links (keyword, open_link, search_in_link) VALUES
    ('ютуб', 'https://youtube.com', 'https://youtube.com/results?search_query='),
    ('гугл', 'https://google.com', 'https://google.com/search?q=');

-- Вставляем начальные данные в таблицу programs
INSERT INTO programs (keyword, command) VALUES
    ('телеграмм', 'telegram-desktop'),
    ('браузер', 'firefox');

INSERT INTO logic (keyword) VALUES
    ('открыть'),
    ('найти'),
    ('найди');

COMMIT;

