# Valera 2.0

### Необходимые библиотеки и их установка для компиляции
```
sudo pacman -S sqlite
sudo pacman -S jsoncpp
sudo pacman -S portaudio
```

```
sudo pacman -S python git make gcc
git clone https://github.com/alphacep/vosk-api
cd vosk-api
cd src
make
sudo cp libvosk.so /usr/local/lib/
sudo cp vosk_api.h /usr/local/include/
sudo ldconfig
```

Добавлена библиотека для прослушивания текста и последующей конвертации ее в текст vosk
### Переходим по ссылке и находим vosk-model-small-ru-0.22
```
https://alphacephei.com/vosk/models
```
### После установки
```
cd Ваши загрузки
7z x vosk-model-small-en-us-0.15.zip
mv /путь/к/исходной/папке /путь/к/проекту
```

### Как добавить новые команды в базу данных
```
cd Valera-2.0/Database/
nvim || vim mydatabase.sql
```
##### Пример как добавить новою команду гитхаб:
```ruby
INSERT INTO links (keyword, open_link, search_in_link) VALUES
    ('ютуб', 'https://youtube.com', 'https://youtube.com/results?search_query='),
    ('гугл', 'https://google.com', 'https://google.com/search?q='),
    ('гитхаб', 'https://github.com/', 'https://github.com/search?q=');
```

После чего сохранить изменения и написать команду
```
sqlite3 mydatabase.db < mydatabase.sql
```

## Если возникли проблемы
```
rm mydatabase.db
sqlite3 mydatabase.db < mydatabase.sql
```


### примечания
Программа запускается по слову «Валера». После этого у вас есть 5 секунд, чтобы сказать команду. Если вы ничего не сказали, программа уходит в спящий режим, и её по-прежнему можно активировать словом «Валера».
