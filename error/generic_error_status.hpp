//
// Copyright 2013 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// https://github.com/arobenko/embxx/blob/master/embxx/error/ErrorStatus.h


#pragma once

namespace error
{

/// @addtogroup error
/// @{

/// @brief Error code wrapper class.
/// @details This class wraps error code enum value to provide simple boolean
/// checks for operation success/error
/// @tparam TErrorCode Error code enum type. 0 value must correspond to success.
/// @headerfile error/error_status.h
template <typename TErrorCode, TErrorCode success_code = 0>
class generic_error_status
{
public:
    /// @brief Error code enum type
    typedef TErrorCode error_code_type;

    /// @brief Default constructor.
    /// @details The code value is 0, which is "success".
    generic_error_status()
        : code_(success_code)
    {}

    /// @brief Constructor
    /// @details This constructor may be used for implicit construction of
    ///          error status object out of error code value.
    /// @param codeVal Numeric error code value.
    generic_error_status(error_code_type codeVal)
        : code_(codeVal)
    {}

    /// @brief Copy constructor is default
    generic_error_status(const generic_error_status&) = default;

    /// @brief Destructor is default
    ~generic_error_status() = default;

    /// @brief Copy assignment is default
    generic_error_status& operator=(const generic_error_status&) = default;

    /// @brief Retrieve error code value.
    const error_code_type code() const
    {
        return code_;
    }

    /// @brief Equality comparison operator
    bool operator==(const generic_error_status& other) const
    {
        return code() == other.code();
    }

    /// @brief Equality comparison operator
    bool operator==(error_code_type otherCode) const
    {
        return code() == otherCode;
    }

    /// @brief Inequality comparison operator
    template <typename TOther>
    bool operator!=(TOther&& other) const
    {
        return !(operator==(std::forward<TOther>(other)));
    }

    /// @brief boolean conversion operator.
    /// @details Returns true if error code is not equal 0, i.e. any error
    ///          will return true, success value will return false.
    operator bool() const
    {
        return (code_ != success_code);
    }

    /// @brief Same as !(static_cast<bool>(*this)).
    bool operator!() const
    {
        return !(static_cast<bool>(*this));
    }

private:
    error_code_type code_;
};


// Implementation
/// @brief Error status equality comparison operator
/// @related generic_error_status
template <typename TOther, typename TErrorCode>
bool operator==(TOther&& value, const generic_error_status<TErrorCode>& status)
{
    return status == std::forward<TOther>(value);
}

template <typename TOther, typename TErrorCode>
bool operator!=(TOther&& value, const generic_error_status<TErrorCode>& status)
{
    return status != std::forward<TOther>(value);
}

/// @}

}  // namespace error
