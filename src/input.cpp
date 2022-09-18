#include "input.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int Input::GetTimeSinceLastInput()
{
  LASTINPUTINFO last_input_info {};
  last_input_info.cbSize = sizeof(LASTINPUTINFO);
  last_input_info.dwTime = 0;

  if (!GetLastInputInfo(&last_input_info)) {
    return -1;
  }

  int ticks = GetTickCount();
  int idle_ms = ticks - last_input_info.dwTime;

  return idle_ms;
}
