#pragma once
#include <iostream>

/* used for formatting */
namespace Color {
    enum Code {
        FG_RED = 31,
        FG_GREEN = 32,
        FG_BLUE = 34,
        FG_LIGHT_BLUE = 94,
        FG_DEFAULT = 39,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_BLUE = 44,
        BG_DEFAULT = 49
    };
    class Modifier {
        Code code;
        public:
        Modifier(Code pCode) :
            code(pCode) {
            }
        friend std::ostream&
            operator<<(std::ostream& os, const Modifier& mod) {
                return os << "\033[" << mod.code << "m";
            }
    };
}

Color::Modifier red(Color::FG_RED);
Color::Modifier def(Color::FG_DEFAULT);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier blue(Color::FG_LIGHT_BLUE);

std::ostream& bold_on(std::ostream& os){
    return os << "\e[1m";
}

std::ostream& bold_off(std::ostream& os){
    return os << "\e[0m";
}



