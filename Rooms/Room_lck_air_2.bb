Function FillRoom_lck_air_2(r.Rooms)
    Local d.Doors, d2.Doors, sc.SecurityCams, de.Decals, r2.Rooms, sc2.SecurityCams
    Local it.Items, i%
    Local xtemp%, ytemp%, ztemp%
    
    Local t1;, Bump

    If r\RoomTemplate\Name = "room2gw_b"
        r\Objects[2] = CreatePivot(r\obj)
        PositionEntity (r\Objects[2], r\x - 156.825*RoomScale, -37.3458*RoomScale, r\z+121.364*RoomScale, True)
        
        de.Decals = CreateDecal(3,  r\x - 156.825*RoomScale, -37.3458*RoomScale, r\z+121.364*RoomScale,90,Rnd(360),0)
        de\Size = 0.5
        ScaleSprite(de\obj, de\Size,de\Size)
        EntityParent de\obj, r\obj
    EndIf
    
    r\RoomDoors[0] = CreateDoor(r\zone, r\x + 336.0 * RoomScale, 0.0, r\z - 382.0 * RoomScale, 0, r, False, False)
    PositionEntity(r\RoomDoors[0]\buttons[0], r\x + 580.822 * RoomScale, EntityY(r\RoomDoors[0]\buttons[0],True), r\z - 606.679 * RoomScale, True)	
    PositionEntity(r\RoomDoors[0]\buttons[1], r\x + 580.822 * RoomScale, EntityY(r\RoomDoors[0]\buttons[1],True), r\z - 606.679 * RoomScale, True)
    r\RoomDoors[0]\dir = 0 : r\RoomDoors[0]\AutoClose = False	: r\RoomDoors[0]\open = True  : r\RoomDoors[0]\locked = True	
    r\RoomDoors[0]\MTFClose = False
    
    r\RoomDoors[1] = CreateDoor(r\zone, r\x + 336.0 * RoomScale, 0.0, r\z + 462.0 * RoomScale, 180, r, False, False)
    PositionEntity(r\RoomDoors[1]\buttons[0], r\x + 580.822 * RoomScale, EntityY(r\RoomDoors[1]\buttons[0],True), r\z - 606.679 * RoomScale, True)	
    PositionEntity(r\RoomDoors[1]\buttons[1], r\x + 580.822 * RoomScale, EntityY(r\RoomDoors[1]\buttons[1],True), r\z - 606.679 * RoomScale, True)
    r\RoomDoors[1]\dir = 0 : r\RoomDoors[1]\AutoClose = False	: r\RoomDoors[1]\open = True  : r\RoomDoors[1]\locked = True
    r\RoomDoors[1]\MTFClose = False
    
    For r2.Rooms = Each Rooms
        If r2<>r Then
            If r2\RoomTemplate\Name = "lck_air_2" Or r2\RoomTemplate\Name = "lck_air_broke_2" Then
                r\Objects[3] = CopyEntity(r2\Objects[3],r\obj) ;don't load the mesh again
                Exit
            EndIf
        EndIf
    Next
    If r\Objects[3]=0 Then r\Objects[3] = LoadMesh_Strict("GFX\map\room2gw_pipes.b3d",r\obj)
    EntityPickMode r\Objects[3],2
    
    If r\RoomTemplate\Name = "lck_air_2"
        r\Objects[0] = CreatePivot()
        ;PositionEntity r\Objects[0],r\x-48.0*RoomScale,128.0*RoomScale,r\z+320.0*RoomScale
        PositionEntity r\Objects[0],r\x+344.0*RoomScale,128.0*RoomScale,r\z
        EntityParent r\Objects[0],r\obj
        
        Local bd_temp% = False
        If room2gw_brokendoor
            If room2gw_x = r\x
                If room2gw_z = r\z
                    bd_temp% = True
                EndIf
            EndIf
        EndIf
        
        If (room2gw_brokendoor = 0 And Rand(1,2)=1) Or bd_temp%
            r\Objects[1] = CopyEntity(DoorOBJ)
            ScaleEntity(r\Objects[1], (204.0 * RoomScale) / MeshWidth(r\Objects[1]), 312.0 * RoomScale / MeshHeight(r\Objects[1]), 16.0 * RoomScale / MeshDepth(r\Objects[1]))
            EntityType r\Objects[1], HIT_MAP
            PositionEntity r\Objects[1], r\x + 336.0 * RoomScale, 0.0, r\z + 462.0 * RoomScale
            RotateEntity(r\Objects[1], 0, 180 + 180, 0)
            EntityParent(r\Objects[1], r\obj)
            MoveEntity r\Objects[1],120.0,0,5.0
            room2gw_brokendoor = True
            room2gw_x# = r\x
            room2gw_z# = r\z
            FreeEntity r\RoomDoors[1]\obj2 : r\RoomDoors[1]\obj2 = 0
        EndIf
    EndIf
End Function


