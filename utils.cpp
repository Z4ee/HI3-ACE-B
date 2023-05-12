#include "utils.h"

namespace utils {
	bool create_console() {

		AllocConsole();

		FILE* file_out;
		FILE* file_in;

		errno_t err_out = freopen_s(&file_out, "CONOUT$", "w", stdout);
		errno_t err_in = freopen_s(&file_in, "CONIN$", "r", stdin);

		return err_out == 0 && err_in == 0;
	}
}