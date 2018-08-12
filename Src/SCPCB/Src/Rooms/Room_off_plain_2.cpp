#include <bbblitz3d.h>

#include "../MapSystem.h"
#include "../Items/Items.h"
#include "../Decals.h"
#include "Room_off_plain_2.h"

namespace CBN {

// Functions.
void FillRoom_off_plain_2(Room* r) {
    Door* d;
    Door* d2;
    SecurityCam* sc;
    Decal* de;
    Room* r2;
    SecurityCam* sc2;
    Item* it;
    int i;
    int xtemp;
    int ytemp;
    int ztemp;

    //, Bump
    int t1;

    it = CreatePaper("doc106", r->x + 404.f * RoomScale, r->y + 145.f * RoomScale, r->z + 559.f * RoomScale);
    bbEntityParent(it->collider, r->obj);

    it = CreateItem("navigator", r->x + 305.f * RoomScale, r->y + 153.f * RoomScale, r->z + 944.f * RoomScale);
    it->state = 20;
    bbEntityParent(it->collider, r->obj);

    it = CreatePaper("914_organic", r->x -137.f * RoomScale, r->y + 153.f * RoomScale, r->z + 464.f * RoomScale);
    bbEntityParent(it->collider, r->obj);

    //TODO: fix?
    //w.waypoints = CreateWaypoint(r\x - 32.f * RoomScale, r\y + 66.f * RoomScale, r\z + 288.f * RoomScale, Null, r)
    //w2.waypoints = CreateWaypoint(r\x, r\y + 66.f * RoomScale, r\z - 448.f * RoomScale, Null, r)
    //w\connected[0] = w2
    // w->dist[0] = bbEntityDistance(w->obj, w2->obj);
    //w2\connected[0] = w
    //w2->dist[0] = w->dist[0];
}

}
