# Hockey Mishap

For the course, CGRA 408 - Physically based rendering

### Team Members
- Jack Purvis - emperorpurvis@gmail.com - 300311934
- Florian Treier - florian.treier@gmx.net - 300414773
					
### Project Description
Composing a photorealistic scene rendered with pbrt where synthetic objects are indistinguishable from the one in the photo. To achieve this, we needed to implement a differential rendering algorithm, model the synthetic objects, apply material properties and render them. Furthermore we wanted to animate the synthetic objects and insert some special effect.
We decided to attempt an outdoor scene showing a hockey gear. The real objects were a hockey stick, ball and two cones. The synthetic objects in the scene were also two cones, a ball and a hockey stick. 

### Content		
- MayaProject:	Folder containing the maya project consisting of the maya scene file and textures.
- pbrt-v3:		Folder with a clone of pbrt-v3, extended to support differential rendering etc.
- scenes:		Folder which holds the .pbrt files for our scene and all the needed assets.
- SceneEditor:	Scene file editor and preview tool. In the subfolder Executable there is a runnable build (windows only). The sources are located in the subfolder OutputComaprison (Visual Studio Project).

### Usage
- PBRT Extensions
    - Command line parameters
		- `--fn <num>` or `-framenumber <num>`:	Specifies which frame shall be rendered, if not specified, the frame 0 is rendered (AnimationFile in scene files will be ignored)
		- `--tf <num>` or `-tempFiles <num>`:	Specifies how many of the temporary scene files will be written: 0 for all of them, 1 for just the reusable one and 2 for none.
	- Scene file format extensions
		- `AnimationFile`: Specifies an animation file with ConcatTransforms statements for animations. Has a filename (string) property.
		- `ScenePhoto`: Specifies the background image of the scene.  Has a filename (string) property.
		- `SynthSceneBegin` / `SynthSceneEnd`: All the objects in between the Begin-End tag are considered to be the synthetic objects.
- Maya Scripts
    - Object export script
        - `pbrt_model_exporter` can be run using a PyCharm - MayaCharm environment or by copying the script into Maya.
        - The `file_path` variable will need to be configured to your `/scenes` directory.
        - The `include_animations` variable disables or enables animation export for the selected object.
        - The `start_frame` and `end_frame` variables specify the frame range to be exported.
    - Material editor
        - `pbrt_material_editor` can be run using a PyCharm - MayaCharm environment or by copying the script into Maya.
        - The `PBRT Executable Path` and `Material Editor Path` will need to be configured in the user interface.
        - Click `Update` to render the current template scene with the configured material parameters.
        - To modify the scene itself, for example to display a different model, the `template.pbrt` file can be edited.
        
        
### Modified PBRT files
```
pbrt-v3/CMakeLists.txt
pbrt-v3/src/core/api.cpp
pbrt-v3/src/core/api.h
pbrt-v3/src/core/film.cpp
pbrt-v3/src/core/film.h
pbrt-v3/src/core/imageio.cpp
pbrt-v3/src/core/imageio.h
pbrt-v3/src/core/pbrt.h
pbrt-v3/src/core/pbrtlex.cpp
pbrt-v3/src/core/pbrtlex.ll
pbrt-v3/src/core/pbrtparse.cpp
pbrt-v3/src/core/pbrtparse.y
pbrt-v3/src/core/primitive.cpp
pbrt-v3/src/core/primitive.h
pbrt-v3/src/ext/stringExtensions.h
pbrt-v3/src/main/pbrt.cpp
```