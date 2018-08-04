#include "NPCtype939.h"
#include "include.h"

namespace CBN {

// Functions.
void InitializeNPCtype939(NPC* n) {
    int i = 53;
    NPC* n2;
    for (int iterator130 = 0; iterator130 < NPC::getListSize(); iterator130++) {
        n2 = NPC::getObject(iterator130);

        if ((n->npcType == n2->npcType) & (n!=n2)) {
            i = i + bbRand(12, 36);
        }
    }
    n->nvName = "SCP-939-"+String(i);
    int amount939 = 0;

    for (int iterator131 = 0; iterator131 < NPC::getListSize(); iterator131++) {
        n2 = NPC::getObject(iterator131);

        if ((n->npcType == n2->npcType) & (n!=n2)) {
            amount939 = amount939 + 1;
        }
    }
    if (amount939 == 0) {
        i = 53;
    }
    if (amount939 == 1) {
        i = 89;
    }
    if (amount939 == 2) {
        i = 96;
    }
    n->nvName = "SCP-939-"+String(i);

    n->collider = bbCreatePivot();
    bbEntityRadius(n->collider, 0.3);
    bbEntityType(n->collider, HIT_PLAYER);
    for (int iterator132 = 0; iterator132 < NPC::getListSize(); iterator132++) {
        n2 = NPC::getObject(iterator132);

        if (n->npcType == n2->npcType & n!=n2) {
            n->obj = bbCopyEntity(n2->obj);
            break;
        }
    }

    float temp;
    if (n->obj == 0) {
        n->obj = bbLoadAnimMesh("GFX/NPCs/scp939/scp-939.b3d");

        temp = GetINIFloat("Data/NPCs.ini", "SCP-939", "scale")/2.5;
        bbScaleEntity(n->obj, temp, temp, temp);
    }

    n->speed = (GetINIFloat("Data/NPCs.ini", "SCP-939", "speed") / 100.0);
}

void UpdateNPCtype939(NPC* n) {
    float dist;
    int prevFrame;
    int temp;
    float angle;

    if (mainPlayer->currRoom->roomTemplate->name != "room3storage") {
        n->state = 66;
    }

    //state is set to 66 in the room3storage-event if player isn't inside the room
    if (n->state < 66) {
        switch (n->state) {
            case 0: {
                AnimateNPC(n, 290,405,0.1);

                //Animate2(n\obj,AnimTime(n\obj),290,405,0.1)
            }
            case 1: {

                //finish the walking animation
                if (n->frame>=644 & n->frame<683) {
                    //n\currSpeed = CurveValue(n\speed*0.2, n\currSpeed, 10.0)
                    n->currSpeed = CurveValue(n->speed*0.05, n->currSpeed, 10.0);
                    AnimateNPC(n, 644,683,28*n->currSpeed*4,false);
                    if (n->frame>=682) {
                        n->frame = 175;
                    }

                    //Animate2(n\obj,AnimTime(n\obj),644,683,28*n\currSpeed,False)
                    //If (AnimTime(n\obj)=683) Then SetAnimTime(n\obj,175)
                } else {
                    n->currSpeed = CurveValue(0, n->currSpeed, 5.0);
                    AnimateNPC(n, 175,297,0.22,false);
                    if (n->frame>=296) {
                        n->state = 2;
                    }

                    //Animate2(n\obj,AnimTime(n\obj),175,297,0.22,False)
                    //If (AnimTime(n\obj)=297) Then n\state = 2
                }

                n->lastSeen = 0;

                bbMoveEntity(n->collider, 0,0,n->currSpeed*timing->tickDuration);

            }
            case 2: {
                n->state2 = Max(n->state2, (n->prevState-3));

                dist = bbEntityDistance(n->collider, mainPlayer->currRoom->objects[(int)(n->state2)]);

                n->currSpeed = CurveValue(n->speed*0.3*Min(dist,1.0), n->currSpeed, 10.0);
                bbMoveEntity(n->collider, 0,0,n->currSpeed*timing->tickDuration);

                prevFrame = (int)(n->frame);
                //walk
                AnimateNPC(n, 644,683,28*n->currSpeed);

                //prevFrame = AnimTime(n\obj)
                //Animate2(n\obj,AnimTime(n\obj),644,683,28*n\currSpeed) ;walk

                if (prevFrame<664 & n->frame>=664) | (prevFrame>673 & n->frame<654) {
                    PlayRangedSound(sndManager->footstepMetal[bbRand(0,7)]->internal, mainPlayer->cam, n->collider, 12.0);
                    if (bbRand(10)==1) {
                        temp = false;
                        if (!IsChannelPlaying(n->soundChannels[0])) {
                            temp = true;
                        }
                        if (temp) {
                            if (n->sounds[0] != 0) {
                                bbFreeSound(n->sounds[0]);
                                n->sounds[0] = 0;
                            }
                            n->sounds[0] = bbLoadSound("SFX/SCP/939/"+String(n->id % 3)+"Lure"+String(bbRand(1,10))+".ogg");
                            n->soundChannels[0] = PlayRangedSound(n->sounds[0], mainPlayer->cam, n->collider);
                        }
                    }
                }

                bbPointEntity(n->obj, mainPlayer->currRoom->objects[(int)(n->state2)]);
                bbRotateEntity(n->collider, 0, CurveAngle(bbEntityYaw(n->obj),bbEntityYaw(n->collider),20.0), 0);

                if (dist<0.4) {
                    n->state2 = n->state2 + 1;
                    if (n->state2 > n->prevState) {
                        n->state2 = (n->prevState-3);
                    }
                    n->state = 1;
                }

            }
            case 3: {
                if (bbEntityVisible(mainPlayer->collider, n->collider)) {
                    if (n->sounds[1] == 0) {
                        n->sounds[1] = bbLoadSound("SFX/General/Slash1.ogg");
                    }

                    n->enemyX = bbEntityX(mainPlayer->collider);
                    n->enemyZ = bbEntityZ(mainPlayer->collider);
                    n->lastSeen = 10*7;
                }

                if (n->lastSeen > 0 & (!NoTarget)) {
                    prevFrame = (int)(n->frame);

                    if (n->frame>=18.0 & n->frame<68.0) {
                        n->currSpeed = CurveValue(0, n->currSpeed, 5.0);
                        AnimateNPC(n, 18,68,0.5,true);
                        //Animate2(n\obj,AnimTime(n\obj),18,68,0.5,True)

                        //hasn't hit
                        temp = false;

                        if (prevFrame < 24 & n->frame>=24) {
                            temp = true;
                        } else if ((prevFrame < 57 & n->frame>=57)) {
                            temp = true;
                        }

                        if (temp) {
                            if (Distance(n->enemyX, n->enemyZ, bbEntityX(n->collider), bbEntityZ(n->collider))<1.5) {
                                PlaySound2(n->sounds[1]);
                                mainPlayer->injuries = mainPlayer->injuries + bbRnd(1.5, 2.5)-IsPlayerWearingItem(mainPlayer,"vest")*0.5;
                                mainPlayer->blurTimer = 500;
                            } else {
                                n->frame = 449;
                                //SetAnimTime(n\obj, 449)
                            }
                        }

                        if (mainPlayer->injuries>4.0) {
                            DeathMSG = "\"All four (4) escaped SCP-939 specimens have been captured and recontained successfully. ";
                            DeathMSG = DeathMSG+"Three (3) of them made quite a mess at Storage Area 6. A cleaning team has been dispatched.\"";
                            Kill(mainPlayer);
                            if (!mainPlayer->godMode) {
                                n->state = 5;
                            }
                        }
                    } else {
                        if (n->lastSeen == 10*7) {
                            n->currSpeed = CurveValue(n->speed, n->currSpeed, 20.0);

                            //run
                            AnimateNPC(n, 449,464,6*n->currSpeed);
                            //Animate2(n\obj,AnimTime(n\obj),449,464,6*n\currSpeed) ;run

                            if (prevFrame<452 & n->frame>=452) | (prevFrame<459 & n->frame>=459) {
                                PlayRangedSound(sndManager->footstepMetalRun[bbRand(0,7)]->internal, mainPlayer->cam, n->collider, 12.0);
                            }

                            //player is visible
                            if (Distance(n->enemyX, n->enemyZ, bbEntityX(n->collider), bbEntityZ(n->collider))<1.1) {
                                n->frame = 18;
                                //SetAnimTime(n\obj, 18.0)
                            }
                        } else {
                            n->currSpeed = CurveValue(0, n->currSpeed, 5.0);
                            AnimateNPC(n, 175,297,5*n->currSpeed,true);
                            //Animate2(n\obj,AnimTime(n\obj),175,297,5*n\currSpeed,True)
                        }

                    }

                    angle = bbVectorYaw(n->enemyX-bbEntityX(n->collider), 0.0, n->enemyZ-bbEntityZ(n->collider));
                    bbRotateEntity(n->collider, 0, CurveAngle(angle,bbEntityYaw(n->collider),15.0), 0);

                    bbMoveEntity(n->collider, 0,0,n->currSpeed*timing->tickDuration);

                    n->lastSeen = (int)(n->lastSeen - timing->tickDuration);
                } else {
                    n->state = 2;
                }

                //Animate2(n\obj,AnimTime(n\obj),406,437,0.1) ;leap
            }
            case 5: {
                if (n->frame<68) {
                    //finish the attack animation
                    AnimateNPC(n, 18,68,0.5,false);

                    //Animate2(n\obj,AnimTime(n\obj),18,68,0.5,False) ;finish the attack animation
                } else {
                    //attack to idle
                    AnimateNPC(n, 464,473,0.5,false);

                    //Animate2(n\obj,AnimTime(n\obj),464,473,0.5,False) ;attack to idle
                }

            }
        }

        if (n->state < 3 & (!NoTarget) & (!n->ignorePlayer)) {
            dist = bbEntityDistance(n->collider, mainPlayer->collider);

            if (dist < 4.0) {
                dist = dist - bbEntityVisible(mainPlayer->collider, n->collider);
            }
            if (mainPlayer->loudness*1.2>dist | dist < 1.5) {
                if (n->state3 == 0) {
                    if (n->sounds[0] != 0) {
                        bbFreeSound(n->sounds[0]);
                        n->sounds[0] = 0;
                    }
                    n->sounds[0] = bbLoadSound("SFX/SCP/939/"+String(n->id % 3)+"Attack"+String(bbRand(1,3))+".ogg");
                    n->soundChannels[0] = PlayRangedSound(n->sounds[0], mainPlayer->cam, n->collider);

                    PlaySound2(LoadTempSound("SFX/SCP/939/attack.ogg"));
                    n->state3 = 1;
                }

                n->state = 3;
            } else if ((mainPlayer->loudness*1.6>dist)) {
                if (n->state!=1 & n->reload <= 0) {
                    if (n->sounds[0] != 0) {
                        bbFreeSound(n->sounds[0]);
                        n->sounds[0] = 0;
                    }
                    n->sounds[0] = bbLoadSound("SFX/SCP/939/"+String(n->id % 3)+"Alert"+String(bbRand(1,3))+".ogg");
                    n->soundChannels[0] = PlayRangedSound(n->sounds[0], mainPlayer->cam, n->collider);

                    n->frame = 175;
                    n->reload = 70 * 3;
                    //SetAnimTime(n\obj, 175)
                }

                n->state = 1;

            }

            n->reload = n->reload - timing->tickDuration;

        }

        bbRotateEntity(n->collider, 0, bbEntityYaw(n->collider), 0, true);

        bbPositionEntity(n->obj, bbEntityX(n->collider), bbEntityY(n->collider)-0.28, bbEntityZ(n->collider));
        bbRotateEntity(n->obj, bbEntityPitch(n->collider)-90, bbEntityYaw(n->collider), bbEntityRoll(n->collider), true);
    }
}

}