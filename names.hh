#pragma once

#include <memory>
#include <regex>

namespace nix {

struct DrvName {
    string fullName;
    string name;
    string version;
    unsigned int hits;

    DrvName();
    DrvName(const string& s);

    bool matches(DrvName& n);

private:
    std::unique_ptr<std::regex> regex;
}

}
