#ifndef CLIENT_H
#define CLIENT_H

#include <utility>
#include <vector>

#include "dtos/EntitySnapshot.h"
#include "include/model/utils/Vec2D.h"

class Client {
private:
    // Tiene que tener el contacto con las colas y los hilos sender y receiver.
    EntitySnapshot snap;
    float x;  // borrarlo una vez que tengamos la conexion con el server.
    float y;

public:
    explicit Client(const EntitySnapshot&
                            snap);  // cambiar por el constructor que reciba el hostname y el port.

    std::vector<EntitySnapshot> getGameSnapshot() const;
    void move(const Vec2D& direction);
};


#endif  // CLIENT_H
