#ifndef CLIENT_H
#define CLIENT_H

#include <utility>
#include <vector>

#include "dtos/AimInfo.h"
#include "dtos/EntitySnapshot.h"
#include "include/model/utils/Vec2D.h"

class Client {
private:
    // Tiene que tener el contacto con las colas y los hilos sender y receiver.
    EntitySnapshot snap;
    float x;  // borrarlo una vez que tengamos la conexion con el server.
    float y;
    float angle;

public:
    explicit Client(const EntitySnapshot&
                            snap);  // cambiar por el constructor que reciba el hostname y el port.

    std::vector<EntitySnapshot> getGameSnapshot() const;
    void move(const Vec2D& direction);
    void shoot(const AimInfo& aimInfo);
    void rotate(const float angle);
};


#endif  // CLIENT_H
