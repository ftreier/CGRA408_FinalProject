Course			CGRA 408 - Phisically based rendering
Project title	Hockey Mishap
Team Members	Jack Purvis		emperorpurvis@gmail.com		300311934
				Florian Treier	florian.treier@gmx.net		300414773					
Project Description	
	Composing a photorealistic scen rendered with pbrt where synthetic objects are indistinguishable from the one in the photo. To achieve this, we needed to implement a differential rendering algorithm, model the synthetic objects, apply material properties and render them. Furthermore we wanted to animate the synthetic objects and insert some special effect.
	We decided to attempt an outdoor scene showing a hockey gear. The real objects were a hockey stick, ball and two cones. The synthetic objects in the scene were also two cones, a ball and a hockey stick. 

Content			- MayaProject:	Folder containing models and animations
				- pbrt-v3:		Folder with a clone of pbrt-v3, extended to support differential rendering etc.
				- scenes:		Folder which holds the .pbrt files for our scene and all the needed assets
				- SceneEditor:	Scene file editor and preview tool. In the subfolder Executable there is a runable build (windows only). The sources are located in the subfolder OutputComaprison (Visual Studio Project).
				- WinTools:		Bison and Flex for windows to parse the pbrtparse.y and pbrtlex.ll files.
				
Usage			PBRT can be used as normal
Extensions
	- Command line parameters
		--fn or -framenumber <num>:	Specifies wich frame shall be rendered, if not specified, the frame 0 is rendered (AnimationFile in scene files will be ignored)
		--tf or -tempFiles <num>:	Specifies how many of the temproary scene files will be written: 0 for all of them, 1 for just the reusable one and 2 for none.
	- Scene file format extensions
		- AnimationFile: Specifies an animation file with ConcatTransforms statements for animations. Has a filename (string) property.
		- ScenePhoto: Specifies the background image of the scene.  Has a filename (string) property.
		- SynthSceneBegin/End: All the objects in between the Begin-End tag are considered to be the synthetic objects.