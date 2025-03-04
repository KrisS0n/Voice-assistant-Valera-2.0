#ifndef SPEECHPROCESSING_H
#define SPEECHPROCESSING_H

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <json/json.h>
#include <portaudio.h>
#include <vosk_api.h>

#include "commandhandler.h"
#include "constants.h"
#include "database.h"
#include "voiceGuidance.h"


// Класс для работы с микрофоном
class Microphone
{
private:
    PaStream *stream;

public:
    Microphone();
    ~Microphone();
    void readAudio(int16_t *buffer, int bufferSize);
};

// Класс для распознавания речи
class SpeechRecognizer
{
private:
    VoskModel *model;
    VoskRecognizer *recognizer;

public:
    SpeechRecognizer(const std::string &modelPath);
    ~SpeechRecognizer();
    std::string recognize(const int16_t *buffer, int bufferSize);
};

// Класс для управления голосовым ассистентом
class VoiceAssistant
{
private:
    Microphone microphone;
    SpeechRecognizer recognizer;
    Database db;
    CommandHandler handler;
    VoiceGuidance voiceGuidance;
    bool isActive;
    std::chrono::steady_clock::time_point lastCommandTime;

public:
    VoiceAssistant(const std::string &modelPath, const std::string &dbName);
    void run();
};

#endif // !SPEECHPROCESSING_H
