#ifndef AKINATOR_SPEAKER_H
#define AKINATOR_SPEAKER_H

const int MAX_SPEECH_BUFFER_SIZE = 2000;

const char* const AKINATOR_SPEAK_COMMAND = "PowerShell -Command \""
                            "Add-Type -AssemblyName System.Speech; "
                            "$speech = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
                            "$speech.Speak('%s');\"";


void speakFlush();

void speak(const char* format, ...);

#endif //AKINATOR_SPEAKER_H