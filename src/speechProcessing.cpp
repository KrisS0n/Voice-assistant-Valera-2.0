#include "constants.h"
#include "speechProcessing.h"


Microphone::Microphone() : stream(nullptr)
{
    PaError err = Pa_Initialize();
    if (err != paNoError)
    {
        throw std::runtime_error(ERROR_PORTAUDIO_INIT + std::string(Pa_GetErrorText(err)));
    }

    err = Pa_OpenDefaultStream(&stream, 1, 0, paInt16, SAMPLE_RATE, FRAMES_PER_BUFFER, nullptr, nullptr);
    if (err != paNoError)
    {
        Pa_Terminate();
        throw std::runtime_error(ERROR_PORTAUDIO_STREAM + std::string(Pa_GetErrorText(err)));
    }

    err = Pa_StartStream(stream);
    if (err != paNoError)
    {
        Pa_CloseStream(stream);
        Pa_Terminate();
        throw std::runtime_error(ERROR_PORTAUDIO_STREAM + std::string(Pa_GetErrorText(err)));
    }
}

Microphone::~Microphone()
{
    if (stream)
    {
        Pa_StopStream(stream);
        Pa_CloseStream(stream);
        Pa_Terminate();
    }
}

void Microphone::readAudio(int16_t *buffer, int bufferSize)
{
    PaError err = Pa_ReadStream(stream, buffer, bufferSize);
    if (err != paNoError)
    {
        throw std::runtime_error(ERROR_PORTAUDIO_READ + std::string(Pa_GetErrorText(err)));
    }
}

//

SpeechRecognizer::SpeechRecognizer(const std::string &modelPath)
{
    model = vosk_model_new(modelPath.c_str());
    if (!model)
    {
        throw std::runtime_error(ERROR_VOSK_MODEL);
    }

    recognizer = vosk_recognizer_new(model, SAMPLE_RATE);
    if (!recognizer)
    {
        vosk_model_free(model);
        throw std::runtime_error(ERROR_VOSK_RECOGNIZER);
    }
}

SpeechRecognizer::~SpeechRecognizer()
{
    if (recognizer)
    {
        vosk_recognizer_free(recognizer);
    }
    if (model)
    {
        vosk_model_free(model);
    }
}

std::string SpeechRecognizer::recognize(const int16_t *buffer, int bufferSize)
{
    if (vosk_recognizer_accept_waveform(recognizer,
                                        reinterpret_cast<const char *>(buffer),
                                        bufferSize * sizeof(int16_t)))
    {
        const char *result = vosk_recognizer_result(recognizer);
        return result;
    }
    return "";
}

//

VoiceAssistant::VoiceAssistant(const std::string &modelPath, const std::string &dbName)
    : microphone(), recognizer(modelPath), db(dbName), handler(db), isActive(false), voiceGuidance(),
      lastCommandTime(std::chrono::steady_clock::now())
{
}

void VoiceAssistant::run()
{
    int16_t buffer[AUDIO_BUFFER_SIZE];

    std::cout << "Listening... \n";
    while (true)
    {
        microphone.readAudio(buffer, AUDIO_BUFFER_SIZE);

        std::string result = recognizer.recognize(buffer, AUDIO_BUFFER_SIZE);
        if (!result.empty())
        {
            std::cout << "Recognized: " << result << "\n";

            Json::Value root;
            Json::CharReaderBuilder reader;
            std::string errs;
            std::istringstream s(result);
            if (Json::parseFromStream(reader, s, &root, &errs))
            {
                if (root.isMember("text"))
                {
                    std::string text = root["text"].asString();
                    std::cout << "Text: " << text << "\n";

                    if (!text.empty())
                    {
                        // Обработка команд
                        if (!isActive && text.find(ENABLING) != std::string::npos)
                        {
                            voiceGuidance.voiceGuidance("Настало мое время");
                            isActive = true;
                            lastCommandTime = std::chrono::steady_clock::now();
                        }
                        else if (isActive && text.find(SHUTDOWN) != std::string::npos)
                        {
                            voiceGuidance.voiceGuidance("Прощай");
                            isActive = false;
                        }
                        else if (isActive)
                        {
                            handler.handleCommand(text);
                            lastCommandTime = std::chrono::steady_clock::now();
                        }
                    }
                }
            }
        }

        if (isActive)
        {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastCommandTime).count();

            if (elapsed >= TIME_PASSIV_MODE)
            {
                std::cout << "No command for " << TIME_PASSIV_MODE << " seconds. Deactivating...\n";
                isActive = false;
            }
        }
    }
}
