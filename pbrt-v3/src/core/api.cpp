/*
	pbrt source code is Copyright(c) 1998-2016
						Matt Pharr, Greg Humphreys, and Wenzel Jakob.

	This file is part of pbrt.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are
	met:

	- Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.

	- Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
	IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
	TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
	PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

// core/api.cpp*
#include "api.h"
#include "parallel.h"
#include "paramset.h"
#include "spectrum.h"
#include "scene.h"
#include "film.h"
#include "medium.h"
#include "stats.h"

// API Additional Headers
#include "accelerators/bvh.h"
#include "accelerators/kdtreeaccel.h"
#include "cameras/environment.h"
#include "cameras/orthographic.h"
#include "cameras/perspective.h"
#include "cameras/realistic.h"
#include "filters/box.h"
#include "filters/gaussian.h"
#include "filters/mitchell.h"
#include "filters/sinc.h"
#include "filters/triangle.h"
#include "integrators/bdpt.h"
#include "integrators/directlighting.h"
#include "integrators/mlt.h"
#include "integrators/ao.h"
#include "integrators/path.h"
#include "integrators/sppm.h"
#include "integrators/volpath.h"
#include "integrators/whitted.h"
#include "lights/diffuse.h"
#include "lights/distant.h"
#include "lights/goniometric.h"
#include "lights/infinite.h"
#include "lights/point.h"
#include "lights/projection.h"
#include "lights/spot.h"
#include "materials/disney.h"
#include "materials/fourier.h"
#include "materials/glass.h"
#include "materials/hair.h"
#include "materials/kdsubsurface.h"
#include "materials/matte.h"
#include "materials/metal.h"
#include "materials/mirror.h"
#include "materials/mixmat.h"
#include "materials/plastic.h"
#include "materials/substrate.h"
#include "materials/subsurface.h"
#include "materials/translucent.h"
#include "materials/uber.h"
#include "samplers/halton.h"
#include "samplers/maxmin.h"
#include "samplers/random.h"
#include "samplers/sobol.h"
#include "samplers/stratified.h"
#include "samplers/zerotwosequence.h"
#include "shapes/cone.h"
#include "shapes/curve.h"
#include "shapes/cylinder.h"
#include "shapes/disk.h"
#include "shapes/heightfield.h"
#include "shapes/hyperboloid.h"
#include "shapes/loopsubdiv.h"
#include "shapes/nurbs.h"
#include "shapes/paraboloid.h"
#include "shapes/sphere.h"
#include "shapes/triangle.h"
#include "shapes/plymesh.h"
#include "textures/bilerp.h"
#include "textures/checkerboard.h"
#include "textures/constant.h"
#include "textures/dots.h"
#include "textures/fbm.h"
#include "textures/imagemap.h"
#include "textures/marble.h"
#include "textures/mix.h"
#include "textures/ptex.h"
#include "textures/scale.h"
#include "textures/uv.h"
#include "textures/windy.h"
#include "textures/wrinkled.h"
#include "media/grid.h"
#include "media/homogeneous.h"
#include <map>
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "ext/stringExtensions.h"
#include <iomanip>

using namespace std;

namespace pbrt {

// API Global Variables
Options PbrtOptions;

// API Local Classes
PBRT_CONSTEXPR int MaxTransforms = 2;
PBRT_CONSTEXPR int StartTransformBits = 1 << 0;
PBRT_CONSTEXPR int EndTransformBits = 1 << 1;
PBRT_CONSTEXPR int AllTransformsBits = (1 << MaxTransforms) - 1;
struct TransformSet {
	// TransformSet Public Methods
	Transform &operator[](int i) {
		CHECK_GE(i, 0);
		CHECK_LT(i, MaxTransforms);
		return t[i];
	}
	const Transform &operator[](int i) const {
		CHECK_GE(i, 0);
		CHECK_LT(i, MaxTransforms);
		return t[i];
	}
	friend TransformSet Inverse(const TransformSet &ts) {
		TransformSet tInv;
		for (int i = 0; i < MaxTransforms; ++i) tInv.t[i] = Inverse(ts.t[i]);
		return tInv;
	}
	bool IsAnimated() const {
		for (int i = 0; i < MaxTransforms - 1; ++i)
			if (t[i] != t[i + 1]) return true;
		return false;
	}

  private:
	Transform t[MaxTransforms];
};

struct RenderOptions {
	// RenderOptions Public Methods
	Integrator *MakeIntegrator() const;
	Scene *MakeScene(bool includeLocal, bool includeSynth);
	Camera *MakeCamera() const;

	// RenderOptions Public Data
	Float transformStartTime = 0, transformEndTime = 1;
	string FilterName = "box";
	ParamSet FilterParams;
	string FilmName = "image";
	ParamSet FilmParams;
	string SamplerName = "halton";
	ParamSet SamplerParams;
	string AcceleratorName = "bvh";
	ParamSet AcceleratorParams;
	string IntegratorName = "path";
	ParamSet IntegratorParams;
	string CameraName = "perspective";
	ParamSet CameraParams;
	TransformSet CameraToWorld;
	map<string, shared_ptr<Medium>> namedMedia;
	vector<shared_ptr<Light>> lights;
	vector<shared_ptr<Primitive>> primitives;
	map<string, vector<shared_ptr<Primitive>>> instances;
	vector<shared_ptr<Primitive>> *currentInstance = nullptr;
	bool haveScatteringMedia = false;

	// Differential rendering properties
	ParamSet _differentialBg;
	float* _destBuffer;
	//bool diffRendering;
};

struct GraphicsState {
	// Graphics State Methods
	shared_ptr<Material> CreateMaterial(const ParamSet &params);
	MediumInterface CreateMediumInterface();

	// Graphics State
	string currentInsideMedium, currentOutsideMedium;
	map<string, shared_ptr<Texture<Float>>> floatTextures;
	map<string, shared_ptr<Texture<Spectrum>>> spectrumTextures;
	ParamSet materialParams;
	string material = "matte";
	map<string, shared_ptr<Material>> namedMaterials;
	string currentNamedMaterial;
	ParamSet areaLightParams;
	string areaLight;
	bool reverseOrientation = false;
};

class TransformCache {
  public:
	// TransformCache Public Methods
	void Lookup(const Transform &t, Transform **tCached,
				Transform **tCachedInverse) {
		auto iter = cache.find(t);
		if (iter == cache.end()) {
			Transform *tr = arena.Alloc<Transform>();
			*tr = t;
			Transform *tinv = arena.Alloc<Transform>();
			*tinv = Transform(Inverse(t));
			cache[t] = make_pair(tr, tinv);
			iter = cache.find(t);
		}
		if (tCached) *tCached = iter->second.first;
		if (tCachedInverse) *tCachedInverse = iter->second.second;
	}
	void Clear() {
		arena.Reset();
		cache.erase(cache.begin(), cache.end());
	}

  private:
	// TransformCache Private Data
	map<Transform, pair<Transform *, Transform *>> cache;
	MemoryArena arena;
};

// API Static Data
enum class APIState { Uninitialized, OptionsBlock, WorldBlock };
static APIState currentApiState = APIState::Uninitialized;
static TransformSet curTransform;
static uint32_t activeTransformBits = AllTransformsBits;
static map<string, TransformSet> namedCoordinateSystems;
static unique_ptr<RenderOptions> renderOptions;
static GraphicsState graphicsState;
static vector<GraphicsState> pushedGraphicsStates;
static vector<TransformSet> pushedTransforms;
static vector<uint32_t> pushedActiveTransformBits;
static TransformCache transformCache;
int catIndentCount = 0;

bool _isSynthScene = false;
static const int _noOfChannels = 3;

// API Forward Declarations
vector<shared_ptr<Shape>> MakeShapes(const string &name,
									 const Transform *ObjectToWorld,
									 const Transform *WorldToObject,
									 bool reverseOrientation,
									 const ParamSet &paramSet);

// API Macros
#define VERIFY_INITIALIZED(func)                           \
	if (!(PbrtOptions.cat || PbrtOptions.toPly) &&           \
		currentApiState == APIState::Uninitialized) {        \
		Error(                                             \
			"pbrtInit() must be before calling \"%s()\". " \
			"Ignoring.",                                   \
			func);                                         \
		return;                                            \
	} else /* swallow trailing semicolon */
#define VERIFY_OPTIONS(func)                             \
	VERIFY_INITIALIZED(func);                            \
	if (!(PbrtOptions.cat || PbrtOptions.toPly) &&       \
		currentApiState == APIState::WorldBlock) {       \
		Error(                                           \
			"Options cannot be set inside world block; " \
			"\"%s\" not allowed.  Ignoring.",            \
			func);                                       \
		return;                                          \
	} else /* swallow trailing semicolon */
