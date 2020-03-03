#include "Error.hpp"
#include <system_error>

namespace {

struct ProtocolErrCategory : std::error_category
{
    const char* name() const noexcept override;
    std::string message(int ev) const override;
};

const char* ProtocolErrCategory::name() const noexcept
{
    return "cop errors";
}

std::string ProtocolErrCategory::message(int ev) const {
    switch(static_cast<COP::ProtocolErrc>(ev)) {
        default:
            return "(unrecognised error)";
    }
}

const ProtocolErrCategory protocolErrorCategory {};

}

namespace COP {

std::error_code make_error_code(COP::ProtocolErrc e) {
    return {static_cast<int>(e), protocolErrorCategory};
}

}

