#include <stdint.h>

typedef enum
{
	LOG_LEVEL_ERROR,
	LOG_LEVEL_INFORMATION,
	LOG_LEVEL_DEBUG
}LogLevel;

//global variable system_log_level-> defined and given desired log level
extern LogLevel system_log_level;

void log_error(char const * const format, ...);
void log_info(char const * const format, ...);
void log_debug(char const * const format, ...);
void log_debug_array (char const * const label, void const *array, uint16_t const len);

