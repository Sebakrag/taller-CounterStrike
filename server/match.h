#ifndef MATCH_H_
#define MATCH_H_

#include <string>

#include "../common/thread.h"

class Match: Thread {
private:
    // definir
public:
    Match();

    bool addPlayer(const std::string& username);

    void removePlayer(const std::string& username);

    ~Match();

private:
    void run() override;
};

#endif  // MATCH_H_
