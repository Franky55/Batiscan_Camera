#include "main.h"
#include <stdio.h>
#include "pilote_Camera.h"

#include "esp_camera.h"

#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems

camera_config_t config;
uint8_t pilote_Camera_Buffer[GROSSEUR_MAX_IMAGE];

int pilote_Camera_initialise(void)
{
    config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sccb_sda = 26;
  config.pin_sccb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  //config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return -1;
  }

    return 0;
}


void pilote_Camera_Take_Photo(uint8_t* buffer, int *size)
{
    camera_fb_t * fb = NULL;
    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;
    char * part_buf[64];

    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    *size = fb->len;
    //buffer = pilote_Camera_Buffer;

    for(int i = 0; i < fb->len; i++)
    {
        buffer[i] = fb->buf[i];
        // Serial.print(pilote_Camera_Buffer[i]);
        // Serial.print(", ");
    }

}