#define VERIFY_WORLD(func)                                   \
	VERIFY_INITIALIZED(func);                                \
	if (!(PbrtOptions.cat || PbrtOptions.toPly) &&           \
		currentApiState == APIState::OptionsBlock) {         \
		Error(                                               \
			"Scene description must be inside world block; " \
			"\"%s\" not allowed. Ignoring.",                 \
			func);                                           \
		return;                                              \
	} else /* swallow trailing semicolon */
#define FOR_ACTIVE_TRANSFORMS(expr)           \
	for (int i = 0; i < MaxTransforms; ++i)   \
		if (activeTransformBits & (1 << i)) { \
			expr                              \
		}
#define WARN_IF_ANIMATED_TRANSFORM(func)                             \
	do {                                                             \
		if (curTransform.IsAnimated())                               \
			Warning(                                                 \
				"Animated transformations set; ignoring for \"%s\" " \
				"and using the start transform only",                \
				func);                                               \
	} while (false) /* swallow trailing semicolon */

// Object Creation Function Definitions
vector<shared_ptr<Shape>> MakeShapes(const string &name,
									 const Transform *object2world,
									 const Transform *world2object,
									 bool reverseOrientation,
									 const ParamSet &paramSet) {
	vector<shared_ptr<Shape>> shapes;
	shared_ptr<Shape> s;
	if (name == "sphere")
		s = CreateSphereShape(object2world, world2object, reverseOrientation,
							  paramSet);
	// Create remaining single _Shape_ types
	else if (name == "cylinder")
		s = CreateCylinderShape(object2world, world2object, reverseOrientation,
								paramSet);
	else if (name == "disk")
		s = CreateDiskShape(object2world, world2object, reverseOrientation,
							paramSet);
	else if (name == "cone")
		s = CreateConeShape(object2world, world2object, reverseOrientation,
							paramSet);
	else if (name == "paraboloid")
		s = CreateParaboloidShape(object2world, world2object,
								  reverseOrientation, paramSet);
	else if (name == "hyperboloid")
		s = CreateHyperboloidShape(object2world, world2object,
								   reverseOrientation, paramSet);
	if (s != nullptr) shapes.push_back(s);

	// Create multiple-_Shape_ types
	else if (name == "curve")
		shapes = CreateCurveShape(object2world, world2object,
								  reverseOrientation, paramSet);
	else if (name == "trianglemesh") {
		if (PbrtOptions.toPly) {
			static int count = 1;
			const char *plyPrefix =
				getenv("PLY_PREFIX") ? getenv("PLY_PREFIX") : "mesh";
			string fn = StringPrintf("%s_%05d.ply", plyPrefix, count++);

			int nvi, npi, nuvi, nsi, nni;
			const int *vi = paramSet.FindInt("indices", &nvi);
			const Point3f *P = paramSet.FindPoint3f("P", &npi);
			const Point2f *uvs = paramSet.FindPoint2f("uv", &nuvi);
			if (!uvs) uvs = paramSet.FindPoint2f("st", &nuvi);
			vector<Point2f> tempUVs;
			if (!uvs) {
				const Float *fuv = paramSet.FindFloat("uv", &nuvi);
				if (!fuv) fuv = paramSet.FindFloat("st", &nuvi);
				if (fuv) {
					nuvi /= 2;
					tempUVs.reserve(nuvi);
					for (int i = 0; i < nuvi; ++i)
						tempUVs.push_back(Point2f(fuv[2 * i], fuv[2 * i + 1]));
					uvs = &tempUVs[0];
				}
			}
			const Normal3f *N = paramSet.FindNormal3f("N", &nni);
			const Vector3f *S = paramSet.FindVector3f("S", &nsi);

			if (!WritePlyFile(fn.c_str(), nvi / 3, vi, npi, P, S, N, uvs))
				Error("Unable to write PLY file \"%s\"", fn.c_str());

			printf("%*sShape \"plymesh\" \"string filename\" \"%s\" ",
				   catIndentCount, "", fn.c_str());

			string alphaTex = paramSet.FindTexture("alpha");
			if (alphaTex != "")
				printf("\n%*s\"texture alpha\" \"%s\" ", catIndentCount + 8, "",
					   alphaTex.c_str());
			else {
				int count;
				const Float *alpha = paramSet.FindFloat("alpha", &count);
				if (alpha)
					printf("\n%*s\"float alpha\" %f ", catIndentCount + 8, "",
						   *alpha);
			}

			string shadowAlphaTex = paramSet.FindTexture("shadowalpha");
			if (shadowAlphaTex != "")
				printf("\n%*s\"texture shadowalpha\" \"%s\" ",
					   catIndentCount + 8, "", shadowAlphaTex.c_str());
			else {
				int count;
				const Float *alpha = paramSet.FindFloat("shadowalpha", &count);
				if (alpha)
					printf("\n%*s\"float shadowalpha\" %f ", catIndentCount + 8,
						   "", *alpha);
			}
			printf("\n");
		} else
			shapes = CreateTriangleMeshShape(object2world, world2object,
											 reverseOrientation, paramSet,
											 &graphicsState.floatTextures);
	} else if (name == "plymesh")
		shapes = CreatePLYMesh(object2world, world2object, reverseOrientation,
							   paramSet, &graphicsState.floatTextures);
	else if (name == "heightfield")
		shapes = CreateHeightfield(object2world, world2object,
								   reverseOrientation, paramSet);
	else if (name == "loopsubdiv")
		shapes = CreateLoopSubdiv(object2world, world2object,
								  reverseOrientation, paramSet);
	else if (name == "nurbs")
		shapes = CreateNURBS(object2world, world2object, reverseOrientation,
							 paramSet);
	else
		Warning("Shape \"%s\" unknown.", name.c_str());
	paramSet.ReportUnused();
	return shapes;
}

STAT_COUNTER("Scene/Materials created", nMaterialsCreated);

shared_ptr<Material> MakeMaterial(const string &name, const TextureParams &mp) {
	Material *material = nullptr;
	if (name == "" || name == "none")
		return nullptr;
	else if (name == "matte")
		material = CreateMatteMaterial(mp);
	else if (name == "plastic")
		material = CreatePlasticMaterial(mp);
	else if (name == "translucent")
		material = CreateTranslucentMaterial(mp);
	else if (name == "glass")
		material = CreateGlassMaterial(mp);
	else if (name == "mirror")
		material = CreateMirrorMaterial(mp);
	else if (name == "hair")
		material = CreateHairMaterial(mp);
	else if (name == "disney")
		material = CreateDisneyMaterial(mp);
	else if (name == "mix") {
		string m1 = mp.FindString("namedmaterial1", "");
		string m2 = mp.FindString("namedmaterial2", "");
		shared_ptr<Material> mat1 = graphicsState.namedMaterials[m1];
		shared_ptr<Material> mat2 = graphicsState.namedMaterials[m2];
		if (!mat1) {
			Error("Named material \"%s\" undefined.  Using \"matte\"",
				  m1.c_str());
			mat1 = MakeMaterial("matte", mp);
		}
		if (!mat2) {
			Error("Named material \"%s\" undefined.  Using \"matte\"",
				  m2.c_str());
			mat2 = MakeMaterial("matte", mp);
		}

		material = CreateMixMaterial(mp, mat1, mat2);
	} else if (name == "metal")
		material = CreateMetalMaterial(mp);
	else if (name == "substrate")
		material = CreateSubstrateMaterial(mp);
	else if (name == "uber")
		material = CreateUberMaterial(mp);
	else if (name == "subsurface")
		material = CreateSubsurfaceMaterial(mp);
	else if (name == "kdsubsurface")
		material = CreateKdSubsurfaceMaterial(mp);
	else if (name == "fourier")
		material = CreateFourierMaterial(mp);
	else {
		Warning("Material \"%s\" unknown. Using \"matte\".", name.c_str());
		material = CreateMatteMaterial(mp);
	}

	if ((name == "subsurface" || name == "kdsubsurface") &&
		(renderOptions->IntegratorName != "path" &&
		 (renderOptions->IntegratorName != "volpath")))
		Warning(
			"Subsurface scattering material \"%s\" used, but \"%s\" "
			"integrator doesn't support subsurface scattering. "
			"Use \"path\" or \"volpath\".",
			name.c_str(), renderOptions->IntegratorName.c_str());

	mp.ReportUnused();
	if (!material) Error("Unable to create material \"%s\"", name.c_str());
	else ++nMaterialsCreated;
	return shared_ptr<Material>(material);
}

