#include "client/include/model/utils/SpriteMetadataProvider.h"


SpriteSheetLayout SpriteMetadataProvider::getLayout(const SpriteType type) {
    switch (type) {
        case SpriteType::PHEONIX:
        case SpriteType::L337_KREW:
        case SpriteType::ARTIC_AVENGER:
        case SpriteType::GUERRILLA:
        case SpriteType::SEAL_FORCE:
        case SpriteType::GERMAN_GSG_9:
        case SpriteType::UK_SAS:
        case SpriteType::FRENCH_GIGN:
            return SpriteSheetLayout(2, 3);
        case SpriteType::AK47:
            return SpriteSheetLayout(1, 2);
        default:
            return SpriteSheetLayout(1, 1);
    }
}
