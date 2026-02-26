#include <stdint.h>
#include <stm32f401re_gpio.h>
#include <stm32f401re_rcc.h>

#define LOW                     0
#define BTN_PRESS               LOW

#define GPIO_PIN_SET            1
#define GPIO_PIN_RESET          0

// LEDRED PB13
#define LEDRED_GPIO_PORT           GPIOB
#define LEDRED_GPIO_PIN            GPIO_Pin_13
#define LEDRED_PIN13               13
#define LEDREDControl_SetClock     RCC_AHB1Periph_GPIOB

// BUTTON PB4
#define BUTTON_GPIO_PORT        GPIOB
#define BUTTON_GPIO_PIN         GPIO_Pin_3
#define BUTTON_PIN13			3
#define BUTTONControl_SetClock  RCC_AHB1Periph_GPIOB

void delay(void) {
    for (uint32_t i = 0; i < 500000; i++);
}

/* ================= LED INIT ================= */
static void Ledred_init(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable clock GPIOC
    RCC_AHB1PeriphClockCmd(LEDREDControl_SetClock, ENABLE);

    //Choose Pin LEDRED
    GPIO_InitStructure.GPIO_Pin   = LEDRED_GPIO_PIN;
    //Choose Pin LEDRED as Out
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    //Choose Speed LEDRED
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //Select Type
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    //Select Status
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    //Initalizes all of the above values
    GPIO_Init(LEDRED_GPIO_PORT, &GPIO_InitStructure);
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
static void LedredControl_SetStatus(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint8_t Status) {

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
int main(void)
{
    uint8_t led_status = 0;
    uint8_t button_state;
    uint8_t last_button_state = 1;   // Pull-up

    Ledred_init();
    Button_init();

    while (1){
        button_state = ButtonRead_Status(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
        // Phát hiện cạnh nhấn (1 -> 0)
        if ((button_state == BTN_PRESS) && (last_button_state == 1)){
            delay();  // chống rung

            led_status = !led_status;  // đảo trạng thái

            if (led_status){
               LedredControl_SetStatus(LEDRED_GPIO_PORT, LEDRED_GPIO_PIN, GPIO_PIN_SET);
            }
            else{
                LedredControl_SetStatus(LEDRED_GPIO_PORT, LEDRED_GPIO_PIN,GPIO_PIN_RESET);
            }
        }
        last_button_state = button_state;
    }
}
