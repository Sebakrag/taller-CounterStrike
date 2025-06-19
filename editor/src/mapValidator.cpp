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
        switch (element->getType()) {
            case TEAM_SPAWN_CT:
                teamSpawnCT++;
                break;
            case TEAM_SPAWN_T:
                teamSpawnT++;
                break;
            case BOMB_ZONE:
                bombZones++;
                break;
            default:
                break;
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
