#include "IHC.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define ESPERA 500

bool IHC::escFoiPressionado() {
#ifdef _WIN32
  // https://msdn.microsoft.com/pt-br/library/windows/desktop/ms646293(v=vs.85).aspx
  return GetAsyncKeyState(VK_ESCAPE);
#else
  // http://www.gnu.org/software/libc/manual/html_node/Character-Input.html
  // TODO
  return false;
#endif

}

void IHC::sleep() {
#ifdef _WIN32
  Sleep (ESPERA);
#else
  usleep(1000 * ESPERA);
#endif
}
