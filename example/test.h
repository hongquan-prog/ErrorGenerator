#pragma once 

// error code begin
typedef enum {
  LCD_ERR_OK = ERR_CODE_BEGIN(1),
  LCD_ERR_MEM,
  LCD_ERR_BUF,
  LCD_ERR_TIMEOUT,
  LCD_ERR_VAL
} lcd_err_enum1_t;

// error code end

typedef enum {
  LCD_ERR_OK = ERR_CODE_BEGIN(2),
  LCD_ERR_MEM,
  LCD_ERR_BUF,
  LCD_ERR_TIMEOUT,
  LCD_ERR_VAL
} lcd_err_enum2_t;

typedef enum {
  LCD_ERR_OK = ERR_CODE_BEGIN(3),
  LCD_ERR_MEM,
  LCD_ERR_BUF,
  LCD_ERR_TIMEOUT,
  LCD_ERR_VAL
} lcd_err_enum3_t;

typedef enum {
  LCD_ERR_OK = ERR_CODE_BEGIN(4),
  LCD_ERR_MEM,
  LCD_ERR_BUF,
  LCD_ERR_TIMEOUT,
  LCD_ERR_VAL
} lcd_err_enum4_t;

