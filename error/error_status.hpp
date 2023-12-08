#pragma once

#include "sdk_errors.h"
#include "error/generic_error_status.hpp"

namespace error
{

/// @brief generic_error_status that uses ret_code_t for its error codes.
typedef generic_error_status<ret_code_t, NRF_SUCCESS> error_status;

}  // namespace error
