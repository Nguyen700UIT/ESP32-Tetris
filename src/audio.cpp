#include <audio.h>

u_int16_t tempo = 180;
TaskHandle_t musicTask = NULL;

int melody[] = {
  
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,
  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,
  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  
  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  REST,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,
};

void playMusic()
{
    int wholeNote = (60000 * 4) / tempo;
    int noteAmount = sizeof(melody) / sizeof(melody[0]) / 2; //So luong note


    for (int currNode = 0; currNode < noteAmount * 2; currNode += 2)
    {
        if (gameOverFlag)
        {
            ledcWriteTone(PWM_CHANNEL_BUZZER, 0);
            return;
        }

        int divider = melody[currNode + 1]; //Lay nhip dieu
        int noteDuration = 0;
        if (divider > 0) noteDuration = wholeNote / divider; //Normal note
        else if (divider < 0) noteDuration = (wholeNote / abs(divider) ) * 1.5;

        ledcWriteTone(PWM_CHANNEL_BUZZER, melody[currNode]);
        //Fade in
        for (int d = 0; d <= 60; d += 5)
        {
            ledcWrite(PWM_CHANNEL_BUZZER, d);
            vTaskDelay(5 / portTICK_PERIOD_MS);
        }

        

        vTaskDelay((noteDuration * 0.9) / portTICK_PERIOD_MS); //Chay note nhac 90%

        //Fade out
        for (int d = 60; d >= 0; d -= 5)
        {
            ledcWrite(PWM_CHANNEL_BUZZER, d);
            vTaskDelay(5 / portTICK_PERIOD_MS);
        }

        ledcWriteTone(PWM_CHANNEL_BUZZER, 0); //Dung note nhac

        vTaskDelay((noteDuration * 0.1) / portTICK_PERIOD_MS); //Note nghi 10%
        
    }
}

void audioTask(void *pvParameters)
{
    //Buzzer PWM
    ledcSetup(PWM_CHANNEL_BUZZER, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(BUZZER, PWM_CHANNEL_BUZZER);
    
    while(true)
    {
        if(!gameOverFlag)
        {
            playMusic();
        }
        else
        {
            ledcWriteTone(PWM_CHANNEL_BUZZER, 0); //Tat nhac
            vTaskDelay(200 / portTICK_PERIOD_MS);
        }
    }
}

void initAudio()
{
    xTaskCreatePinnedToCore(audioTask, "MusicTask", 2048, NULL, 2, &musicTask, 0);
}