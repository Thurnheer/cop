#ifndef COP_ERRROR_HPP
#define COP_ERRROR_HPP

#include <system_error>

namespace cop {

enum class ProtocolErrc
{
    success = 0,
    invalid_message_id,
    invalid_message_type,
    not_enough_space_in_buffer,
    not_enough_data,
    framing_error,
    crc_error,
    unexpected_frame_end,
    receiving
};

std::error_code make_error_code(ProtocolErrc);

} // namespace COP

namespace std {
    template<>
    struct is_error_code_enum<cop::ProtocolErrc> : true_type{};
}

#endif // COP_ERRROR_HPP

