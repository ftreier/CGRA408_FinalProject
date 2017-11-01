using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using FreeImageAPI;

namespace OutputComaprison
{
	public partial class ImageBox : UserControl
	{
		public ImageBox()
		{
			InitializeComponent();
		}

		public void SetImage(string fileName)
		{
			FIBITMAP dib = new FIBITMAP();

			if (!FreeImage.IsAvailable())
			{
				Console.WriteLine(@"FreeImage.dll seems to be missing. Aborting.");
				return;
			}

			if (!File.Exists(fileName))
			{
				Console.WriteLine(fileName + @" does not exist. Aborting.");
				return;
			}

			if (!dib.IsNull)
			{
				FreeImage.Unload(dib);
			}

			FREE_IMAGE_FORMAT fif = FREE_IMAGE_FORMAT.FIF_UNKNOWN;
			dib = FreeImage.LoadEx(fileName, ref fif);

			// Check if the handle is null which means the bitmap could not be loaded.
			if (dib.IsNull)
			{
				Console.WriteLine(@"Loading bitmap failed. Aborting.");
				// Check whether there was an error message.
				return;
			}

			Bitmap bitmap = FreeImage.GetBitmap(FreeImage.TmoReinhard05(dib, 0, 0));

			if (bitmap != null)
			{
				if (_pictureBox.Image != null)
				{
					_pictureBox.Image.Dispose();
				}

				_pictureBox.Image = bitmap;
			}

			// The bitmap was saved to disk but is still allocated in memory, so the handle has to be freed.
			if (!dib.IsNull)
			{
				FreeImage.Unload(dib);
			}

			// Make sure to set the handle to null so that it is clear that the handle is not pointing to a bitmap.
			dib.SetNull();

			_label.Text = FindFileName(fileName);
		}

		private string FindFileName(string fileName)
		{
			string[] parts = fileName.Trim().Replace('\\', '/').Split('/');
			return parts[parts.Length - 1];
		}
	}
}
