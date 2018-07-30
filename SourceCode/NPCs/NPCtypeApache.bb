Function InitializeNPCtypeApache(n.NPC)
    n\nvName = "Human"
    n\gravityMult = 0.0
    n\maxGravity = 0.0
    n\collider = CreatePivot()
    EntityRadius(n\collider, 0.2)

	Local n2.NPC
	For n2 = Each NPC
        If (n\npcType = n2\npcType) And (n <> n2) Then
            n\obj = CopyEntity(n2\obj)
			n\obj2 = CopyEntity(n2\obj2)
			n\obj3 = CopyEntity(n2\obj3)
            Exit
        EndIf
    Next

	If (n\obj = 0) Then
		n\obj = LoadAnimMesh("GFX/NPCs/apache/apache.b3d")
		n\obj2 = LoadAnimMesh("GFX/NPCs/apache/apacherotor.b3d")
		n\obj3 = LoadAnimMesh("GFX/NPCs/apache/apacherotor2.b3d")
	EndIf

    EntityParent(n\obj2, n\obj)
	EntityParent(n\obj3, n\obj)

	n\sounds[0] = LoadSound("SFX/Character/Apache/Propeller.ogg")
    n\sounds[1] = LoadSound("SFX/Character/Apache/Alarm.ogg")

	Local i%, rotor2%
    For i = -1 To 1 Step 2
        rotor2 = CopyEntity(n\obj2,n\obj2)
        RotateEntity(rotor2,0,4.0*i,0)
        EntityAlpha(rotor2, 0.5)
    Next

    PositionEntity(n\obj3, 0.0, 2.15, -5.48)

    EntityType(n\collider, HIT_APACHE)
    EntityRadius(n\collider, 3.0)

    Local temp# = 0.6
    ScaleEntity(n\obj, temp, temp, temp)
End Function

