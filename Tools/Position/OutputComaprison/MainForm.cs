using System.Windows.Forms;
using Crom.Controls;

namespace OutputComaprison
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();

			DockableToolWindow childForm = new DockableToolWindow();
			ImageBox ib = new ImageBox();
			ib.Dock = DockStyle.Fill;
			childForm.Controls.Add(ib);

			// Add the form to the dock container
			_dockContainer.AddToolWindow(childForm);
			_dockContainer.AddToolWindow(childForm);

			// Show the form
			childForm.Show();

			string fileName = @"C:\Data\Victoria\CGRA408_Rendering\FinalProject\CGRA408_FinalProject\complete.exr";
			//string fileName = @"C:\Data\Fotos\Ronja\Schwimmabzeichen.JPG";
			//string outFileName = @"C:\Data\Victoria\CGRA408_Rendering\FinalProject\CGRA408_FinalProject\diff_.jpg";

			ib.SetImage(fileName);

			//var file = EXRFile.FromFile(filename);
			//var part = file.Parts[0];

			//part.OpenParallel(filename);

			//var data = part.GetBytes(ImageDestFormat.PremultipliedRGBA16, GammaEncoding.sRGB);

			//FIBITMAP dib = new FIBITMAP();

			//if (!FreeImage.IsAvailable())
			//{
			//	Console.WriteLine("FreeImage.dll seems to be missing. Aborting.");
			//	return;
			//}

			//if (!File.Exists(fileName))
			//{
			//	Console.WriteLine(fileName + " does not exist. Aborting.");
			//	return;
			//}

			//// Try to unload the bitmap handle (in case it is not null).
			//// Always assert that a handle (like dib) is unloaded before it is reused, because
			//// on unmanaged side there is no garbage collector that will clean up unreferenced
			//// objects.
			//// The following code will produce a memory leak (in case the bitmap is loaded
			//// successfully) because the handle to the first bitmap is lost:
			////   dib = FreeImage.Load(FREE_IMAGE_FORMAT.FIF_JPEG, fileName, FREE_IMAGE_LOAD_FLAGS.JPEG_ACCURATE);
			////   dib = FreeImage.Load(FREE_IMAGE_FORMAT.FIF_JPEG, fileName, FREE_IMAGE_LOAD_FLAGS.JPEG_ACCURATE);
			//if (!dib.IsNull)
			//	FreeImage.Unload(dib);

			//// Loading a sample bitmap. In this case it's a .jpg file. 'Load' requires the file
			//// format or the loading process will fail. An additional flag (the default value is
			//// 'DEFAULT') can be set to enable special loading options.
			//FREE_IMAGE_FORMAT fif = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			//dib = FreeImage.LoadEx(fileName, ref fif);
			////dib = FreeImage.Load(FREE_IMAGE_FORMAT.FIF_EXR, fileName, FREE_IMAGE_LOAD_FLAGS.DEFAULT);
			////dib = FreeImage.Load(FREE_IMAGE_FORMAT.FIF_JPEG, fileName, FREE_IMAGE_LOAD_FLAGS.JPEG_ACCURATE);

			//// Check if the handle is null which means the bitmap could not be loaded.
			//if (dib.IsNull)
			//{
			//	Console.WriteLine("Loading bitmap failed. Aborting.");
			//	// Check whether there was an error message.
			//	return;
			//}

			//// Try flipping the bitmap.
			//if (!FreeImage.FlipHorizontal(dib))
			//{
			//	Console.WriteLine("Unable to flip bitmap.");
			//	// Check whether there was an error message.
			//}

			//// Store the bitmap back to disk. Again the desired format is needed. In this case the format is 'TIFF'.
			//// An output filename has to be chosen (which will be overwritten without a warning).
			//// A flag can be provided to enable pluginfunctions (compression is this case).
			////FreeImage.Save(FREE_IMAGE_FORMAT.FIF_EXR, dib, outFileName, FREE_IMAGE_SAVE_FLAGS.EXR_NONE);
			//FIBITMAP c = FreeImage.TmoReinhard05(dib, 0, 0);
			////FIBITMAP c = FreeImage.ConvertToStandardType(FreeImage.ConvertToType(dib, FREE_IMAGE_TYPE.FIT_FLOAT, true), true);
			////FIBITMAP c = FreeImage.ConvertToType(dib, FREE_IMAGE_TYPE.FIT_RGBF, true);
			//FreeImage.Save(FREE_IMAGE_FORMAT.FIF_JPEG, c, outFileName, FREE_IMAGE_SAVE_FLAGS.JPEG_QUALITYNORMAL);
			////FreeImage.Save(FREE_IMAGE_FORMAT.FIF_JPEG, dib, outFileName, FREE_IMAGE_SAVE_FLAGS.JPEG_QUALITYNORMAL);

			//// The bitmap was saved to disk but is still allocated in memory, so the handle has to be freed.
			//if (!dib.IsNull)
			//	FreeImage.Unload(dib);

			//// Make sure to set the handle to null so that it is clear that the handle is not pointing to a bitmap.
			//dib.SetNull();
		}
	}
}
