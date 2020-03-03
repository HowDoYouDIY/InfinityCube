#define MAX_BRIGHTNESS  150

bool increaseBrightness = true;

int adjustLedBrightness (int ledBrightness)
{
  if (increaseBrightness) {
    ledBrightness++;
  } else {
    ledBrightness--;
  }

  if (ledBrightness > MAX_BRIGHTNESS) {
    increaseBrightness = false;
  } else if (ledBrightness < 1) {
    increaseBrightness = true;
  }

  return ledBrightness;
}