shared_ptr<Texture<Float>> MakeFloatTexture(const string &name,
											const Transform &tex2world,
											const TextureParams &tp) {
	Texture<Float> *tex = nullptr;
	if (name == "constant")
		tex = CreateConstantFloatTexture(tex2world, tp);
	else if (name == "scale")
		tex = CreateScaleFloatTexture(tex2world, tp);
	else if (name == "mix")
		tex = CreateMixFloatTexture(tex2world, tp);
	else if (name == "bilerp")
		tex = CreateBilerpFloatTexture(tex2world, tp);
	else if (name == "imagemap")
		tex = CreateImageFloatTexture(tex2world, tp);
	else if (name == "uv")
		tex = CreateUVFloatTexture(tex2world, tp);
	else if (name == "checkerboard")
		tex = CreateCheckerboardFloatTexture(tex2world, tp);
	else if (name == "dots")
		tex = CreateDotsFloatTexture(tex2world, tp);
	else if (name == "fbm")
		tex = CreateFBmFloatTexture(tex2world, tp);
	else if (name == "wrinkled")
		tex = CreateWrinkledFloatTexture(tex2world, tp);
	else if (name == "marble")
		tex = CreateMarbleFloatTexture(tex2world, tp);
	else if (name == "windy")
		tex = CreateWindyFloatTexture(tex2world, tp);
	else if (name == "ptex")
		tex = CreatePtexFloatTexture(tex2world, tp);
	else
		Warning("Float texture \"%s\" unknown.", name.c_str());
	tp.ReportUnused();
	return shared_ptr<Texture<Float>>(tex);
}

shared_ptr<Texture<Spectrum>> MakeSpectrumTexture(
	const string &name, const Transform &tex2world,
	const TextureParams &tp) {
	Texture<Spectrum> *tex = nullptr;
	if (name == "constant")
		tex = CreateConstantSpectrumTexture(tex2world, tp);
	else if (name == "scale")
		tex = CreateScaleSpectrumTexture(tex2world, tp);
	else if (name == "mix")
		tex = CreateMixSpectrumTexture(tex2world, tp);
	else if (name == "bilerp")
		tex = CreateBilerpSpectrumTexture(tex2world, tp);
	else if (name == "imagemap")
		tex = CreateImageSpectrumTexture(tex2world, tp);
	else if (name == "uv")
		tex = CreateUVSpectrumTexture(tex2world, tp);
	else if (name == "checkerboard")
		tex = CreateCheckerboardSpectrumTexture(tex2world, tp);
	else if (name == "dots")
		tex = CreateDotsSpectrumTexture(tex2world, tp);
	else if (name == "fbm")
		tex = CreateFBmSpectrumTexture(tex2world, tp);
	else if (name == "wrinkled")
		tex = CreateWrinkledSpectrumTexture(tex2world, tp);
	else if (name == "marble")
		tex = CreateMarbleSpectrumTexture(tex2world, tp);
	else if (name == "windy")
		tex = CreateWindySpectrumTexture(tex2world, tp);
	else if (name == "ptex")
		tex = CreatePtexSpectrumTexture(tex2world, tp);
	else
		Warning("Spectrum texture \"%s\" unknown.", name.c_str());
	tp.ReportUnused();
	return shared_ptr<Texture<Spectrum>>(tex);
}

shared_ptr<Medium> MakeMedium(const string &name,
							  const ParamSet &paramSet,
							  const Transform &medium2world) {
	Float sig_a_rgb[3] = {.0011f, .0024f, .014f},
		  sig_s_rgb[3] = {2.55f, 3.21f, 3.77f};
	Spectrum sig_a = Spectrum::FromRGB(sig_a_rgb),
			 sig_s = Spectrum::FromRGB(sig_s_rgb);
	string preset = paramSet.FindOneString("preset", "");
	bool found = GetMediumScatteringProperties(preset, &sig_a, &sig_s);
	if (preset != "" && !found)
		Warning("Material preset \"%s\" not found.  Using defaults.",
				preset.c_str());
	Float scale = paramSet.FindOneFloat("scale", 1.f);
	Float g = paramSet.FindOneFloat("g", 0.0f);
	sig_a = paramSet.FindOneSpectrum("sigma_a", sig_a) * scale;
	sig_s = paramSet.FindOneSpectrum("sigma_s", sig_s) * scale;
	Medium *m = NULL;
	if (name == "homogeneous") {
		m = new HomogeneousMedium(sig_a, sig_s, g);
	} else if (name == "heterogeneous") {
		int nitems;
		const Float *data = paramSet.FindFloat("density", &nitems);
		if (!data) {
			Error("No \"density\" values provided for heterogeneous medium?");
			return NULL;
		}
		int nx = paramSet.FindOneInt("nx", 1);
		int ny = paramSet.FindOneInt("ny", 1);
		int nz = paramSet.FindOneInt("nz", 1);
		Point3f p0 = paramSet.FindOnePoint3f("p0", Point3f(0.f, 0.f, 0.f));
		Point3f p1 = paramSet.FindOnePoint3f("p1", Point3f(1.f, 1.f, 1.f));
		if (nitems != nx * ny * nz) {
			Error(
				"GridDensityMedium has %d density values; expected nx*ny*nz = "
				"%d",
				nitems, nx * ny * nz);
			return NULL;
		}
		Transform data2Medium = Translate(Vector3f(p0)) *
								Scale(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
		m = new GridDensityMedium(sig_a, sig_s, g, nx, ny, nz,
								  medium2world * data2Medium, data);
	} else
		Warning("Medium \"%s\" unknown.", name.c_str());
	paramSet.ReportUnused();
	return shared_ptr<Medium>(m);
}

shared_ptr<Light> MakeLight(const string &name,
							const ParamSet &paramSet,
							const Transform &light2world,
							const MediumInterface &mediumInterface) {
	shared_ptr<Light> light;
	if (name == "point")
		light =
			CreatePointLight(light2world, mediumInterface.outside, paramSet);
	else if (name == "spot")
		light = CreateSpotLight(light2world, mediumInterface.outside, paramSet);
	else if (name == "goniometric")
		light = CreateGoniometricLight(light2world, mediumInterface.outside,
									   paramSet);
	else if (name == "projection")
		light = CreateProjectionLight(light2world, mediumInterface.outside,
									  paramSet);
	else if (name == "distant")
		light = CreateDistantLight(light2world, paramSet);
	else if (name == "infinite" || name == "exinfinite")
		light = CreateInfiniteLight(light2world, paramSet);
	else
		Warning("Light \"%s\" unknown.", name.c_str());
	paramSet.ReportUnused();
	return light;
}

shared_ptr<AreaLight> MakeAreaLight(const string &name,
									const Transform &light2world,
									const MediumInterface &mediumInterface,
									const ParamSet &paramSet,
									const shared_ptr<Shape> &shape) {
	shared_ptr<AreaLight> area;
	if (name == "area" || name == "diffuse")
		area = CreateDiffuseAreaLight(light2world, mediumInterface.outside,
									  paramSet, shape);
	else
		Warning("Area light \"%s\" unknown.", name.c_str());
	paramSet.ReportUnused();
	return area;
}

shared_ptr<Primitive> MakeAccelerator(
	const string &name,
	const vector<shared_ptr<Primitive>> &prims,
	const ParamSet &paramSet) {
	shared_ptr<Primitive> accel;
	if (name == "bvh")
		accel = CreateBVHAccelerator(prims, paramSet);
	else if (name == "kdtree")
		accel = CreateKdTreeAccelerator(prims, paramSet);
	else
		Warning("Accelerator \"%s\" unknown.", name.c_str());
	paramSet.ReportUnused();
	return accel;
}

Camera *MakeCamera(const string &name, const ParamSet &paramSet,
				   const TransformSet &cam2worldSet, Float transformStart,
				   Float transformEnd, Film *film) {
	Camera *camera = nullptr;
	MediumInterface mediumInterface = graphicsState.CreateMediumInterface();
	static_assert(MaxTransforms == 2,
				  "TransformCache assumes only two transforms");
	Transform *cam2world[2];
	transformCache.Lookup(cam2worldSet[0], &cam2world[0], nullptr);
	transformCache.Lookup(cam2worldSet[1], &cam2world[1], nullptr);
	AnimatedTransform animatedCam2World(cam2world[0], transformStart,
										cam2world[1], transformEnd);
	if (name == "perspective")
		camera = CreatePerspectiveCamera(paramSet, animatedCam2World, film,
										 mediumInterface.outside);
	else if (name == "orthographic")
		camera = CreateOrthographicCamera(paramSet, animatedCam2World, film,
										  mediumInterface.outside);
	else if (name == "realistic")
		camera = CreateRealisticCamera(paramSet, animatedCam2World, film,
									   mediumInterface.outside);
	else if (name == "environment")
		camera = CreateEnvironmentCamera(paramSet, animatedCam2World, film,
										 mediumInterface.outside);
	else
		Warning("Camera \"%s\" unknown.", name.c_str());
	paramSet.ReportUnused();
	return camera;
}

shared_ptr<Sampler> MakeSampler(const string &name,
								const ParamSet &paramSet,
								const Film *film) {
	Sampler *sampler = nullptr;
	if (name == "lowdiscrepancy" || name == "02sequence")
		sampler = CreateZeroTwoSequenceSampler(paramSet);
	else if (name == "maxmindist")
		sampler = CreateMaxMinDistSampler(paramSet);
	else if (name == "halton")
		sampler = CreateHaltonSampler(paramSet, film->GetSampleBounds());
	else if (name == "sobol")
		sampler = CreateSobolSampler(paramSet, film->GetSampleBounds());
	else if (name == "random")
		sampler = CreateRandomSampler(paramSet);
	else if (name == "stratified")
		sampler = CreateStratifiedSampler(paramSet);
	else
		Warning("Sampler \"%s\" unknown.", name.c_str());
	paramSet.ReportUnused();
	return shared_ptr<Sampler>(sampler);
}

unique_ptr<Filter> MakeFilter(const string &name, const ParamSet &paramSet) {
	Filter *filter = nullptr;
	if (name == "box")
		filter = CreateBoxFilter(paramSet);
	else if (name == "gaussian")
		filter = CreateGaussianFilter(paramSet);
	else if (name == "mitchell")
		filter = CreateMitchellFilter(paramSet);
	else if (name == "sinc")
		filter = CreateSincFilter(paramSet);
	else if (name == "triangle")
		filter = CreateTriangleFilter(paramSet);
	else {
		Error("Filter \"%s\" unknown.", name.c_str());
		exit(1);
	}
	paramSet.ReportUnused();
	return unique_ptr<Filter>(filter);
}

Film *MakeFilm(const string &name, const ParamSet &paramSet, unique_ptr<Filter> filter)
{
	Film *film = nullptr;
	if (name == "image")
	{
		film = CreateFilm(paramSet, move(filter), renderOptions->_destBuffer);
	}
	else
	{
		Warning("Film \"%s\" unknown.", name.c_str());
	}
	
	paramSet.ReportUnused();
	return film;
}

// API Function Definitions
void pbrtInit(const Options &opt) {
	PbrtOptions = opt;
	// API Initialization
	if (currentApiState != APIState::Uninitialized)
		Error("pbrtInit() has already been called.");
	currentApiState = APIState::OptionsBlock;
	renderOptions.reset(new RenderOptions);
	graphicsState = GraphicsState();
	catIndentCount = 0;

	// General \pbrt Initialization
	SampledSpectrum::Init();
	ParallelInit();  // Threads must be launched before the profiler is
					 // initialized.
	InitProfiler();
}

void pbrtCleanup() {
	// API Cleanup
	if (currentApiState == APIState::Uninitialized)
		Error("pbrtCleanup() called without pbrtInit().");
	else if (currentApiState == APIState::WorldBlock)
		Error("pbrtCleanup() called while inside world block.");
	currentApiState = APIState::Uninitialized;
	ParallelCleanup();
	renderOptions.reset(nullptr);
	CleanupProfiler();
}

void pbrtIdentity() {
	VERIFY_INITIALIZED("Identity");
	FOR_ACTIVE_TRANSFORMS(curTransform[i] = Transform();)
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sIdentity\n", catIndentCount, "");
}

void pbrtTranslate(Float dx, Float dy, Float dz) {
	VERIFY_INITIALIZED("Translate");
	FOR_ACTIVE_TRANSFORMS(curTransform[i] = curTransform[i] *
											Translate(Vector3f(dx, dy, dz));)
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sTranslate %.9g %.9g %.9g\n", catIndentCount, "", dx, dy,
			   dz);
}

