void setupLight()
{
  pinMode(LED_PIN, OUTPUT);
  lightState = LIGHT_STATE_OFF;
  powerOn = false;
  isFadeMode = false;
  setLedPWM(0);
}

void lightOffAfterTime()
{
  if (lightTimeOff == 0) {
    return;
  }

  if (currentMillis - lightMillis <= lightTimeOff) {
    return;  
  }

  if (isFadeMode) {
    lightFadeOutStart(config.fadeTime);
  } else {
    lightOff();  
  }
  lightTimeOff = 0;
}

void lightOnWithTimeOff(int sec)
{
  lightTimeOff = 1000 * sec;
  lightMillis = currentMillis;
  lightOn();
}

void lightToggle()
{
  if (powerOn) {
    lightOff();
    return;
  }

  lightOn();
}

void lightOff()
{
  lightOff(false);
}

void lightOff(bool isFadeModeTmp)
{
  lightState = LIGHT_STATE_OFF;
  powerOn = false;
  isFadeMode = false;
  setLedPWM(0);
  
  publishResult();
}

void lightOn()
{
  lightOn(false);
}

void lightOn(bool isFadeModeTmp)
{
  lightState = LIGHT_STATE_ON;
  powerOn = true;
  isFadeMode = isFadeModeTmp;
  setLedPWM(config.light);
  
  publishResult();
}

void lightPWM(uint16_t value)
{
  if (0 == value) {
    lightState = LIGHT_STATE_OFF;
    powerOn = false;
    isFadeMode = false;
  } else {
    lightState = LIGHT_STATE_ON;
    powerOn = true;
    isFadeMode = false;
  }
  setLedPWM(value);

  publishResult();
}

void lightToggleFade(uint16_t fadeTime)
{
  if (powerOn) {
    lightFadeOutStart(fadeTime);
    return;
  }

  lightFadeInStart(fadeTime);
}


void lightFadeInStart(uint16_t fadeTime)
{
  if(0 == fadeTime) {
    lightOn(true);
    return;
  }
  
  lightState = LIGHT_STATE_FADE_IN;
  powerOn = true;
  fadeValue = 0;
  fadeStep = (float)config.light / ((float)fadeTime * 10.0);
  isFadeMode = true;
  publishResult();
}

void lightFadeOutStart(uint16_t fadeTime)
{
  if(0 == fadeTime) {
    lightOff(true);
    return;
  }
  
  lightState = LIGHT_STATE_FADE_OUT;
  powerOn = true;
  fadeValue = config.light;
  fadeStep = (float)config.light / ((float)fadeTime * 10.0);
  isFadeMode = true;
  publishResult();
}


void lightFadeInLoop()
{
  fadeValue += fadeStep;
  int light = fadeValue;
  if (light >= config.light) {
    lightOn(true);
    return;
  }
  
  setLedPWM(light);
}

void lightFadeOutLoop()
{
  fadeValue -= fadeStep;
  int light = fadeValue;
  if (light <= 0) {
    lightOff(true);
    return;
  }
  
  setLedPWM(light);
}

void setLedPWM(uint16_t value)
{
  currentLight = value;
  if (config.pwmReverse) {
    analogWrite(LED_PIN, 1023 - value);
    return;
  }
  
  analogWrite(LED_PIN, value);
}
