#pragma once

#include "MemLeaks.h"

#define MIN( a, b ) ( ((a) < (b)) ? (a) : (b) )
#define MAX( a, b ) ( ((a) > (b)) ? (a) : (b) )

enum class UpdateStatus
{
	Continue = 1,
	Stop,
	Error
};

//config file
#define CONFIG_FILE "config.json"