Function UpdateEventRoom_gw(e.Events)
	Local dist#, i%, temp%, pvt%, strtemp$, j%, k%

	Local p.Particles, n.NPCs, r.Rooms, e2.Events, it.Items, em.Emitters, sc.SecurityCams, sc2.SecurityCams

	Local CurrTrigger$ = ""

	Local x#, y#, z#

	Local angle#

	;[Block]
	;e\EventState: Determines if the airlock is in operation or not
	;e\EventState2: The timer for the airlocks
	;e\EventState3: Checks if the player had left the airlock or not
	
	e\room\RoomDoors[0]\locked = True
	e\room\RoomDoors[1]\locked = True
	
	Local brokendoor% = False
	If e\room\Objects[1]<>0 Then brokendoor% = True
	
	If mainPlayer\currRoom = e\room
		If e\EventState = 0.0
			If EntityDistance(e\room\Objects[0],mainPlayer\collider)<1.4 And e\EventState3 = 0.0
				e\EventState = 1.0
				If brokendoor
					If e\Sound2 <> 0 Then FreeSound_Strict(e\Sound2) : e\Sound2 = 0
					e\Sound2 = LoadSound_Strict("SFX\Door\DoorSparks.ogg")
					e\SoundCHN2 = PlaySound2(e\Sound2,mainPlayer\cam,e\room\Objects[1],5)
				EndIf
				StopChannel e\SoundCHN
				e\SoundCHN = 0
				If e\Sound <> 0 Then FreeSound_Strict(e\Sound) : e\Sound = 0
				e\Sound = LoadSound_Strict("SFX\Door\Airlock.ogg")
				e\room\RoomDoors[0]\locked = False
				e\room\RoomDoors[1]\locked = False
				UseDoor(e\room\RoomDoors[0])
				UseDoor(e\room\RoomDoors[1])
			ElseIf EntityDistance(e\room\Objects[0],mainPlayer\collider)>2.4
				e\EventState3 = 0.0
			EndIf
		Else
			If e\EventState2 < 70*7
				e\EventState2 = e\EventState2 + timing\tickDuration
				e\room\RoomDoors[0]\open = False
				e\room\RoomDoors[1]\open = False
				If e\EventState2 < 70*1
					
					If brokendoor
						pvt% = CreatePivot()
						Local d_ent% = e\room\Objects[1]
						PositionEntity(pvt, EntityX(d_ent%,True), EntityY(d_ent%,True)+Rnd(0.0,0.05), EntityZ(d_ent%,True))
						RotateEntity(pvt, 0, EntityYaw(d_ent%,True)+90, 0)
						MoveEntity pvt,0,0,0.2
						
						For i = 0 To 3									
							p.Particles = CreateParticle(EntityX(pvt), EntityY(pvt), EntityZ(pvt), 7, 0.002, 0, 25)
							p\speed = Rnd(0.01,0.05)
							;RotateEntity(p\pvt, Rnd(-20, 20), Rnd(360), 0)
							RotateEntity(p\pvt, Rnd(-45,0), EntityYaw(pvt)+Rnd(-10.0,10.0), 0)
							
							p\size = 0.0075
							ScaleSprite p\obj,p\size,p\size
							
							;EntityOrder p\obj,-1
							
							p\Achange = -0.05
						Next
						
						FreeEntity pvt
					EndIf
						
				ElseIf e\EventState2 > 70*3 And e\EventState < 70*5.5
					pvt% = CreatePivot(e\room\obj)								
					For i = 0 To 1
						If e\room\RoomTemplate\Name$ = "lck_ez_3"
							If i = 0
								PositionEntity pvt%,-288.0,416.0,320.0,False
							Else
								PositionEntity pvt%,192.0,416.0,320.0,False
							EndIf
						Else
							If i = 0
								PositionEntity pvt%,312.0,416.0,-128.0,False
							Else
								PositionEntity pvt%,312.0,416.0,224.0,False
							EndIf
						EndIf
						
						p.Particles = CreateParticle(EntityX(pvt,True), EntityY(pvt,True), EntityZ(pvt,True),  6, 0.8, 0, 50)
						p\speed = 0.025
						RotateEntity(p\pvt, 90, 0, 0)
						
						p\Achange = -0.02
					Next
					
					FreeEntity pvt
					If e\SoundCHN = 0 Then e\SoundCHN = PlaySound2(e\Sound,mainPlayer\cam,e\room\Objects[0],5)
				EndIf
			Else
				e\EventState = 0.0
				e\EventState2 = 0.0
				e\EventState3 = 1.0
				If e\room\RoomDoors[0]\open = False
					e\room\RoomDoors[0]\locked = False
					e\room\RoomDoors[1]\locked = False
					UseDoor(e\room\RoomDoors[0])
					UseDoor(e\room\RoomDoors[1])
				EndIf
			EndIf
		EndIf
		
		If brokendoor
			If ChannelPlaying(e\SoundCHN2)
				UpdateSoundOrigin(e\SoundCHN2,mainPlayer\cam,e\room\Objects[1],5)
			EndIf
		EndIf
		If ChannelPlaying(e\SoundCHN)
			UpdateSoundOrigin(e\SoundCHN,mainPlayer\cam,e\room\Objects[0],5)
		EndIf
	Else
		e\EventState3 = 0.0
	EndIf
	;[End Block]
End Function



Function UpdateEventRoom2gw_b(e.Events)
	Local dist#, i%, temp%, pvt%, strtemp$, j%, k%

	Local p.Particles, n.NPCs, r.Rooms, e2.Events, it.Items, em.Emitters, sc.SecurityCams, sc2.SecurityCams

	Local CurrTrigger$ = ""

	Local x#, y#, z#

	Local angle#

	;[Block]
	If e\room\dist < 8
		If e\EventState = 0 Then
			e\room\NPC[0]=CreateNPC(NPCtypeGuard, EntityX(e\room\Objects[2],True), EntityY(e\room\Objects[2],True)+0.5, EntityZ(e\room\Objects[2],True))
			PointEntity e\room\NPC[0]\Collider, e\room\obj
			RotateEntity e\room\NPC[0]\Collider, 0, EntityYaw(e\room\NPC[0]\Collider),0, True
			SetNPCFrame(e\room\NPC[0], 906)
			e\room\NPC[0]\State = 8
			
			e\EventState = 1
		EndIf
	EndIf
	;[End Block]
End Function


;~IDEal Editor Parameters:
;~C#Blitz3D