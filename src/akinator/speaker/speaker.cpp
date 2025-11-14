#include "speaker.h"

#include <cassert>
#include <cstdarg>

#include "akinator.h"


static char speechBuffer[MAX_SPEECH_BUFFER_SIZE] = {};
static size_t bufferPosition = 0;

void speakFlush() {
    if (bufferPosition == 0) {
        return;
    }

    char command[MAX_SPEECH_BUFFER_SIZE * 2] = {};
    snprintf(command, sizeof(command),
             AKINATOR_SPEAK_COMMAND,
             speechBuffer);

    system(command);

    bufferPosition = 0;
    speechBuffer[0] = '\0';
}

void speak(const char* format, ...) {
    assert(format);

    char formatted_text[MAX_INPUT_SIZE] = {};

    va_list args;
    va_start(args, format);
    int chars_written = vsnprintf(formatted_text, MAX_INPUT_SIZE, format, args);
    va_end(args);

    if (bufferPosition + (size_t) chars_written > MAX_SPEECH_BUFFER_SIZE) {
        speakFlush();
    }

    strcat(speechBuffer + bufferPosition, formatted_text);
    bufferPosition += (size_t) chars_written;
}