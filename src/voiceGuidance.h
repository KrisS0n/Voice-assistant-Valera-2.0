#ifndef VOICEGUIDANCE_H
#define VOICEGUIDANCE_H


#include <cstdlib>
#include <string>

#include "constants.h"

class VoiceGuidance
{
private:
    const char *Espeak = "espeak-ng -v ru \"";

public:
    void voiceGuidance(const std::string &text)
    {
        std::string command = Espeak + text + "\"";
        system(command.c_str());
    }
};

#endif // !VOICEGUIDANCE_H
