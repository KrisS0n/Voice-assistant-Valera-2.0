#include "constants.h"
#include "speechProcessing.h"

int main()
{
    try
    {
        VoiceAssistant assistant(MODEL_PATH, DB_NAME);
        assistant.run();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return -1;
    }

    return 0;
}
