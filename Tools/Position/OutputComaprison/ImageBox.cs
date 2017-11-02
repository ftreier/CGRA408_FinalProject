using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using FreeImageAPI;

namespace OutputComaprison
{
	public partial class ImageBox : UserControl
	{
		public string FileName { get; private set; }
		private string BasePath { get; set; }

		public ImageBox()
		{
			InitializeComponent();
		}

		public ImageBox(string basePath, string fileName) : this()
		{
			BasePath = basePath;
			FileName = fileName;
		}

		public void ReloadImage()
		{
			FIBITMAP dib = new FIBITMAP();

			if (!FreeImage.IsAvailable())
			{
				Console.WriteLine(@"FreeImage.dll seems to be missing. Aborting.");
				return;
			}

			string filename = Path.Combine(BasePath, FileName + ".exr");

			if (!File.Exists(filename))
			{
				filename = Path.Combine(BasePath, FileName + ".png");
				if (!File.Exists(filename))
				{
					Console.WriteLine(filename + @" does not exist. Aborting.");
					return;
				}
			}

			FREE_IMAGE_FORMAT fif = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			dib = FreeImage.LoadEx(filename, ref fif);

			// Check if the handle is null which means the bitmap could not be loaded.
			if (dib.IsNull)
			{
				Console.WriteLine(@"Loading bitmap failed. Aborting.");
				// Check whether there was an error message.
				return;
			}

			Bitmap bitmap;
			if (fif == FREE_IMAGE_FORMAT.FIF_EXR)
			{
				bitmap = FreeImage.GetBitmap(FreeImage.TmoReinhard05(dib, 0, 0));
			}
			else
			{
				bitmap = FreeImage.GetBitmap(dib);
			}

			if (bitmap != null)
			{
				_pictureBox.Image?.Dispose();

				_pictureBox.Image = bitmap;
			}

			// The bitmap was saved to disk but is still allocated in memory, so the handle has to be freed.
			if (!dib.IsNull)
			{
				FreeImage.Unload(dib);
			}

			// Make sure to set the handle to null so that it is clear that the handle is not pointing to a bitmap.
			dib.SetNull();

			_label.Text = FileName;
		}

		public void SetBasePath(string baseImgPath)
		{
			if (!string.Equals(BasePath, baseImgPath, StringComparison.CurrentCultureIgnoreCase))
			{
				BasePath = baseImgPath;
				ReloadImage();
			}
		}
	}
}
