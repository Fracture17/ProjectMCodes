#ifndef PROJECTMCODES_SOGENERATEARTICLEMANAGEMODULEIMPL_H
#define PROJECTMCODES_SOGENERATEARTICLEMANAGEMODULEIMPL_H

// #include "Brawl/Weapon.h"
struct Weapon;
struct soGenerateArticleManageModuleImpl {
    // MUST CONVERT TO WEAPONSTAGEOBJ THROUGH DynamicCast
    void*** getArticle(int kind);
};

#define _getArticle_soGenerateArticleManageModuleImpl ((void*** (*)(soGenerateArticleManageModuleImpl* self, int kind)) 0x807a17a0)

#endif //PROJECTMCODES_SOGENERATEARTICLEMANAGEMODULEIMPL_H