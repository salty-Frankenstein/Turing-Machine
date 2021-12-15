#ifndef CONFIG_H
#define CONFIG_H

enum _Mode { NORMAL, VERBOSE };

class Mode {
public:
    friend class Shell;
    static _Mode getMode();
private:
    static _Mode mode;
};

#endif
