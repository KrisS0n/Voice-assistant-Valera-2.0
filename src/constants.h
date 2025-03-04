#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Путь к модели Vosk и sql
const std::string MODEL_PATH = "../vosk-model-small-ru-0.22";
const std::string DB_NAME = "../Database/mydatabase.db";

// Поиск в браузере
const std::string XDG = "xdg-open ";


// Слова для включение и выключение
const std::string SHUTDOWN = "пока";
const std::string ENABLING = "валера";

// Время для перехода в пассивный режим
const int TIME_PASSIV_MODE = 5;

// Параметры аудио
const int SAMPLE_RATE = 16000;
const int FRAMES_PER_BUFFER = 512;
const int AUDIO_BUFFER_SIZE = 4096;

// Ключи JSON
const std::string JSON_KEY_TEXT = "text";

// Сообщения об ошибках
const std::string ERROR_PORTAUDIO_INIT = "PortAudio initialization failed: ";
const std::string ERROR_PORTAUDIO_STREAM = "PortAudio stream opening failed: ";
const std::string ERROR_PORTAUDIO_READ = "PortAudio read failed: ";
const std::string ERROR_VOSK_MODEL = "Failed to load Vosk model";
const std::string ERROR_VOSK_RECOGNIZER = "Failed to create Vosk recognizer";

#endif // CONSTANTS_H
