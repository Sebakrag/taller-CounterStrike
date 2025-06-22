#include "mapValidator.h"

// Inicialización de la variable estática
QStringList MapValidator::errors;

bool MapValidator::validateMap(const QList<MapElement*> &elements) {
    // Limpiar errores anteriores
    errors.clear();
    
    // Contadores para validación
    int teamSpawnCT = 0;
    int teamSpawnT = 0;
    int bombZones = 0;
    
    // Contar elementos por tipo
    for (const MapElement* element : elements) {
        // Para tiles, verificamos si es alguno de los tiles especiales por su ID
        if (element->getType() == TILE) {
            // Hacer un dynamic_cast para acceder a los métodos específicos de Tile
            const Tile* tile = dynamic_cast<const Tile*>(element);
            if (tile) {
                int tileId = tile->getTileId();
                
                // Identificar tiles especiales por su ID
                if (tileId == 1) { // CT Spawn es siempre el tile #1
                    teamSpawnCT++;
                } else if (tileId == 2) { // T Spawn es siempre el tile #2
                    teamSpawnT++;
                } else if (tileId == 3) { // Bombsite es siempre el tile #3
                    bombZones++;
                }
            }
        }
    }
    
    // Validar requisitos
    bool isValid = true;
    
    // Verificar que haya al menos una zona de inicio para cada equipo
    if (teamSpawnCT == 0) {
        errors.append("Falta zona de inicio para el equipo Counter-Terrorist");
        isValid = false;
    }
    
    if (teamSpawnT == 0) {
        errors.append("Falta zona de inicio para el equipo Terrorist");
        isValid = false;
    }
    
    // Verificar que haya al menos una zona para plantar bombas
    if (bombZones == 0) {
        errors.append("Se requiere al menos una zona para plantar bombas");
        isValid = false;
    }
    
    return isValid;
}

QStringList MapValidator::getValidationErrors() {
    return errors;
}

void MapValidator::setErrors(const QStringList &errorList) {
    errors = errorList;
}
