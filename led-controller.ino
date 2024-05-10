
const unsigned long led_ctl_blinkRate = 300;
unsigned long led_ctl_lastRenderTime = 0;
bool led_ctl_on = true;

void led_ctl_animateBlink(State score) {
  unsigned long currentTime = millis();
  if (currentTime - led_ctl_lastRenderTime >= led_ctl_blinkRate) {
    if (led_ctl_on) {
      std_led_setState(score);
      addr_led_setState(score);
      led_ctl_on = false;
    } else {
      std_led_turn_off();
      addr_led_off();
      led_ctl_on = true;
    }
    led_ctl_lastRenderTime = millis();
  }
}