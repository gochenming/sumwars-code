
-- initialises a dialog in the most common way
function initDialog()
	createDialogue();
	setTopicBase("global");
	addAllPlayersToDialog();
	addStandardRoles();
end;
--pitch black darkness
function fadeOut(millisecs)
	setLight("ambient",{0,0,0},millisecs)
	setLight("directional",{0,0,0},millisecs)
	setLight("hero",{0,0,0},millisecs)
end;
--fades to a set of lightstrengthes
--lighttypes: {red,green,blue}
function fadeIn(millisecs, ambient,hero,directional)
	setLight("ambient",ambient,millisecs)
	setLight("directional",directional,millisecs)
	setLight("hero",hero,millisecs)
end;

--imitates a earthquake by shaking the camera repeatedly between two locations
-- loc1, loc2: the locations to shake between
-- cameraPosition: {lookAtAngle(phi),lookDownAngle(theta), distance}
-- iterations: every iteration is one movement loc2 -> loc1
function quake(loc1, loc2, cameraposition, iterations)
	local limit = 20;
	limit = iterations
	for i = 1, iterations do
		addCameraPosition(100, loc2, cameraposition[1],cameraposition[2],cameraposition[3]);
		addCameraPosition(100, loc1, cameraposition[1],cameraposition[2],cameraposition[3]);
	end;
end;


