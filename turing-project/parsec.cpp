#include"parsec.h"
using namespace std;

string tostring(const String& s) {
    return string(s.begin(), s.end());
}

string show(const String& s){
    return "\"" + tostring(s) + "\""; 
}

String toString(const string& s) {
    return String(s.begin(), s.end());
}

Parsec<char> satisfy(const function<bool(char)> f, const string& info) {
    return Parsec<char>([=](String s)->ParseRes<char> {
        if (s.empty()) {
            return makeError<char>("Unexpected end of string");
        }
        auto i = s.begin();
        auto head = *i;
        i++;
        String res = String(i, s.end());
        if (f(head)) {
            return makeRes<char>({ head, res });
        }
        return makeError<char>(
            string("'") + head + "' cannot satisfy the given rule \"" + info + "\"");
        }
    );
}