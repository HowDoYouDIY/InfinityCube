enum changingColors {
  whiteToBlue,
  blueToGreen,
  greenToRed,
  redToWhite,
  redToBlue,
  blueToRed,
  whiteToRed,
  redToGreen,
  greenToBlue,
  blueToWhite
};

changingColors colorChangeState = whiteToBlue;

void changeLedColor(int color[3], int ledBrightness)
{
  int r = color[0];
  int g = color[1];
  int b = color[2];
  
  switch (colorChangeState) {
    case whiteToBlue:
      r--;
      g--;
      b += 2;

      if ((r <= 0) || (g <= 0)) {
        b = ledBrightness;
      }

      if(b >= ledBrightness) {
        r = 0;
        g = 0;
        colorChangeState = blueToGreen;
      }
      break;
    case blueToGreen:
      r = 0;
      g++;
      b--;

      if ((g >= ledBrightness) || (b <= 0)) {
        g = ledBrightness;
        b = 0;
        colorChangeState = greenToRed;
      }
      break;
    case greenToRed:
      r++;
      g--;
      b = 0;

      if ((r >= ledBrightness) || (g <= 0)) {
        r = ledBrightness;
        g = 0;
        colorChangeState = redToBlue;
      }
      break;
    case redToBlue:
      r--;
      g = 0;
      b++;

      if ((r <= 0) || (b >= ledBrightness)) {
        r = 0;
        b = ledBrightness;
        colorChangeState = blueToRed;
      }
      break;
    case blueToRed:
      r++;
      g = 0;
      b--;

      if ((r >= ledBrightness) || (b <= 0)) {
        r = ledBrightness;
        b = 0;
        colorChangeState = redToWhite;
      }
      break;
    case redToWhite:
      r -= 2;
      g++;
      b++;

      if (r <= ((float)ledBrightness / 3)) {
        r = (float)ledBrightness / 3;
        g = (float)ledBrightness / 3;
        b = (float)ledBrightness / 3;
        colorChangeState = whiteToRed;
      }
      break;
    case whiteToRed:
      r += 2;
      g--;
      b--;

      if ((g <= 0) || (b <= 0)) {
        r = ledBrightness;
      }

      if(r >= ledBrightness) {
        g = 0;
        b = 0;
        colorChangeState = redToGreen;
      }
      break;
    case redToGreen:
      r--;
      g++;
      b = 0;

      if ((r <= 0) || (g >= ledBrightness)) {
        r = 0;
        g = ledBrightness;
        colorChangeState = greenToBlue;
      }
      break;
    case greenToBlue:
      r = 0;
      g--;
      b++;

      if ((g <= 0) || (b >= ledBrightness)) {
        g = 0;
        b = ledBrightness;
        colorChangeState = blueToWhite;
      }
      break;
    case blueToWhite:
      r++;
      g++;
      b -= 2;

      if (b <= ((float)ledBrightness / 3)) {
        r = (float)ledBrightness / 3;
        g = (float)ledBrightness / 3;
        b = (float)ledBrightness / 3;
        colorChangeState = whiteToBlue;
      }
      break;
    default:
      break;
  }
/**
  Serial.print("Red: ");
  Serial.print(r);
  Serial.print("  Green: ");
  Serial.print(g);
  Serial.print("  Blue: ");
  Serial.print(b);
  Serial.print("  Total: ");
  Serial.println(r + g + b);
/**/
      color[0] = r;
      color[1] = g;
      color[2] = b;      
}
