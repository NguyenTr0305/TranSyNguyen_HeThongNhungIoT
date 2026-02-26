#include <stdint.h>
#include <stm32f401re_gpio.h>
#include <stm32f401re_rcc.h>

#define LOW                     0
#define BTN_PRESS               LOW

#define GPIO_PIN_SET            1
#define GPIO_PIN_RESET          0

// LED PC9
#define BUZZER_GPIO_PORT           GPIOC
#define BUZZER_GPIO_PIN            GPIO_Pin_9
#define BUZZER_PIN9                9
#define BUZZERControl_SetClock     RCC_AHB1Periph_GPIOC

// BUTTON PB4
#define BUTTON_GPIO_PORT        GPIOB
#define BUTTON_GPIO_PIN         GPIO_Pin_4
#define BUTTON_PIN4			    4
#define BUTTONControl_SetClock  RCC_AHB1Periph_GPIOB

void delay(void) {
    for (uint32_t i = 0; i < 500000; i++);
}

/* ================= LED INIT ================= */
static void Buzzer_init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable clock GPIOC
    RCC_AHB1PeriphClockCmd(BUZZERControl_SetClock, ENABLE);

    //Choose Pin Buzzer
    GPIO_InitStructure.GPIO_Pin   = BUZZER_GPIO_PIN;
    //Choose Pin Buzzer as Out
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    //Choose Speed Buzzer
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //Select Type
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //Select Status
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    //Initalizes all of the above values
    GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);
}

/* ================= BUTTON INIT ================= */
static void Button_init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable clock GPIOC
    RCC_AHB1PeriphClockCmd(BUTTONControl_SetClock, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = BUTTON_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;   // Pull-up

    GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStructure);
}

/* ================= LED CONTROL ================= */
static void BuzzerControl_SetStatus(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint8_t Status) {

	if (Status == GPIO_PIN_SET) {
	        GPIOx->BSRRL = GPIO_PIN;
	    }
	 else {
	        GPIOx->BSRRH = GPIO_PIN;
	    }
}

/* ================= BUTTON READ ================= */
static uint8_t ButtonRead_Status(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN) {
	uint32_t Read_Pin;
    Read_Pin = (GPIOx->IDR) >> GPIO_PIN;
	Read_Pin = Read_Pin & 0x01;

	return Read_Pin;
}
int main(void) {
    uint8_t buzzer_status = 0;
    uint8_t button_state;
    uint8_t last_button_state = 1;   // Pull-up

    Buzzer_init();
    Button_init();

    while (1)
    {
        button_state = ButtonRead_Status(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
        // Phát hiện cạnh nhấn (1 → 0)
        if ((button_state == BTN_PRESS) && (last_button_state == 1)){
        	delay();
            buzzer_status = !buzzer_status;   // đảo trạng thái
            if (buzzer_status){
               BuzzerControl_SetStatus( BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_SET);
            }
            else{
                BuzzerControl_SetStatus( BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, GPIO_PIN_RESET);
            }
        }
        last_button_state = button_state;
    }
}