void pbrtTransform(Float tr[16]) {
	VERIFY_INITIALIZED("Transform");
	FOR_ACTIVE_TRANSFORMS(
		curTransform[i] = Transform(Matrix4x4(
			tr[0], tr[4], tr[8], tr[12], tr[1], tr[5], tr[9], tr[13], tr[2],
			tr[6], tr[10], tr[14], tr[3], tr[7], tr[11], tr[15]));)
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sTransform [ ", catIndentCount, "");
		for (int i = 0; i < 16; ++i) printf("%.9g ", tr[i]);
		printf(" ]\n");
	}
}

void pbrtConcatTransform(Float tr[16]) {
	VERIFY_INITIALIZED("ConcatTransform");
	FOR_ACTIVE_TRANSFORMS(
		curTransform[i] =
			curTransform[i] *
			Transform(Matrix4x4(tr[0], tr[4], tr[8], tr[12], tr[1], tr[5],
								tr[9], tr[13], tr[2], tr[6], tr[10], tr[14],
								tr[3], tr[7], tr[11], tr[15]));)
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sConcatTransform [ ", catIndentCount, "");
		for (int i = 0; i < 16; ++i) printf("%.9g ", tr[i]);
		printf(" ]\n");
	}
}

void pbrtRotate(Float angle, Float dx, Float dy, Float dz) {
	VERIFY_INITIALIZED("Rotate");
	FOR_ACTIVE_TRANSFORMS(curTransform[i] =
							  curTransform[i] *
							  Rotate(angle, Vector3f(dx, dy, dz));)
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sRotate %.9g %.9g %.9g %.9g\n", catIndentCount, "", angle,
			   dx, dy, dz);
}

void pbrtScale(Float sx, Float sy, Float sz) {
	VERIFY_INITIALIZED("Scale");
	FOR_ACTIVE_TRANSFORMS(curTransform[i] =
							  curTransform[i] * Scale(sx, sy, sz);)
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sScale %.9g %.9g %.9g\n", catIndentCount, "", sx, sy, sz);
}

void pbrtLookAt(Float ex, Float ey, Float ez, Float lx, Float ly, Float lz,
				Float ux, Float uy, Float uz) {
	VERIFY_INITIALIZED("LookAt");
	Transform lookAt =
		LookAt(Point3f(ex, ey, ez), Point3f(lx, ly, lz), Vector3f(ux, uy, uz));
	FOR_ACTIVE_TRANSFORMS(curTransform[i] = curTransform[i] * lookAt;);
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf(
			"%*sLookAt %.9g %.9g %.9g\n%*s%.9g %.9g %.9g\n"
			"%*s%.9g %.9g %.9g\n",
			catIndentCount, "", ex, ey, ez, catIndentCount + 8, "", lx, ly, lz,
			catIndentCount + 8, "", ux, uy, uz);
}

void pbrtCoordinateSystem(const string &name) {
	VERIFY_INITIALIZED("CoordinateSystem");
	namedCoordinateSystems[name] = curTransform;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sCoordinateSystem \"%s\"\n", catIndentCount, "",
			   name.c_str());
}

void pbrtCoordSysTransform(const string &name) {
	VERIFY_INITIALIZED("CoordSysTransform");
	if (namedCoordinateSystems.find(name) != namedCoordinateSystems.end())
		curTransform = namedCoordinateSystems[name];
	else
		Warning("Couldn't find named coordinate system \"%s\"", name.c_str());
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sCoordSysTransform \"%s\"\n", catIndentCount, "",
			   name.c_str());
}

void pbrtActiveTransformAll() {
	activeTransformBits = AllTransformsBits;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sActiveTransform All\n", catIndentCount, "");
}

void pbrtActiveTransformEndTime() {
	activeTransformBits = EndTransformBits;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sActiveTransform EndTime\n", catIndentCount, "");
}

void pbrtActiveTransformStartTime() {
	activeTransformBits = StartTransformBits;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sActiveTransform StartTime\n", catIndentCount, "");
}

void pbrtTransformTimes(Float start, Float end) {
	VERIFY_OPTIONS("TransformTimes");
	renderOptions->transformStartTime = start;
	renderOptions->transformEndTime = end;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sTransformTimes %.9g %.9g\n", catIndentCount, "", start,
			   end);
}

