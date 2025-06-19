#pragma once

#include <QList>
#include <QStringList>

#include "mapElements.h"

class MapValidator {
public:
    static bool validateMap(const QList<MapElement*>& elements);
    static QStringList getValidationErrors();
    static void setErrors(const QStringList& errorList);

private:
    static QStringList errors;
};