Function UpdateNPCtypeApache(n.NPC)
    Local dist2#, dist#, target%, pvt%

    If (n\playerDistance<60.0) Then
        If (mainPlayer\currRoom\roomTemplate\name = "exit1") Then
            dist2 = Max(Min(EntityDistance(n\collider, mainPlayer\currRoom\objects[3])/(8000.0*RoomScale),1.0),0.0)
        Else
            dist2 = 1.0
        EndIf

        n\soundChannels[0] = LoopRangedSound(n\sounds[0], n\soundChannels[0], mainPlayer\cam, n\collider, 25.0, dist2)
    EndIf

    n\dropSpeed = 0

    Select (n\state)
        Case 0,1
            TurnEntity(n\obj2,0,20.0*timing\tickDuration,0)
            TurnEntity(n\obj3,20.0*timing\tickDuration,0,0)

            If (n\state=1 And (Not NoTarget)) Then
                If (Abs(EntityX(mainPlayer\collider)-EntityX(n\collider))< 30.0) Then
                    If (Abs(EntityZ(mainPlayer\collider)-EntityZ(n\collider))<30.0) Then
                        If (Abs(EntityY(mainPlayer\collider)-EntityY(n\collider))<20.0) Then
                            If (Rand(20)=1) Then
                                If (EntityVisible(mainPlayer\collider, n\collider)) Then
                                    n\state = 2
                                    PlayRangedSound(n\sounds[1], mainPlayer\cam, n\collider, 50, 1.0)
                                EndIf
                            EndIf
                        EndIf
                    EndIf
                EndIf
            EndIf
        Case 2,3 ;player located -> attack

            If (n\state = 2) Then
                target = mainPlayer\collider
            ElseIf (n\state = 3) Then
                target=CreatePivot()
                PositionEntity(target, n\enemyX, n\enemyY, n\enemyZ, True)
            EndIf

            If (NoTarget And n\state = 2) Then n\state = 1

            TurnEntity(n\obj2,0,20.0*timing\tickDuration,0)
            TurnEntity(n\obj3,20.0*timing\tickDuration,0,0)

            If (Abs(EntityX(target)-EntityX(n\collider)) < 55.0) Then
                If (Abs(EntityZ(target)-EntityZ(n\collider)) < 55.0) Then
                    If (Abs(EntityY(target)-EntityY(n\collider))< 20.0) Then
                        PointEntity(n\obj, target)
                        RotateEntity(n\collider, CurveAngle(Min(WrapAngle(EntityPitch(n\obj)),40.0),EntityPitch(n\collider),40.0), CurveAngle(EntityYaw(n\obj),EntityYaw(n\collider),90.0), EntityRoll(n\collider), True)
                        PositionEntity(n\collider, EntityX(n\collider), CurveValue(EntityY(target)+8.0,EntityY(n\collider),70.0), EntityZ(n\collider))

                        dist = Distance(EntityX(target),EntityZ(target),EntityX(n\collider),EntityZ(n\collider))

                        n\currSpeed = CurveValue(Min(dist-6.5,6.5)*0.008, n\currSpeed, 50.0)

                        ;If (Distance(EntityX(mainPlayer\collider),EntityZ(mainPlayer\collider),EntityX(n\collider),EntityZ(n\collider)) > 6.5) Then
                        ;	n\currspeed = CurveValue(0.08,n\currspeed,50.0)
                        ;Else
                        ;	n\currspeed = CurveValue(0.0,n\currspeed,30.0)
                        ;EndIf
                        MoveEntity(n\collider, 0,0,n\currSpeed*timing\tickDuration)


                        If (n\pathTimer = 0) Then
                            n\pathStatus = EntityVisible(n\collider,target)
                            n\pathTimer = Rand(100,200)
                        Else
                            n\pathTimer = Min(n\pathTimer-timing\tickDuration,0.0)
                        EndIf

                        If (n\pathStatus = 1) Then ;player visible
                            RotateEntity(n\collider, EntityPitch(n\collider), EntityYaw(n\collider), CurveAngle(0, EntityRoll(n\collider),40), True)

                            If (n\reload =< 0) Then
                                If (dist<20.0) Then
                                    pvt = CreatePivot()

                                    PositionEntity(pvt, EntityX(n\collider),EntityY(n\collider), EntityZ(n\collider))
                                    RotateEntity(pvt, EntityPitch(n\collider), EntityYaw(n\collider),EntityRoll(n\collider))
                                    MoveEntity(pvt, 0, 8.87*(0.21/9.0), 8.87*(1.7/9.0)) ;2.3
                                    PointEntity(pvt, target)

                                    If (WrapAngle(EntityYaw(pvt)-EntityYaw(n\collider))<10) Then
                                        PlayRangedSound_SM(sndManager\gunshot[1], mainPlayer\cam, n\collider, 20)

                                        DeathMSG = Chr(34)+"CH-2 to control. Shot down a runaway Class D at Gate B."+Chr(34)

                                        Shoot( EntityX(pvt),EntityY(pvt), EntityZ(pvt),((10/dist)*(1/dist))*(n\state=2),(n\state=2))

                                        n\reload = 5
                                    EndIf

                                    FreeEntity(pvt)
                                EndIf
                            EndIf
                        Else
                            RotateEntity(n\collider, EntityPitch(n\collider), EntityYaw(n\collider), CurveAngle(-20, EntityRoll(n\collider),40), True)
                        EndIf
                        MoveEntity(n\collider, -EntityRoll(n\collider)*0.002,0,0)

                        n\reload=n\reload-timing\tickDuration


                    EndIf
                EndIf
            EndIf

            If (n\state = 3) Then FreeEntity(target)
        Case 4 ;crash
            If (n\state2 < 300) Then

                TurnEntity(n\obj2,0,20.0*timing\tickDuration,0)
                TurnEntity(n\obj3,20.0*timing\tickDuration,0,0)

                TurnEntity(n\collider,0,-timing\tickDuration*7,0);Sin(TimeInPosMilliSecs()/40)*timing\tickDuration)
                n\state2=n\state2+timing\tickDuration*0.3

                target=CreatePivot()
                PositionEntity(target, n\enemyX, n\enemyY, n\enemyZ, True)

                PointEntity(n\obj, target)
                MoveEntity(n\obj, 0,0,timing\tickDuration*0.001*n\state2)
                PositionEntity(n\collider, EntityX(n\obj), EntityY(n\obj), EntityZ(n\obj))

                If (EntityDistance(n\obj, target) <0.3) Then
					;TODO:
;                    If (TempSound2 <> 0) Then FreeSound(TempSound2 : TempSound2 = 0)
;                    TempSound2 = LoadSound("SFX/Character/Apache/Crash"+Rand(1,2)+".ogg")
;                    mainPlayer\camShake = Max(mainPlayer\camShake, 3.0)
;                    PlaySound2(TempSound2)
;                    n\state = 5
                EndIf

                FreeEntity(target)
            EndIf
    End Select

    PositionEntity(n\obj, EntityX(n\collider), EntityY(n\collider), EntityZ(n\collider))
    RotateEntity(n\obj, EntityPitch(n\collider), EntityYaw(n\collider), EntityRoll(n\collider), True)
End Function
;~IDEal Editor Parameters:
;~C#Blitz3D