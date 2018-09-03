#include <bbblitz3d.h>
#include <bbgraphics.h>
#include <bbmath.h>

#include "Item.h"
#include "Inventory.h"
#include "../GameMain.h"
#include "../AssetMgmt/TextMgmt.h"
#include "../Map/MapSystem.h"
#include "../MathUtils/MathUtils.h"
#include "../Player/Player.h"

using namespace CBN;

std::vector<Item*> Item::list;
int Item::itemDistanceTimer = 0;

Item::Item(const String& meshPath, float scale, ItemPickSound sound, WornItemSlot slot) : meshPath(meshPath), scale(scale) {
    list.push_back(this);

    id = -1;
    addID();
    parentInv = nullptr;
    markedForRemoval = false;

    collider = bbCreatePivot();

    bbEntityType(collider, HIT_ITEM);
    bbResetEntity(collider);
    bbRotateEntity(collider, 0.f, bbRnd(360), 0.f);

    mesh = bbLoadMesh(meshPath, collider);
    bbScaleEntity(mesh, scale * RoomScale, scale * RoomScale, scale * RoomScale);
    pickSound = sound;
    wornSlot = slot;
    needsInvImg = true;
    invImg = nullptr;
    dist = 0.f;
    dropSpeed = 0.f;
}

Item::~Item() {
    bbFreeEntity(mesh);
    if (invImg != nullptr) {
        bbFreeImage(invImg);
    }
    bbFreeEntity(collider);

    if (parentInv != nullptr) {
        parentInv->removeItem(this);
    }

    for (int i = 0; i < (int)list.size(); i++) {
        if (list[i] == this) {
            list.erase(list.begin() + i);
            break;
        }
    }
}

void Item::setCoords(float x, float y, float z) {
    bbPositionEntity(collider, x, y, z, true);
}

void Item::assignTag(const String& tag) {
    if (hasTag(tag)) { return; }
    tags.push_back(tag);
}

bool Item::hasTag(const String& tag) {
    for (int i = 0; i < (int)tags.size(); i++) {
        if (tags[i].equals(tag)) {
            return true;
        }
    }
    return false;
}

void Item::removeTag(const String& tag) {
    bool found = false;
    for (int i = 0; i < (int)tags.size(); i++) {
        if (tags[i].equals(tag)) {
            found = true;
            tags.erase(tags.begin() + i);
            return;
        }
    }

    if (!found) {
        throw ("Removed non-existant tag: " + getType() + ", tag: " + tag);
    }
}

void Item::onPick() { return; }
void Item::onUse() { return; }
void Item::on914Use(Setting914 setting) { return; }
void Item::draw() { return; }
void Item::updateUse() { return; }
void Item::drawUse() { return; }
void Item::drawAll() { return; }
void Item::saveXML() { return; }

void Item::combineWith(Item* other) {
    txtMgmt->setMsg(txtMgmt->lang["inv_cantcombine"]);
}

void Item::update() {
    float hideDistSqr = HideDistance*0.5f; hideDistSqr*=hideDistSqr;

    if (parentInv == nullptr) {
        if (itemDistanceTimer < TimeInPosMilliSecs()) {
            dist = bbEntityDistanceSquared(mainPlayer->collider, collider);
        }

        if (dist < hideDistSqr) {
            bbShowEntity(collider);

            if (dist < 1.2f*1.2f) {
                if (mainPlayer->closestItem == nullptr) {
                    if (bbEntityInView(mesh, mainPlayer->cam)) {
                        Object* pick = bbLinePick(bbEntityX(mainPlayer->cam),bbEntityY(collider),bbEntityZ(mainPlayer->cam),bbEntityX(collider)-bbEntityX(mainPlayer->cam),0,bbEntityZ(collider)-bbEntityZ(mainPlayer->cam),0.f);
                        if (pick == collider || pick == nullptr) {
                            mainPlayer->closestItem = this;
                        }
                    }
                // Is this item closer to the player than their currently selected item?
                } else if (this != mainPlayer->closestItem && dist < mainPlayer->closestItem->dist) {
                    if (bbEntityInView(mesh, mainPlayer->cam)) {
                        Object* pick = bbLinePick(bbEntityX(mainPlayer->cam),bbEntityY(collider),bbEntityZ(mainPlayer->cam),bbEntityX(collider)-bbEntityX(mainPlayer->cam),0,bbEntityZ(collider)-bbEntityZ(mainPlayer->cam),0.f);
                        if (pick == collider || pick == nullptr) {
                            mainPlayer->closestItem = this;
                        }
                    }
                }
            }

            if (bbEntityCollided(collider, HIT_MAP)) {
                dropSpeed = 0;
            } else {
                std::cout << String(dropSpeed) + "\n";
                dropSpeed = dropSpeed - 0.0004f * timing->tickDuration;
                bbTranslateEntity(collider, 0.f, dropSpeed * timing->tickDuration, 0.f);
            }

            if (dist < hideDistSqr*0.2f) {
                for (int i = 0; i < (int)list.size(); i++) {
                    Item* collItem = list[i];

                    if (this != collItem && collItem->parentInv == nullptr && collItem->dist < hideDistSqr*0.2f) {

                        float xtemp = bbEntityX(collItem->collider,true)-bbEntityX(collider,true);
                        float ytemp = bbEntityY(collItem->collider,true)-bbEntityY(collider,true);
                        float ztemp = bbEntityZ(collItem->collider,true)-bbEntityZ(collider,true);

                        float ed = (xtemp*xtemp+ztemp*ztemp);
                        if (ed < 0.07f && abs(ytemp) < 0.25f) {
                            //items are too close together, push away

                            xtemp = xtemp*(0.07f-ed);
                            ztemp = ztemp*(0.07f-ed);

                            while (abs(xtemp)+abs(ztemp)<0.001f) {
                                xtemp = xtemp+bbRnd(-0.002f,0.002f);
                                ztemp = ztemp+bbRnd(-0.002f,0.002f);
                            }

                            bbTranslateEntity(collItem->collider,xtemp,0,ztemp);
                            bbTranslateEntity(collider,-xtemp,0,-ztemp);
                        }
                    }
                }
            }

            if (bbEntityY(collider) < - 35.0f) {
                std::cout << "remove: " + getType() << "\n";
                markedForRemoval = true;
                return;
            }
        } else {
            bbHideEntity(collider);
        }
    }
}

void Item::updateAll() {
    mainPlayer->closestItem = nullptr;
    for (int i = 0; i < (int)list.size(); i++) {
        if (list[i]->markedForRemoval) {
            delete list[i];
            i--;
            continue;
        }
        list[i]->update();
    }

    bool canSeePlayer = true;
    if (mainPlayer->closestItem != nullptr) {
        //Can the player see this? (TODO: fix)
        //canSeePlayer = EntityVisible(mainPlayer\closestItem\collider,mainPlayer\cam)
        //If (Not canSeePlayer) Then canSeePlayer = EntityVisible(mainPlayer\closestItem\collider,mainPlayer\collider)
        if (canSeePlayer) {
            if (MouseHit1) {
                mainPlayer->pickItem(mainPlayer->closestItem);
            }
        }
    }

    if (itemDistanceTimer < TimeInPosMilliSecs()) {
        itemDistanceTimer = TimeInPosMilliSecs() + 800;
    }
}

int Item::ID = 0;
void Item::addID() {
    this->id = ID++;
}
// For XML loading.
void Item::setID(int id) {
    this->id = id;
    if (id > ID) {
        ID = id;
    }
}