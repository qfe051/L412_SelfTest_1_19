// jump 이후에 실행할 앱
#include "boot_app.h"
#include "boot_config.h"

/*
App가 정상적으로 빌드된 경우:
첫 4바이트 = stack pointer
→ 항상 SRAM 영역 주소
*/

bool app_is_valid(void) {
  // APP_ADDR을 (uint32_t *) 데이터로 읽음
  // 주소 내의 값을 읽음 '*'으로 4바이트 값을 읽어 옴
  // boot_config 파일에 저장된 APP 주소
  uint32_t sp = *(uint32_t*)APP_ADDR;

  // 정상적이라면 거짓(0)으로 리턴? - X
  // 앱 주소 안의 값이 RAM 내부에 있으면, 참!
  return (sp >= SRAM_START && sp <= SRAM_END);
}
