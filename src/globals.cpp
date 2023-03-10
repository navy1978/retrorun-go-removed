/*
retrorun-go2 - libretro frontend for the ODROID-GO Advance
Copyright (C) 2020  OtherCrashOverride
Copyright (C) 2021-present  navy1978

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "globals.h"
#include <cstring>
#include <string>


// #include <cstring>
const char *OS_ARCH = "cat /storage/.config/.OS_ARCH";
static const int DEVICE_NAME_SIZE = 1024;
static char DEVICE_NAME[DEVICE_NAME_SIZE];
static bool deviceInitialized = false;

std::string release = "2.0.1";

RETRORUN_CORE_TYPE Retrorun_Core = RETRORUN_CORE_UNKNOWN;
Device device = UNKNOWN;
Resolution resolution = R_UNKNOWN;
bool force_left_analog_stick = true;

bool opt_triggers = false;
bool gpio_joypad = false;
float opt_aspect = 0.0f;
float aspect_ratio = 0.0f;

std::string romName;
std::string coreName;

std::string screenShotFolder;

float fps = 0.0f;
float originalFps = 0.0f;
float newFps = 0.0f;
int retrorunLoopCounter = 0;
int retrorunLoopSkip = 15; // 10 ?

int frameCounter = 0;
int frameCounterSkip = 4;

int audioCounter = 0;
int audioCounterSkip = 6;

bool processVideoInAnotherThread = true;

bool adaptiveFps = false;

bool runLoopAt60fps = true;

int retrorun_audio_buffer = -1; // means it will be fixed to a value related with the original FPS of the game

int retrorun_mouse_speed_factor = 5;

const char *getEnv(const char *tag) noexcept
{
    const char *ret = std::getenv(tag);
    return ret ? ret : "";
}

bool isFlycast()
{
    return coreName == "Flycast";
}

bool isParalleln64()
{
    return coreName == "ParaLLEl N64";
}

bool isSwanStation()
{
    return coreName == "SwanStation";
}

bool isMGBA()
{
    return coreName == "mGBA";
}

bool isVBA()
{
    return coreName == "VBA-M";
}

bool isJaguar()
{
    return coreName == "Virtual Jaguar";
}

bool isDosBox()
{
    return coreName == "DOSBox-pure";
}

bool isDosCore()
{
    return coreName == "DOSBox-core";
}

bool isBeetleVB()
{
    return coreName == "Beetle VB";
}

bool isMame(){
    return coreName.find("MAME") != std::string::npos ;
}


const char *getDeviceName() noexcept
{

    if (!deviceInitialized)
    {
        FILE *pipe = popen(OS_ARCH, "r");
        if (!pipe)
        {
            printf("-RR- Error: Could not open pipe to `cat` command.\n");
            return "";
        }

        char *result = fgets(DEVICE_NAME, DEVICE_NAME_SIZE, pipe);
        if (!result)
        {
            printf("-RR- Error: Could not read output from `cat` command.\n");
            return "";
        }

        // Close the pipe
        pclose(pipe);
        deviceInitialized = true;
        printf("-RR- Device name: %s\n", DEVICE_NAME);
    }
    return DEVICE_NAME;
}

bool isRG351M()
{
    return strcmp(DEVICE_NAME, "RG351M\n") == 0;
}
bool isRG351P()
{
    return strcmp(DEVICE_NAME, "RG351P\n") == 0;
}
bool isRG351V()
{
    return strcmp(DEVICE_NAME, "RG351V\n")==0 ;
}
bool isRG351MP()
{
    return strcmp(DEVICE_NAME, "RG351MP\n") == 0;
}
bool isRG552()
{
    return strcmp(DEVICE_NAME, "RG552\n") == 0;
}
