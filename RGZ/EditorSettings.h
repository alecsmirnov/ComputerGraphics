#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_h

#include "Color.h"
#include "KeyButtons.h"

// Field settings
static constexpr Color		  BACKGROUND_COLOR	   = ColorElem::WHITE;

static constexpr Color		  POINTS_COLOR		   = ColorElem::RED;
static constexpr GLfloat	  POINTS_SIZE		   = 3.0f;

static constexpr Color		  SPLINE_COLOR		   = ColorElem::BLUE;
static constexpr GLfloat	  SPLINE_POINTS_SIZE   = 1.0f;

static constexpr Color		  GRID_COLOR		   = ColorElem::GREY;
static constexpr Color		  GRID_COORDLINE_COLOR = ColorElem::BLACK;
static constexpr std::uint8_t GRID_CELL_SIZE	   = 10;

static constexpr std::uint8_t ARROW_WIDTH		   = 5;
static constexpr std::uint8_t ARROW_HEIGHT		   = 15;
static constexpr std::uint8_t DIVISION_SIZE		   = 6;

// Transformation settings
static constexpr std::uint8_t DEGREE_MIN		= 1;
static constexpr std::uint8_t DEGREE_MAX		= 8;
static constexpr std::uint8_t DEGREE_COEF		= 1;

static constexpr GLdouble	  STEP_MIN			= 0.1;
static constexpr GLdouble	  STEP_MAX			= 0.00001;
static constexpr GLdouble	  STEP_COEF			= 0.1;

static constexpr std::uint8_t SHIFT_SPEED		= 20;
static constexpr GLfloat	  SCALE_COEF_FACTOR	= 0.05f;
static constexpr GLfloat	  SCALE_COEF_MAX	= 10.0f;
static constexpr GLfloat	  SCALE_COEF_MIN	= 0.1f;

#endif