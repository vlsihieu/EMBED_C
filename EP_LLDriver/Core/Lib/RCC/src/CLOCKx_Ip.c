#include <CLOCKx_Ip.h>
void Config_Clock_HSE_Register(void){
	uint32_t *RCC_CR = (uint32_t *)(RCCx_Address  + 0x00);
	uint32_t *FLASH_ACR = (uint32_t *)(Flashx_Address  + 0x00);
	uint32_t *RCC_CFGR = (uint32_t *)(RCCx_Address  + 0x04);
	// HSE clock enable
	*RCC_CR |= (1 << 16);
	// External high-speed clock ready flag
	while(!(*RCC_CR & (1 << 17)));
	//  Prefetch buffer enable
	*FLASH_ACR |= (1 << 4);
	// Two wait states, if 48 MHz < SYSCLK ≤ 72 MHz
	*FLASH_ACR |= (0b010 << 0);
	//  : HSE divider for PLL entry
	*RCC_CFGR &= ~(1 << 17); // HSE clock not divided
	// : PLL entry clock source
	*RCC_CFGR |= (1 << 16); // HSE oscillator clock selected as PLL input clock
	// PLL multiplication factor
	*RCC_CFGR |= (0b0111 << 18); // PLL input clock x 9
	//  PLL enable
	*RCC_CR |= (1 << 24);
	//  PLL clock ready flag
	while(!(*RCC_CR & (1 << 25)));
	// System clock switch
	*RCC_CFGR |= (0b10 << 0); //   PLL selected as system clock
	while(!(*RCC_CFGR & (0b10 << 2)));
	//*RCC_CFGR |= 0x00000000U;
	// APB high-speed prescaler (APB2)
	*RCC_CFGR |= (0b000 << 11);  //// HCLK not divided HCLK not divided = 72M
	// APB high-speed prescaler (APB1)
	*RCC_CFGR |= (0b100 << 8);  //HCLK divided by 2 = 36M

}
void Config_Clock_HSI_Register(void){
	uint32_t *RCC_CR = (uint32_t *)(RCCx_Address  + 0x00);
	uint32_t *FLASH_ACR = (uint32_t *)(Flashx_Address  + 0x00);
	uint32_t *RCC_CFGR = (uint32_t *)(RCCx_Address  + 0x04);
	// HSI clock enable
	*RCC_CR |= (1 << 0);
	while(!(*RCC_CR & (1 << 1)));
// 2. Config Flash
	*FLASH_ACR |= (1 << 4);                          // Enable Prefetch Buffer

	*FLASH_ACR |= (1<<1);                                    // Flash 2 wait state
	*FLASH_ACR &= ~((1<<2)|(1<<0));
// 3. PREDIV
	*RCC_CFGR &= ~(1 << 17);
// 4. PLL Source Mux
	*RCC_CFGR |= (1 << 16);
// 5. PLL miultipler
	*RCC_CFGR |= (0x5 << 18);
// 6. Enable PLL
	*RCC_CR |= (1 << 24);
  while(!(*RCC_CR & (1 << 25)));

  *RCC_CFGR &= ~(0x3 << 0);
  *RCC_CFGR |= 0x00000002U;
	while(!(*RCC_CFGR & 0x00000008U));
// 7. AHB prescaler
	*RCC_CFGR |= 0x00000000U;
// 8. APB1 prescaler
	*RCC_CFGR |= 0x00000400U;
// 9. APB2 prescaler
	*RCC_CFGR |= 0x00000000U ;

}
void RCC_APB2_Peripheral(RCC_APB2ENR index)
{
	uint32_t *RCC_AHB2ENR = (uint32_t *)(RCCx_Address + 0x18);
	*RCC_AHB2ENR |= (1 << index);
}
void RCC_APB1_Peripheral(RCC_APB1ENR index){
	uint32_t *RCC_AHB1ENR = (uint32_t *)(RCCx_Address + 0x1C);

	*RCC_AHB1ENR |= (1 << index);
}
void RCC_AHB_Peripheral(RCC_AHBENR index){
	uint32_t *RCC_AHBENR = (uint32_t *)(RCCx_Address + 0x14);

	*RCC_AHBENR |= (1 << index);
}
void RCC_DeInit(void){
	uint32_t *RCC_CIR = (uint32_t *)(RCCx_Address  + 0x08);

	uint32_t *RCC_APB1ENR = (uint32_t *)(RCCx_Address + 0x1C);
	uint32_t *RCC_APB2ENR = (uint32_t *)(RCCx_Address + 0x18);

	/* clear all interrupts */
	*RCC_CIR = 0;
	// Disable IRQ Interrupts
	__asm volatile ("cpsid i" : : : "memory");

	/* Reset of all peripherals */
	*RCC_APB1ENR = 0;
	*RCC_APB2ENR = 0;

}
