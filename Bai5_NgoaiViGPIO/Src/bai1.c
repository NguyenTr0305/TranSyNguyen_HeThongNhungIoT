#include <stdint.h>
#include <stm32f401re_gpio.h>
#include <stm32f401re_rcc.h>

#define LOW                     0
#define BTN_PRESS               LOW

#define GPIO_PIN_SET            1
#define GPIO_PIN_RESET          0

// LED PA5
#define LED_GPIO_PORT           GPIOA
#define LED_GPIO_PIN            GPIO_Pin_5
#define LED_PIN5				5
#define LEDControl_SetClock     RCC_AHB1Periph_GPIOA

// BUTTON PC13
#define BUTTON_GPIO_PORT        GPIOC
#define BUTTON_GPIO_PIN         GPIO_Pin_13
#define BUTTON_PIN13			13
#define BUTTONControl_SetClock  RCC_AHB1Periph_GPIOC

void delay(void) {
    for (uint32_t i = 0; i < 500000; i++);
}

/* ================= LED INIT ================= */
static void Led_init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable clock GPIOA
    RCC_AHB1PeriphClockCmd(LEDControl_SetClock, ENABLE);

    //Choose Pin Led
    GPIO_InitStructure.GPIO_Pin   = LED_GPIO_PIN;
    //Choose Pin Led as Out
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    //Choose Speed Pin
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //Select Type
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //Select Status
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    //Initalizes all of the above values
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
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
static void LedControl_SetStatus(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint8_t Status) {

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

/* ================= MAIN ================= */
int main(void) {

    uint8_t led_status = 0;
    uint8_t button_state;
    uint8_t last_button_state = 1;   // Do dùng Pull-up

    Led_init();
    Button_init();

    while (1) {

        button_state = ButtonRead_Status(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);

        if ((button_state == BTN_PRESS) && (last_button_state == 1)) {
            delay();
            led_status = !led_status;

            if (led_status) {
                LedControl_SetStatus(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
            }
            else {
                LedControl_SetStatus(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
            }
        }

        last_button_state = button_state;
    }
}