void pbrtPixelFilter(const string &name, const ParamSet &params) {
	VERIFY_OPTIONS("PixelFilter");
	renderOptions->FilterName = name;
	renderOptions->FilterParams = params;
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sPixelFilter \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtFilm(const string &type, const ParamSet &params) {
	VERIFY_OPTIONS("Film");
	renderOptions->FilmParams = params;
	renderOptions->FilmName = type;
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sFilm \"%s\" ", catIndentCount, "", type.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtSampler(const string &name, const ParamSet &params) {
	VERIFY_OPTIONS("Sampler");
	renderOptions->SamplerName = name;
	renderOptions->SamplerParams = params;
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sSampler \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtAccelerator(const string &name, const ParamSet &params) {
	VERIFY_OPTIONS("Accelerator");
	renderOptions->AcceleratorName = name;
	renderOptions->AcceleratorParams = params;
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sAccelerator \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtIntegrator(const string &name, const ParamSet &params) {
	VERIFY_OPTIONS("Integrator");
	renderOptions->IntegratorName = name;
	renderOptions->IntegratorParams = params;
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sIntegrator \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtCamera(const string &name, const ParamSet &params) {
	VERIFY_OPTIONS("Camera");
	renderOptions->CameraName = name;
	renderOptions->CameraParams = params;
	renderOptions->CameraToWorld = Inverse(curTransform);
	namedCoordinateSystems["camera"] = renderOptions->CameraToWorld;
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sCamera \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtMakeNamedMedium(const string &name, const ParamSet &params) {
	VERIFY_INITIALIZED("MakeNamedMedium");
	WARN_IF_ANIMATED_TRANSFORM("MakeNamedMedium");
	string type = params.FindOneString("type", "");
	if (type == "")
		Error("No parameter string \"type\" found in MakeNamedMedium");
	else {
		shared_ptr<Medium> medium =
			MakeMedium(type, params, curTransform[0]);
		if (medium) renderOptions->namedMedia[name] = medium;
	}
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sMakeNamedMedium \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtMediumInterface(const string &insideName, const string &outsideName) {
	VERIFY_INITIALIZED("MediumInterface");
	graphicsState.currentInsideMedium = insideName;
	graphicsState.currentOutsideMedium = outsideName;
	renderOptions->haveScatteringMedia = true;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sMediumInterface \"%s\" \"%s\"\n", catIndentCount, "",
			   insideName.c_str(), outsideName.c_str());
}

void pbrtWorldBegin() {
	VERIFY_OPTIONS("WorldBegin");
	currentApiState = APIState::WorldBlock;
	for (int i = 0; i < MaxTransforms; ++i) curTransform[i] = Transform();
	activeTransformBits = AllTransformsBits;
	namedCoordinateSystems["world"] = curTransform;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("\n\nWorldBegin\n\n");
}

void pbrtAttributeBegin() {
	VERIFY_WORLD("AttributeBegin");
	pushedGraphicsStates.push_back(graphicsState);
	pushedTransforms.push_back(curTransform);
	pushedActiveTransformBits.push_back(activeTransformBits);
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("\n%*sAttributeBegin\n", catIndentCount, "");
		catIndentCount += 4;
	}
}

void pbrtAttributeEnd() {
	VERIFY_WORLD("AttributeEnd");
	if (!pushedGraphicsStates.size()) {
		Error(
			"Unmatched pbrtAttributeEnd() encountered. "
			"Ignoring it.");
		return;
	}
	graphicsState = pushedGraphicsStates.back();
	pushedGraphicsStates.pop_back();
	curTransform = pushedTransforms.back();
	pushedTransforms.pop_back();
	activeTransformBits = pushedActiveTransformBits.back();
	pushedActiveTransformBits.pop_back();
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		catIndentCount -= 4;
		printf("%*sAttributeEnd\n", catIndentCount, "");
	}
}

void pbrtTransformBegin() {
	VERIFY_WORLD("TransformBegin");
	pushedTransforms.push_back(curTransform);
	pushedActiveTransformBits.push_back(activeTransformBits);
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sTransformBegin\n", catIndentCount, "");
		catIndentCount += 4;
	}
}

void pbrtTransformEnd() {
	VERIFY_WORLD("TransformEnd");
	if (!pushedTransforms.size()) {
		Error(
			"Unmatched pbrtTransformEnd() encountered. "
			"Ignoring it.");
		return;
	}
	curTransform = pushedTransforms.back();
	pushedTransforms.pop_back();
	activeTransformBits = pushedActiveTransformBits.back();
	pushedActiveTransformBits.pop_back();
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		catIndentCount -= 4;
		printf("%*sTransformEnd\n", catIndentCount, "");
	}
}

void pbrtTexture(const string &name, const string &type,
				 const string &texname, const ParamSet &params) {
	VERIFY_WORLD("Texture");
	TextureParams tp(params, params, graphicsState.floatTextures,
					 graphicsState.spectrumTextures);
	if (type == "float") {
		// Create _Float_ texture and store in _floatTextures_
		if (graphicsState.floatTextures.find(name) !=
			graphicsState.floatTextures.end())
			Warning("Texture \"%s\" being redefined", name.c_str());
		WARN_IF_ANIMATED_TRANSFORM("Texture");
		shared_ptr<Texture<Float>> ft =
			MakeFloatTexture(texname, curTransform[0], tp);
		if (ft) graphicsState.floatTextures[name] = ft;
	} else if (type == "color" || type == "spectrum") {
		// Create _color_ texture and store in _spectrumTextures_
		if (graphicsState.spectrumTextures.find(name) !=
			graphicsState.spectrumTextures.end())
			Warning("Texture \"%s\" being redefined", name.c_str());
		WARN_IF_ANIMATED_TRANSFORM("Texture");
		shared_ptr<Texture<Spectrum>> st =
			MakeSpectrumTexture(texname, curTransform[0], tp);
		if (st) graphicsState.spectrumTextures[name] = st;
	} else
		Error("Texture type \"%s\" unknown.", type.c_str());
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sTexture \"%s\" \"%s\" \"%s\" ", catIndentCount, "",
			   name.c_str(), type.c_str(), texname.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtMaterial(const string &name, const ParamSet &params) {
	VERIFY_WORLD("Material");
	graphicsState.material = name;
	graphicsState.materialParams = params;
	graphicsState.currentNamedMaterial = "";
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sMaterial \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtMakeNamedMaterial(const string &name, const ParamSet &params) {
	VERIFY_WORLD("MakeNamedMaterial");
	// error checking, warning if replace, what to use for transform?
	ParamSet emptyParams;
	TextureParams mp(params, emptyParams, graphicsState.floatTextures,
					 graphicsState.spectrumTextures);
	string matName = mp.FindString("type");
	WARN_IF_ANIMATED_TRANSFORM("MakeNamedMaterial");
	if (matName == "")
		Error("No parameter string \"type\" found in MakeNamedMaterial");

	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sMakeNamedMaterial \"%s\" ", catIndentCount, "",
			   name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	} else {
		shared_ptr<Material> mtl = MakeMaterial(matName, mp);
		if (graphicsState.namedMaterials.find(name) !=
			graphicsState.namedMaterials.end())
			Warning("Named material \"%s\" redefined.", name.c_str());
		graphicsState.namedMaterials[name] = mtl;
	}
}

void pbrtNamedMaterial(const string &name) {
	VERIFY_WORLD("NamedMaterial");
	graphicsState.currentNamedMaterial = name;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sNamedMaterial \"%s\"\n", catIndentCount, "", name.c_str());
}

void pbrtLightSource(const string &name, const ParamSet &params) {
	VERIFY_WORLD("LightSource");
	WARN_IF_ANIMATED_TRANSFORM("LightSource");
	MediumInterface mi = graphicsState.CreateMediumInterface();
	shared_ptr<Light> lt = MakeLight(name, params, curTransform[0], mi);
	if (!lt)
		Error("LightSource: light type \"%s\" unknown.", name.c_str());
	else
		renderOptions->lights.push_back(lt);
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sLightSource \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtAreaLightSource(const string &name, const ParamSet &params) {
	VERIFY_WORLD("AreaLightSource");
	graphicsState.areaLight = name;
	graphicsState.areaLightParams = params;
	if (PbrtOptions.cat || PbrtOptions.toPly) {
		printf("%*sAreaLightSource \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}
}

void pbrtShape(const string &name, const ParamSet &params)
{
	VERIFY_WORLD("Shape");
	vector<shared_ptr<Primitive>> prims;
	vector<shared_ptr<AreaLight>> areaLights;
	if (PbrtOptions.cat || (PbrtOptions.toPly && name != "trianglemesh"))
	{
		printf("%*sShape \"%s\" ", catIndentCount, "", name.c_str());
		params.Print(catIndentCount);
		printf("\n");
	}

	if (!curTransform.IsAnimated())
	{
		// Initialize _prims_ and _areaLights_ for static shape

		// Create shapes for shape _name_
		Transform *ObjToWorld, *WorldToObj;
		transformCache.Lookup(curTransform[0], &ObjToWorld, &WorldToObj);
		vector<shared_ptr<Shape>> shapes = MakeShapes(name, ObjToWorld, WorldToObj, graphicsState.reverseOrientation, params);
		if (shapes.empty())
		{
			return;
		}

		shared_ptr<Material> mtl = graphicsState.CreateMaterial(params);
		params.ReportUnused();
		MediumInterface mi = graphicsState.CreateMediumInterface();
		for (auto s : shapes)
		{
			// Possibly create area light for shape
			shared_ptr<AreaLight> area;
			if (graphicsState.areaLight != "")
			{
				area = MakeAreaLight(graphicsState.areaLight, curTransform[0], mi, graphicsState.areaLightParams, s);
				if (area)
				{
					areaLights.push_back(area);
				}
			}

			auto prim = make_shared<GeometricPrimitive>(s, mtl, area, mi);
			prim->SetIsSynthetic(_isSynthScene);
			prims.push_back(prim);
		}
	}
	else
	{
		// Initialize _prims_ and _areaLights_ for animated shape

		// Create initial shape or shapes for animated shape
		if (graphicsState.areaLight != "")
		{
			Warning("Ignoring currently set area light when creating animated shape");
		}
		
		Transform *identity;
		transformCache.Lookup(Transform(), &identity, nullptr);
		vector<shared_ptr<Shape>> shapes = MakeShapes(name, identity, identity, graphicsState.reverseOrientation, params);
		if (shapes.empty())
		{
			return;
		}

		// Create _GeometricPrimitive_(s) for animated shape
		shared_ptr<Material> mtl = graphicsState.CreateMaterial(params);
		params.ReportUnused();
		MediumInterface mi = graphicsState.CreateMediumInterface();
		for (auto s : shapes)
		{
			auto prim = make_shared<GeometricPrimitive>(s, mtl, nullptr, mi);
			prim->SetIsSynthetic(_isSynthScene);
			prims.push_back(prim);
		}

		// Create single _TransformedPrimitive_ for _prims_

		// Get _animatedObjectToWorld_ transform for shape
		static_assert(MaxTransforms == 2, "TransformCache assumes only two transforms");
		Transform *ObjToWorld[2];
		transformCache.Lookup(curTransform[0], &ObjToWorld[0], nullptr);
		transformCache.Lookup(curTransform[1], &ObjToWorld[1], nullptr);
		AnimatedTransform animatedObjectToWorld(ObjToWorld[0], renderOptions->transformStartTime, ObjToWorld[1], renderOptions->transformEndTime);
		if (prims.size() > 1)
		{
			shared_ptr<Primitive> bvh = make_shared<BVHAccel>(prims);
			bvh->SetIsSynthetic(_isSynthScene);
			prims.clear();
			prims.push_back(bvh);
		}

		prims[0] = make_shared<TransformedPrimitive>(prims[0], animatedObjectToWorld);
		prims[0]->SetIsSynthetic(_isSynthScene);
	}
	// Add _prims_ and _areaLights_ to scene or current instance
	if (renderOptions->currentInstance)
	{
		if (areaLights.size())
		{
			Warning("Area lights not supported with object instancing");
		}

		renderOptions->currentInstance->insert(renderOptions->currentInstance->end(), prims.begin(), prims.end());
	}
	else
	{
		renderOptions->primitives.insert(renderOptions->primitives.end(), prims.begin(), prims.end());
		if (areaLights.size())
		{
			renderOptions->lights.insert(renderOptions->lights.end(), areaLights.begin(), areaLights.end());
		}
	}
}

shared_ptr<Material> GraphicsState::CreateMaterial(const ParamSet &params) {
	TextureParams mp(params, materialParams, floatTextures, spectrumTextures);
	shared_ptr<Material> mtl;
	if (currentNamedMaterial != "") {
		if (namedMaterials.find(currentNamedMaterial) != namedMaterials.end())
			mtl = namedMaterials[graphicsState.currentNamedMaterial];
		else {
			Error("Named material \"%s\" not defined. Using \"matte\".",
				  currentNamedMaterial.c_str());
			mtl = MakeMaterial("matte", mp);
		}
	} else {
		mtl = MakeMaterial(material, mp);
		if (!mtl && material != "" && material != "none")
			mtl = MakeMaterial("matte", mp);
	}
	return mtl;
}

MediumInterface GraphicsState::CreateMediumInterface() {
	MediumInterface m;
	if (currentInsideMedium != "") {
		if (renderOptions->namedMedia.find(currentInsideMedium) !=
			renderOptions->namedMedia.end())
			m.inside = renderOptions->namedMedia[currentInsideMedium].get();
		else
			Error("Named medium \"%s\" undefined.",
				  currentInsideMedium.c_str());
	}
	if (currentOutsideMedium != "") {
		if (renderOptions->namedMedia.find(currentOutsideMedium) !=
			renderOptions->namedMedia.end())
			m.outside = renderOptions->namedMedia[currentOutsideMedium].get();
		else
			Error("Named medium \"%s\" undefined.",
				  currentOutsideMedium.c_str());
	}
	return m;
}

void pbrtReverseOrientation() {
	VERIFY_WORLD("ReverseOrientation");
	graphicsState.reverseOrientation = !graphicsState.reverseOrientation;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sReverseOrientation\n", catIndentCount, "");
}

void pbrtObjectBegin(const string &name) {
	VERIFY_WORLD("ObjectBegin");
	pbrtAttributeBegin();
	if (renderOptions->currentInstance)
		Error("ObjectBegin called inside of instance definition");
	renderOptions->instances[name] = vector<shared_ptr<Primitive>>();
	renderOptions->currentInstance = &renderOptions->instances[name];
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sObjectBegin \"%s\"\n", catIndentCount, "", name.c_str());
}

STAT_COUNTER("Scene/Object instances created", nObjectInstancesCreated);

void pbrtObjectEnd() {
	VERIFY_WORLD("ObjectEnd");
	if (!renderOptions->currentInstance)
		Error("ObjectEnd called outside of instance definition");
	renderOptions->currentInstance = nullptr;
	pbrtAttributeEnd();
	++nObjectInstancesCreated;
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sObjectEnd\n", catIndentCount, "");
}

STAT_COUNTER("Scene/Object instances used", nObjectInstancesUsed);

void pbrtObjectInstance(const string &name)
{
	VERIFY_WORLD("ObjectInstance");
	// Perform object instance error checking
	if (PbrtOptions.cat || PbrtOptions.toPly)
		printf("%*sObjectInstance \"%s\"\n", catIndentCount, "", name.c_str());
	if (renderOptions->currentInstance) {
		Error("ObjectInstance can't be called inside instance definition");
		return;
	}

	if (renderOptions->instances.find(name) == renderOptions->instances.end()) {
		Error("Unable to find instance named \"%s\"", name.c_str());
		return;
	}
	vector<shared_ptr<Primitive>> &in = renderOptions->instances[name];
	if (in.empty()) return;
	++nObjectInstancesUsed;
	if (in.size() > 1)
	{
		// Create aggregate for instance _Primitive_s
		shared_ptr<Primitive> accel(
			MakeAccelerator(renderOptions->AcceleratorName, in,
							renderOptions->AcceleratorParams));
		if (!accel) accel = make_shared<BVHAccel>(in);
		in.erase(in.begin(), in.end());
		in.push_back(accel);
	}

	static_assert(MaxTransforms == 2, "TransformCache assumes only two transforms");
	// Create _animatedInstanceToWorld_ transform for instance
	Transform *InstanceToWorld[2];
	transformCache.Lookup(curTransform[0], &InstanceToWorld[0], nullptr);
	transformCache.Lookup(curTransform[1], &InstanceToWorld[1], nullptr);
	AnimatedTransform animatedInstanceToWorld(InstanceToWorld[0], renderOptions->transformStartTime, InstanceToWorld[1], renderOptions->transformEndTime);
	shared_ptr<Primitive> prim(make_shared<TransformedPrimitive>(in[0], animatedInstanceToWorld));
	prim->SetIsSynthetic(_isSynthScene);
	renderOptions->primitives.push_back(prim);
}

void renderScene(float* buffer, bool inclLocal, bool inclSynth)
{
	renderOptions->_destBuffer = buffer;
	unique_ptr<Integrator> integrator(renderOptions->MakeIntegrator());
	unique_ptr<Scene> scene(renderOptions->MakeScene(inclLocal, inclSynth));
	CHECK_EQ(CurrentProfilerState(), ProfToBits(Prof::SceneConstruction));
	ProfilerState = ProfToBits(Prof::IntegratorRender);
	if (scene && integrator)
	{
		integrator->Render(*scene);
	}

	integrator.reset();
	scene.reset();

	CHECK_EQ(CurrentProfilerState(), ProfToBits(Prof::IntegratorRender));
	ProfilerState = ProfToBits(Prof::SceneConstruction);
}

inline bool fileExists(const string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void writeImg(const string &name, const string &extension, const Float *rgb, const Bounds2i bounds, const Point2i &size)
{
	if (extension.compare(".png") == 0)
	{
		WriteNoGammaCorrectedPNGImage(name + extension, rgb, bounds);
	}
	else
	{
		WriteImage(name + extension, rgb, bounds, size);
	}
}

void loadOrRenderImage(string name, string extension, int xRes, int yRes, Bounds2i bounds, float* buffer, bool inclLocal)
{
	bool imgLoaded = false;
	auto filename = name + extension;
	if(fileExists(filename))
	{
		Point2i size;
		unique_ptr<RGBSpectrum[]> img = ReadImage(filename, &size);

		if (size.x != xRes || size.y != yRes)
		{
			cout << name << " scene file with wrong dimentins found. Deleting file " << filename << endl;
			remove(filename.c_str());
		}
		else
		{
			cout << "Reusing " << name << " scene" << endl;

			for (int i = 0; i < xRes * yRes; i++ )
			{
				float *rgb = new float[3];
				img[i].ToRGB(rgb);
				buffer[i * 3 + 0] = rgb[0];
				buffer[i * 3 + 1] = rgb[1];
				buffer[i * 3 + 2] = rgb[2];
			}

			imgLoaded = true;
		}
	}
	
	if(!imgLoaded)
	{
		cout << "Rendering " << name << " scene" << endl;
		renderScene(buffer, inclLocal, false);
		writeImg(name, extension, buffer, bounds, Point2i(xRes, yRes));
	}
}

float ApplyFilter(float* image, float* filterMaks, int maskSize, int x, int y, int xRes)
{
	float result = 0;
	for (int i = 0; i < maskSize; i++)
	{
		for (int j = 0; j < maskSize; j++)
		{
			result += image[_noOfChannels * ((x + i) * xRes + y + j)] * filterMaks[i * maskSize + j];
		}
	}

	return result;
}

void smoothenMask(int xRes, int yRes, float* printMask, float* smoothenedMask)
{
	int enlargeBy = 1;
	int enlargedxRes = (xRes + 2 * enlargeBy);
	int enlargedyRes = (yRes + 2 * enlargeBy);
	int enlargedPixels = enlargedxRes * enlargedyRes;
	float *enlargedOriginal = new float[_noOfChannels * enlargedPixels];

	// enlarge image
	for (int i = 0; i < enlargedyRes; i++)
	{
		for (int j = 0; j < enlargedxRes; j++)
		{
			if (j < enlargeBy || i < enlargeBy || i >= yRes + enlargeBy || j >= xRes + enlargeBy)
			{
				enlargedOriginal[_noOfChannels * (i * enlargedxRes + j)] = 0;
			}
			else
			{
				enlargedOriginal[_noOfChannels * (i * enlargedxRes + j)] = printMask[_noOfChannels * ((i - enlargeBy) * (xRes)+j - enlargeBy)];
			}
		}
	}

	// Apply filter mask
	float *filterMask = new float[9] {
            0.075f, 0.075f, 0.075f,
            0.075f, 0.4f, 0.075f,
            0.075f, 0.075f, 0.075f
    };
	for(int i = 0; i < yRes; i++)
	{
		for (int j = 0; j < xRes; j++)
		{
			float value = ApplyFilter(enlargedOriginal, filterMask, enlargeBy *2 + 1, i, j, enlargedxRes);
			smoothenedMask[_noOfChannels * (i * xRes + j) + 0] = value;
			smoothenedMask[_noOfChannels * (i * xRes + j) + 1] = value;
			smoothenedMask[_noOfChannels * (i * xRes + j) + 2] = value;
		}
	}

	delete[] enlargedOriginal;
	delete[] filterMask;
}

void differentialRendering()
{
	string filename = renderOptions->FilmParams.FindOneFilename("filename", "");
	if(filename.empty())
	{
		cout << "Filename needs to be specified." << endl;
		return;
	}

	string extension = filename.substr(filename.find_last_of('.'));
	//transform(extension.begin(), extension.end(), extension.begin(), tolower);

	double tolerance = 0.01;
	if(extension.compare(".png") == 0)
	{
		tolerance = 0.04;
	}

	string bgFile = renderOptions->_differentialBg.FindOneFilename("filename", "");
	if (bgFile.empty())
	{
		cout << "Differential rendering requires a background image." << endl;
		return;
	}

	Point2i size;
	unique_ptr<RGBSpectrum[]> bg = ReadImage(bgFile, &size);
	int xRes = renderOptions->FilmParams.FindOneInt("xresolution", 0);
	int yRes = renderOptions->FilmParams.FindOneInt("yresolution", 0);
	if (size.x != xRes || size.y != yRes)
	{
		cout << "Background image must match size specified in pbrt file" << endl;
		return;
	}

	int noOfPixels = xRes * yRes;
	int bufferSize = noOfPixels * _noOfChannels;

	// render the complete scene (local + synthetic)
	cout << "Rendering complete scene" << endl;
	float *complete = new float[bufferSize];
	renderScene(complete, true, true);
	auto bounds = Bounds2i({ 0, 0 }, size);
	writeImg("complete", extension, complete, bounds, size);

	//float *original = new float[bufferSize];
	//for (int i = 0; i < noOfPixels; i++)
	//{
	//	float *rgb = new float[3];
	//	bg[i].ToRGB(rgb);
	//	//bg[i].ToRGB(rgb);
	//	original[i * 3 + 0] = rgb[0];
	//	original[i * 3 + 1] = rgb[1];
	//	original[i * 3 + 2] = rgb[2];
	//}

	//WriteImage("test.exr", original, bounds, size);

	// render the local scene
	float *local = new float[bufferSize];
	loadOrRenderImage("local", extension, xRes, yRes, bounds, local, true);
	//cout << "Rendering local scene" << endl;
	//renderScene(local, true, false);
	//writeImg("local", extension, local, bounds, size);

	// render the synthetic scene
	cout << "Rendering synthetic scene" << endl;
	float *synthetic = new float[bufferSize];
	renderScene(synthetic, false, true);
	writeImg("synthetic", extension, synthetic, bounds, size);

	// render environment only
	float *environment = new float[bufferSize];
	loadOrRenderImage("environment", extension, xRes, yRes, bounds, environment, false);
	//cout << "Rendering environment" << endl;
	//renderScene(environment, false, false);
	//writeImg("environment", extension, environment, bounds, size);

	float *printMask = new float[bufferSize];
	float *smoothenedMask = new float[bufferSize];
	float *diffImg = new float[bufferSize];
	float *fin = new float[bufferSize];
	for (int i = 0; i < noOfPixels; i++)
	{
		// calculate mask
		float diff = abs(synthetic[_noOfChannels * i + 0] - environment[_noOfChannels * i + 0]) +
			abs(synthetic[_noOfChannels * i + 1] - environment[_noOfChannels * i + 1]) +
			abs(synthetic[_noOfChannels * i + 2] - environment[_noOfChannels * i + 2]);

		diff = diff > tolerance ? 1 : 0;
		//printMask[i] = diff;
		printMask[_noOfChannels * i + 0] = printMask[_noOfChannels * i + 1] = printMask[_noOfChannels * i + 2] = diff;
	}

	smoothenMask(xRes, yRes, printMask, smoothenedMask);

	for (int i = 0; i < noOfPixels; i++)
	{
		float diff = smoothenedMask[_noOfChannels * i];
		float notDiff = 1.0 - diff;

		float changeR = (complete[_noOfChannels * i + 0] - local[_noOfChannels * i + 0]) * notDiff;
		float changeG = (complete[_noOfChannels * i + 1] - local[_noOfChannels * i + 1]) * notDiff;
		float changeB = (complete[_noOfChannels * i + 2] - local[_noOfChannels * i + 2]) * notDiff;
		diffImg[_noOfChannels * i + 0] = changeR + 0.5;
		diffImg[_noOfChannels * i + 1] = changeG + 0.5;
		diffImg[_noOfChannels * i + 2] = changeB + 0.5;

		// Calculate final image
		float orig[3];
		bg[i].ToRGB(orig);
		fin[_noOfChannels * i + 0] = orig[0] * notDiff + synthetic[_noOfChannels * i + 0] * diff + changeR;
		fin[_noOfChannels * i + 1] = orig[1] * notDiff + synthetic[_noOfChannels * i + 1] * diff + changeG;
		fin[_noOfChannels * i + 2] = orig[2] * notDiff + synthetic[_noOfChannels * i + 2] * diff + changeB;
	}

	writeImg("mask", extension, printMask, bounds, size);
	//auto s2 = Point2i(enlargedxRes, enlargedyRes);
	//auto b2 = Bounds2i({ 0, 0 }, s2);
	//writeImg("mask_", extension, enlargedOriginal, b2, s2);
	writeImg("smoothenedMask", extension, smoothenedMask, bounds, size);
	writeImg("diff", extension, diffImg, bounds, size);
	writeImg("final", extension, fin, bounds, size);

	// Write actual usable image
	string filenameOnly = filename.substr(0, filename.find_last_of('.'));
	stringstream ss;
	ss << filenameOnly << setw(4) << setfill('0') << PbrtOptions.frameNumber;
	writeImg(ss.str(), extension, fin, bounds, size);

	//if(extension.compare(".png") == 0)
	//{
	//	WriteNoGammaCorrectedPNGImage("complete" + extension, complete, bounds);
	//	WriteNoGammaCorrectedPNGImage("local" + extension, local, bounds);
	//	WriteNoGammaCorrectedPNGImage("synthetic" + extension, synthetic, bounds);
	//	WriteNoGammaCorrectedPNGImage("environment" + extension, environment, bounds);
	//	WriteNoGammaCorrectedPNGImage("mask" + extension, printMask, bounds);
	//	WriteNoGammaCorrectedPNGImage("diff" + extension, diffImg, bounds);
	//	WriteNoGammaCorrectedPNGImage("final" + extension, fin, bounds);
	//}
	//else
	//{
	//	
	//	//WriteImage("complete" + extension, complete, bounds, size);
	//	//WriteImage("local" + extension, local, bounds, size);
	//	//WriteImage("synthetic" + extension, synthetic, bounds, size);
	//	//WriteImage("environment" + extension, environment, bounds, size);
	//	//WriteImage("mask" + extension, printMask, bounds, size);
	//	//WriteImage("diff" + extension, diffImg, bounds, size);
	//	//WriteImage("final" + extension, fin, bounds, size);
	//}

	// cleaning up
	delete[] complete;
	delete[] local;
	delete[] synthetic;
	delete[] environment;
	//delete[] mask;
	delete[] printMask;
	delete[] smoothenedMask;
	delete[] diffImg;
	delete[] fin;
}

void pbrtWorldEnd()
{
	VERIFY_WORLD("WorldEnd");
	// Ensure there are no pushed graphics states
	while (pushedGraphicsStates.size())
	{
		Warning("Missing end to pbrtAttributeBegin()");
		pushedGraphicsStates.pop_back();
		pushedTransforms.pop_back();
	}
	while (pushedTransforms.size())
	{
		Warning("Missing end to pbrtTransformBegin()");
		pushedTransforms.pop_back();
	}

	// Create scene and render
	differentialRendering();
	//if (PbrtOptions.cat || PbrtOptions.toPly) {
	//	printf("%*sWorldEnd\n", catIndentCount, "");
	//} else {
	//	unique_ptr<Integrator> integrator(renderOptions->MakeIntegrator());
	//	unique_ptr<Scene> scene(renderOptions->MakeScene());

	//	// This is kind of ugly; we directly override the current profiler
	//	// state to switch from parsing/scene construction related stuff to
	//	// rendering stuff and then switch it back below. The underlying
	//	// issue is that all the rest of the profiling system assumes
	//	// hierarchical inheritance of profiling state; this is the only
	//	// place where that isn't the case.
	//	CHECK_EQ(CurrentProfilerState(), ProfToBits(Prof::SceneConstruction));
	//	ProfilerState = ProfToBits(Prof::IntegratorRender);

	//	if (scene && integrator) integrator->Render(*scene);

	//	CHECK_EQ(CurrentProfilerState(), ProfToBits(Prof::IntegratorRender));
	//	ProfilerState = ProfToBits(Prof::SceneConstruction);
	//}

	// Clean up after rendering. Do this before reporting stats so that
	// destructors can run and update stats as needed.
	graphicsState = GraphicsState();
	transformCache.Clear();
	currentApiState = APIState::OptionsBlock;
	ImageTexture<Float, Float>::ClearCache();
	ImageTexture<RGBSpectrum, Spectrum>::ClearCache();

	if (!PbrtOptions.cat && !PbrtOptions.toPly)
	{
		MergeWorkerThreadStats();
		ReportThreadStats();
		if (!PbrtOptions.quiet)
		{
			PrintStats(stdout);
			ReportProfilerResults(stdout);
			ClearStats();
			ClearProfiler();
		}
	}

	for (int i = 0; i < MaxTransforms; ++i)
	{
		curTransform[i] = Transform();
	}

	activeTransformBits = AllTransformsBits;
	namedCoordinateSystems.erase(namedCoordinateSystems.begin(), namedCoordinateSystems.end());
}

void cgraScenePhoto(const ParamSet params)
{
	renderOptions->_differentialBg = params;
}

void cgraSynthSceneBegin()
{
	_isSynthScene = true;
}

void cgraSynthSceneEnd()
{
	_isSynthScene = false;
}

void cgraAnimation(const ParamSet params)
{
	string file = params.FindOneFilename("filename", "");
	if(file.empty() || PbrtOptions.frameNumber == 0)
	{
		return;
	}

	string line;
	ifstream myfile(file);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if(line[0] == '\t' || line[0] == ' ')
			{
				continue;
			}

			try
			{
				int index = stoi(line);
				if (index == PbrtOptions.frameNumber)
				{
					while(getline(myfile, line))
					{
						if (line[0] != '\t' && line[0] != ' ')
						{
							return;
						}

						trim(line);

						if(line.empty())
						{
							continue;
						}

						vector<string> parts = splitString(line, " ");

						if(parts[0].compare("ConcatTransform") == 0)
						{
							int count = 16;
							float *transform = new float[count];

							for(int i = 0; i < count; i++)
							{
								string t = parts[i + 1];
								trim(t, '[');
								trim(t, ']');
								transform[i] = stof(t);
							}

							pbrtConcatTransform(transform);
						}
						else if(parts[0].compare("Translate") == 0)
						{
							pbrtTranslate(stof(parts[1]), stof(parts[2]), stof(parts[3]));
						}
						else if(parts[0].compare("Rotate") == 0)
						{
							pbrtRotate(stof(parts[1]), stof(parts[2]), stof(parts[3]), stof(parts[4]));
						}
						else
						{
							cout << "not supported command(" << parts[0] << ") found in animation file.";
						}
					}
				}
			}
			catch(exception e)
			{
				cout << "unexpected tolken found in animation file." << endl;
			}
		}

		myfile.close();
	}
	else
	{
		cout << "Unable to open animation file." << endl;
	}
}

Scene *RenderOptions::MakeScene(bool includeLocal, bool includeSynth)
{
	vector<shared_ptr<Primitive>> prim = vector<shared_ptr<Primitive>>();

	for (shared_ptr<Primitive> primitive : primitives)
	{
		if((includeLocal && !primitive->GetIsSynthetic()) || (includeSynth && primitive->GetIsSynthetic()))
		{
			prim.push_back(primitive);
		}
	}

	shared_ptr<Primitive> accelerator = MakeAccelerator(AcceleratorName, prim, AcceleratorParams);
	if (!accelerator)
	{
		accelerator = make_shared<BVHAccel>(prim);
	}

	Scene *scene = new Scene(accelerator, lights);
	//// Erase primitives and lights from _RenderOptions_
	//primitives.erase(primitives.begin(), primitives.end());
	//lights.erase(lights.begin(), lights.end());
	return scene;
}

Integrator *RenderOptions::MakeIntegrator() const
{
	shared_ptr<const Camera> camera(MakeCamera());
	if (!camera)
	{
		Error("Unable to create camera");
		return nullptr;
	}

	shared_ptr<Sampler> sampler = MakeSampler(SamplerName, SamplerParams, camera->film);
	if (!sampler)
	{
		Error("Unable to create sampler.");
		return nullptr;
	}

	Integrator *integrator = nullptr;
	if (IntegratorName == "whitted")
	{
		integrator = CreateWhittedIntegrator(IntegratorParams, sampler, camera);
	}
	else if (IntegratorName == "directlighting")
	{
		integrator = CreateDirectLightingIntegrator(IntegratorParams, sampler, camera);
	}
	else if (IntegratorName == "path")
	{
		integrator = CreatePathIntegrator(IntegratorParams, sampler, camera);
	}
	else if (IntegratorName == "volpath")
	{
		integrator = CreateVolPathIntegrator(IntegratorParams, sampler, camera);
	}
	else if (IntegratorName == "bdpt")
	{
		integrator = CreateBDPTIntegrator(IntegratorParams, sampler, camera);
	}
	else if (IntegratorName == "mlt")
	{
		integrator = CreateMLTIntegrator(IntegratorParams, camera);
	} 
	else if (IntegratorName == "ambientocclusion")
	{
		integrator = CreateAOIntegrator(IntegratorParams, sampler, camera);
	}
	else if (IntegratorName == "sppm")
	{
		integrator = CreateSPPMIntegrator(IntegratorParams, camera);
	} 
	else
	{
		Error("Integrator \"%s\" unknown.", IntegratorName.c_str());
		return nullptr;
	}

	if (renderOptions->haveScatteringMedia && IntegratorName != "volpath" &&
		IntegratorName != "bdpt" && IntegratorName != "mlt")
	{
		Warning(
			"Scene has scattering media but \"%s\" integrator doesn't support "
			"volume scattering. Consider using \"volpath\", \"bdpt\", or "
			"\"mlt\".", IntegratorName.c_str());
	}

	IntegratorParams.ReportUnused();
	// Warn if no light sources are defined
	if (lights.empty())
	{
		Warning("No light sources defined in scene; rendering a black image.");
	}
	
	return integrator;
}

Camera *RenderOptions::MakeCamera() const {
	unique_ptr<Filter> filter = MakeFilter(FilterName, FilterParams);
	Film *film = MakeFilm(FilmName, FilmParams, move(filter));
	if (!film)
	{
		Error("Unable to create film.");
		return nullptr;
	}

	Camera *camera = pbrt::MakeCamera(CameraName, CameraParams, CameraToWorld,
		renderOptions->transformStartTime, renderOptions->transformEndTime, film);
	return camera;
}
}  // namespace pbrt
