using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using Crom.Controls;
using OutputComaprison.Properties;

namespace OutputComaprison
{
	public partial class MainForm : Form
	{
		private readonly List<ImageBox> _imageBoxes;
		private readonly HashSet<DockableToolWindow> _updatingSize;
		private readonly Dictionary<string, zDockMode> _imageNames;

		private string _pbrtPath;
		private string _baseImgPath;

		public MainForm()
		{
			InitializeComponent();

			_imageBoxes = new List<ImageBox>();
			_updatingSize = new HashSet<DockableToolWindow>();
			_imageNames = new Dictionary<string, zDockMode>
			{
				{"complete", zDockMode.Left},
				{"diff", zDockMode.Bottom},
				{"mask", zDockMode.Bottom},
				{"synthetic", zDockMode.Right},
				{"local", zDockMode.Right},
				{"environment", zDockMode.Left},
				{"final", zDockMode.Right}
			};

			_dockContainer.BottomPanelHeight = 400;
		}

		private void CreateImages(string fileName, zDockMode dockMode)
		{
			DockableToolWindow childForm = new DockableToolWindow();
			ImageBox ib = new ImageBox(_baseImgPath, fileName) {Dock = DockStyle.Fill};
			childForm.Controls.Add(ib);
			childForm.Text = ib.FileName;
			childForm.Width = 400;
			childForm.Height = 300;

			childForm.SizeChanged += ChildForm_SizeChanged;

			// Add the form to the dock container
			_dockContainer.DockToolWindow(childForm, dockMode);

			// Show the form
			childForm.Show();
			_imageBoxes.Add(ib);
			ib.ReloadImage();
		}

		private void ChildForm_SizeChanged(object sender, System.EventArgs e)
		{
			var childForm = sender as DockableToolWindow;
			if (childForm != null && !_updatingSize.Contains(childForm))
			{
				_updatingSize.Add(childForm);
				childForm.Width = 400;
				childForm.Height = 300;
				_updatingSize.Remove(childForm);
			}
		}

		private void ReloadImages()
		{
			foreach (var imageBox in _imageBoxes)
			{
				imageBox.ReloadImage();
			}
		}

		private void MainForm_Load(object sender, System.EventArgs e)
		{
			WindowState = FormWindowState.Maximized;

			_baseImgPath = Settings.Default.ImageBasePath;
			_pbrtPath = Settings.Default.PBRTPath;

			foreach (var imageName in _imageNames)
			{
				CreateImages(imageName.Key, imageName.Value);
			}

			UpdateBaseImgPath();
		}

		private void UpdateBaseImgPath()
		{
			_baseImgPathTb.Text = _baseImgPath;
			try
			{
				if (File.Exists(Path.Combine(_baseImgPath, _imageNames.First().Key + ".exr")) ||
				    File.Exists(Path.Combine(_baseImgPath, _imageNames.First().Key + ".png")))
				{
					_baseImgPathTb.ForeColor = Color.Black;
					_baseImgPathLbl.ForeColor = Color.Black;

					foreach (var imageBox in _imageBoxes)
					{
						imageBox.SetBasePath(_baseImgPath);
					}
				}
				else
				{
					_baseImgPathTb.ForeColor = Color.Red;
					_baseImgPathLbl.ForeColor = Color.Red;
				}
			}
			catch (Exception)
			{
				_baseImgPath = "";
				_baseImgPathTb.Text = "";
			}
		}

		private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			Settings.Default.ImageBasePath = _baseImgPath;
			Settings.Default.PBRTPath = _pbrtPath;
			Settings.Default.Save();
		}

		private void _baseImgPathTb_TextChanged(object sender, System.EventArgs e)
		{
			_baseImgPath = _baseImgPathTb.Text;
			UpdateBaseImgPath();
		}
	}
}
