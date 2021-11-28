#include <stdbool.h>

static struct error_str_t
{
	bool exist;
	int last_error;
	const char ** error_array;
}s_error_str_array[1];

static const char* s_lcd_err_enum1_t_err_str[] = {
	"LCD_ERR_OK",
	"LCD_ERR_MEM",
	"LCD_ERR_BUF",
	"LCD_ERR_TIMEOUT",
	"LCD_ERR_VAL",
};

void error_str_init()
{
	s_error_str_array[1].exist = true;
	s_error_str_array[1].last_error = 4;
	s_error_str_array[1].error_array = s_lcd_err_enum1_t_err_str;
}

// example 
/*const char* error_to_str(err_t errno)
{
	static bool initialized = false;
	unsigned short error_code = ERR_GET_ERROR_INDEX(errno);
	unsigned short module_id = ERR_GET_MODULE_ID(errno);
	if(!initialized)
	{
		error_str_init();
		initialized = true;
	}
	if(errno > 0)
		return "Errno should less than 0";
	if(!s_error_str_array[module_id].exist)
		return "Error code array isn't exist";
	if(s_error_str_array[module_id].last_error < error_code)
		return "Error code out of range";
	return s_error_str_array[module_id].error_array[error_code];
}*/